#pragma once

#include <cstdint>
#include <string>

namespace offload
{

enum class SensorType : uint8_t
{
    CAMERA = 0,
    LIDAR,
    RADAR,
    GPS
};

enum class EventType : uint8_t
{
    NORMAL_DRIVING = 0,
    PEDESTRIAN_DETECTED,
    COLLISION_LIKE,
    RARE_OBJECT
};

struct Frame
{
    uint64_t timestamp_us;
    SensorType sensor;

    uint32_t width;
    uint32_t height;

    uint64_t data_size_bytes;

    uint8_t priority_score;
};

struct Event
{
    uint64_t timestamp_us;
    EventType type;

    float confidence;

    std::string metadata;
};

} // namespace offload
