#include <array>
#include <cassert>
#include <cstdint>
#include <string>

#include "offload/common/constants.hpp"
#include "offload/common/errors.hpp"
#include "offload/common/types.hpp"

int main() {
    using namespace offload;
    using namespace offload::constants;

    static_assert(static_cast<std::uint8_t>(SensorType::Camera) == 0);
    static_assert(static_cast<std::uint8_t>(SensorType::Lidar) == 1);
    static_assert(static_cast<std::uint8_t>(SensorType::Radar) == 2);
    static_assert(static_cast<std::uint8_t>(SensorType::Imu) == 3);

    static_assert(static_cast<std::uint8_t>(EventType::FrameReady) == 0);
    static_assert(static_cast<std::uint8_t>(EventType::ErrorOccurred) == 1);
    static_assert(static_cast<std::uint8_t>(EventType::ProcessingComplete) == 2);

    Frame defaults;
    assert(defaults.timestamp == 0);
    assert(defaults.sensor_type == SensorType::Camera);
    assert(defaults.width == 0);
    assert(defaults.height == 0);
    assert(defaults.data_size == 0);
    assert(defaults.data_ptr == nullptr);
    assert(defaults.priority == kDefaultPriority);
    assert(defaults.confidence == kDefaultConfidence);

    std::array<float, 4> buffer{1.0F, 2.0F, 3.0F, 4.0F};

    Frame frame;
    frame.timestamp = 123456789ULL;
    frame.sensor_type = SensorType::Camera;
    frame.width = 2;
    frame.height = 2;
    frame.data_size = buffer.size() * sizeof(float);
    frame.data_ptr = buffer.data();
    frame.priority = kMaxPriority;
    frame.confidence = kMaxConfidence;

    assert(frame.timestamp == 123456789ULL);
    assert(frame.data_ptr == buffer.data());
    assert(frame.data_size == sizeof(buffer));
    assert(frame.priority == 255);
    assert(frame.confidence == 255);
    assert(frame.data_ptr[0] == 1.0F);
    assert(frame.data_ptr[3] == 4.0F);

    Event event;
    event.timestamp = frame.timestamp;
    event.event_type = EventType::FrameReady;
    event.associated_frame_id = 0;
    event.priority = frame.priority;
    event.confidence = frame.confidence;
    event.metadata = "smoke-test";

    assert(event.timestamp == frame.timestamp);
    assert(event.event_type == EventType::FrameReady);
    assert(event.associated_frame_id == 0);
    assert(event.priority == kMaxPriority);
    assert(event.confidence == kMaxConfidence);
    assert(event.metadata == std::string("smoke-test"));

    assert(!is_fatal(ErrorCode::None));
    assert(!is_fatal(ErrorCode::InvalidFrame));
    assert(is_fatal(ErrorCode::InternalError));

    return 0;
}
