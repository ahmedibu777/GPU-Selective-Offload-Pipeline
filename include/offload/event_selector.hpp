#pragma once

#include "offload/common/frame.hpp"
#include "offload/common/types.hpp"

namespace offload {

class EventSelector {
public:
    EventSelector() = default;

    void configure(const ValueWeights& weights,
                   double minimum_value_score,
                   bool require_safety_guard);

    bool select(const Frame& frame, Event& event) const;

private:
    static double clamp01(double value);

    ValueWeights weights_{0.30, 0.25, 0.35, 0.10};
    double minimum_value_score_{0.0};
    bool require_safety_guard_{true};
};

}  // namespace offload
