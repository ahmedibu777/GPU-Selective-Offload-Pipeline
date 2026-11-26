#include <cassert>
#include <cstdint>
#include <vector>

#include "offload/event_selector.hpp"

int main() {
    offload::EventSelector selector;
    selector.configure(
        offload::ValueWeights{0.3, 0.2, 0.4, 0.1},
        0.2,
        true);

    std::vector<std::uint8_t> payload(1024, 7);

    offload::Frame frame;
    frame.frame_id = 1;
    frame.timestamp_s = 0.0;
    frame.rarity = 0.8;
    frame.uncertainty = 0.7;
    frame.criticality = 0.9;
    frame.similarity_penalty = 0.1;
    frame.remaining_slack_s = 0.5;
    frame.data_ptr = payload.data();
    frame.data_size_bytes = payload.size();

    offload::Event event;
    assert(selector.select(frame, event));
    assert(event.data_size_bytes == payload.size());
    assert(event.scheduling_priority > 0.0);

    frame.remaining_slack_s = 0.0;
    assert(!selector.select(frame, event));

    return 0;
}
