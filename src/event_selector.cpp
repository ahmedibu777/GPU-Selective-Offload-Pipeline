#include "offload/event_selector.hpp"

#include <algorithm>

namespace offload {

double EventSelector::clamp01(double value) {
    return std::min(1.0, std::max(0.0, value));
}

void EventSelector::configure(const ValueWeights& weights,
                              double minimum_value_score,
                              bool require_safety_guard) {
    weights_ = weights;
    minimum_value_score_ = minimum_value_score;
    require_safety_guard_ = require_safety_guard;
}

bool EventSelector::select(const Frame& frame, Event& event) const {
    if (require_safety_guard_ && frame.remaining_slack_s <= 0.0) {
        return false;
    }

    if (frame.data_ptr == nullptr || frame.data_size_bytes == 0) {
        return false;
    }

    const double rarity = clamp01(frame.rarity);
    const double uncertainty = clamp01(frame.uncertainty);
    const double criticality = clamp01(frame.criticality);
    const double similarity = clamp01(frame.similarity_penalty);

    const double value =
        weights_.rarity * rarity +
        weights_.uncertainty * uncertainty +
        weights_.criticality * criticality -
        weights_.similarity * similarity;

    if (value < minimum_value_score_) {
        return false;
    }

    const double slack = std::max(frame.remaining_slack_s, 0.001);
    const double scheduling_priority = value / slack;

    event.event_id = frame.frame_id;
    event.frame_id = frame.frame_id;
    event.created_at_s = frame.timestamp_s;
    event.deadline_s = frame.timestamp_s + frame.remaining_slack_s;
    event.priority = frame.priority;
    event.value_score = value;
    event.scheduling_priority = scheduling_priority;
    event.data_ptr = frame.data_ptr;
    event.data_size_bytes = frame.data_size_bytes;

    return true;
}

}  // namespace offload
