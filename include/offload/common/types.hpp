#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace offload {

/// Identifies the source sensor for a Frame.
enum class SensorType : std::uint8_t {
    Camera = 0,
    Lidar = 1,
    Radar = 2,
    Imu = 3
};

/// Categorizes events emitted by the pipeline.
enum class EventType : std::uint8_t {
    FrameReady = 0,
    ErrorOccurred = 1,
    ProcessingComplete = 2
};

/// Represents a single frame of sensor data.
///
/// Timestamp convention:
/// - timestamp is microseconds since the Unix epoch (1970-01-01 UTC).
/// - Every component must interpret timestamps using this same convention.
///
/// Memory ownership:
/// - data_ptr is a non-owning view of an externally managed float buffer.
/// - The caller owns the allocation and lifetime of that buffer.
/// - A Frame must not outlive the buffer referenced by data_ptr.
/// - The representation intentionally remains a raw pointer so future CPU,
///   pinned-host, or device-memory strategies are not prescribed prematurely.
struct Frame {
    std::uint64_t timestamp = 0;
    SensorType sensor_type = SensorType::Camera;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::size_t data_size = 0;
    float* data_ptr = nullptr;
    std::uint8_t priority = 128;
    std::uint8_t confidence = 0;
};

/// Represents auxiliary event metadata.
///
/// associated_frame_id == 0 means that the event is not associated with a
/// specific frame. Phase 1 does not assign an ID to Frame itself.
///
/// Event metadata is deliberately simple and owning; it does not introduce
/// another external lifetime dependency.
struct Event {
    std::uint64_t timestamp = 0;
    EventType event_type = EventType::FrameReady;
    std::uint64_t associated_frame_id = 0;
    std::uint8_t priority = 128;
    std::uint8_t confidence = 0;
    std::string metadata;
};

}  // namespace offload
