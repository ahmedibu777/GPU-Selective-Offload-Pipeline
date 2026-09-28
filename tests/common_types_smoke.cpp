#include <array>
#include <cstdint>
#include <iostream>
#include <string>

#include "offload/common/constants.hpp"
#include "offload/common/errors.hpp"
#include "offload/common/types.hpp"

namespace {

bool check(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << '\n';
    }
    return condition;
}

}  // namespace

int main() {
    using namespace offload;
    using namespace offload::constants;

    bool ok = true;

    static_assert(static_cast<std::uint8_t>(SensorType::Camera) == 0);
    static_assert(static_cast<std::uint8_t>(SensorType::Lidar) == 1);
    static_assert(static_cast<std::uint8_t>(SensorType::Radar) == 2);
    static_assert(static_cast<std::uint8_t>(SensorType::Imu) == 3);

    static_assert(static_cast<std::uint8_t>(EventType::FrameReady) == 0);
    static_assert(static_cast<std::uint8_t>(EventType::ErrorOccurred) == 1);
    static_assert(static_cast<std::uint8_t>(EventType::ProcessingComplete) == 2);

    Frame defaults;
    ok &= check(defaults.timestamp == 0, "Frame timestamp default");
    ok &= check(defaults.sensor_type == SensorType::Camera, "Frame sensor default");
    ok &= check(defaults.width == 0, "Frame width default");
    ok &= check(defaults.height == 0, "Frame height default");
    ok &= check(defaults.data_size == 0, "Frame data_size default");
    ok &= check(defaults.data_ptr == nullptr, "Frame data_ptr default");
    ok &= check(defaults.priority == kDefaultPriority, "Frame priority default");
    ok &= check(defaults.confidence == kDefaultConfidence, "Frame confidence default");

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

    ok &= check(frame.timestamp == 123456789ULL, "Frame timestamp assignment");
    ok &= check(frame.data_ptr == buffer.data(), "Frame non-owning pointer");
    ok &= check(frame.data_size == sizeof(buffer), "Frame data size");
    ok &= check(frame.priority == 255, "255 is highest priority");
    ok &= check(frame.confidence == 255, "255 is highest confidence");
    ok &= check(frame.data_ptr[0] == 1.0F, "Frame first element");
    ok &= check(frame.data_ptr[3] == 4.0F, "Frame last element");

    Event event;
    event.timestamp = frame.timestamp;
    event.event_type = EventType::FrameReady;
    event.associated_frame_id = 0;
    event.priority = frame.priority;
    event.confidence = frame.confidence;
    event.metadata = "smoke-test";

    ok &= check(event.timestamp == frame.timestamp, "Event timestamp");
    ok &= check(event.event_type == EventType::FrameReady, "Event type");
    ok &= check(event.associated_frame_id == 0, "Event frame association default");
    ok &= check(event.priority == kMaxPriority, "Event priority");
    ok &= check(event.confidence == kMaxConfidence, "Event confidence");
    ok &= check(event.metadata == std::string("smoke-test"), "Event metadata");

    ok &= check(!is_fatal(ErrorCode::None), "None is recoverable");
    ok &= check(!is_fatal(ErrorCode::InvalidFrame), "InvalidFrame is recoverable");
    ok &= check(is_fatal(ErrorCode::InternalError), "InternalError is fatal");

    if (!ok) {
        return 1;
    }

    std::cout << "Common type smoke test passed.\n";
    return 0;
}
