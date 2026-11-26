#pragma once

#include <cstddef>
#include <cstdint>

#include "offload/common/types.hpp"

namespace offload {

struct Frame {
    std::uint64_t frame_id{};
    double timestamp_s{};
    Imu imu{};
    Gps gps{};
    EventType event_type{EventType::Unknown};

    std::uint8_t priority{};
    double rarity{};
    double uncertainty{};
    double criticality{};
    double similarity_penalty{};
    double remaining_slack_s{};

    const std::uint8_t* data_ptr{nullptr};
    std::size_t data_size_bytes{};
};

struct Event {
    std::uint64_t event_id{};
    std::uint64_t frame_id{};
    double created_at_s{};
    double deadline_s{};

    std::uint8_t priority{};
    double value_score{};
    double scheduling_priority{};

    const std::uint8_t* data_ptr{nullptr};
    std::size_t data_size_bytes{};
};

}  // namespace offload
