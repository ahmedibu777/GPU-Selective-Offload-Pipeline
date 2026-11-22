#pragma once

#include <cstdint>
#include <string>

#include "offload/recorder/frame.hpp"

namespace offload::common {

enum class SensorType : std::uint8_t { Camera, Lidar, Radar, Imu, Gps };
enum class EventType : std::uint8_t {
    FrameReady, ErrorOccurred, ProcessingComplete, NormalDriving,
    PedestrianDetected, CollisionLike, RareObject
};

struct Event {
    std::uint64_t timestamp_us = 0;
    EventType type = EventType::FrameReady;
    float confidence = 0.0F;
    std::uint8_t priority = 128;
    std::string metadata;
};

}  // namespace offload::common

namespace offload {
using common::SensorType;
using common::EventType;
using common::Event;
}  // namespace offload
