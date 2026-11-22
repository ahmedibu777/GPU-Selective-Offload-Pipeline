#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "offload/common/types.hpp"

namespace offload::recorder {

struct Frame {
    std::uint64_t timestamp_us = 0;
    common::SensorType sensor = common::SensorType::Camera;
    std::array<std::uint32_t, 3> dimensions{0, 0, 0};
    std::size_t data_size = 0;
    // Non-owning payload view; the caller owns storage and must outlive Frame.
    const std::uint8_t* data_ptr = nullptr;
    // 0 is lowest/drop; 255 is highest/immediate.
    std::uint8_t priority = 128;
};

}  // namespace offload::recorder

namespace offload {
using recorder::Frame;
}  // namespace offload
