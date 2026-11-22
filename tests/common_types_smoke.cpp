#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string>

#include "offload/common/types.hpp"
#include "offload/recorder/frame.hpp"

namespace {
bool check(bool condition, const char* label) {
    if (!condition) std::cerr << "FAIL: " << label << '\n';
    return condition;
}
}

int main() {
    using offload::common::Event;
    using offload::common::EventType;
    using offload::common::SensorType;
    using offload::recorder::Frame;

    static_assert(static_cast<std::uint8_t>(SensorType::Camera) == 0U);
    static_assert(static_cast<std::uint8_t>(SensorType::Gps) == 4U);
    static_assert(static_cast<std::uint8_t>(EventType::RareObject) == 6U);
    bool ok = true;

    Frame defaults;
    ok &= check(defaults.timestamp_us == 0U, "timestamp default");
    ok &= check(defaults.sensor == SensorType::Camera, "sensor default");
    ok &= check(defaults.dimensions == std::array<std::uint32_t, 3>{0U,0U,0U}, "dimension defaults");
    ok &= check(defaults.data_ptr == nullptr, "pointer default");
    ok &= check(defaults.data_size == 0U, "size default");
    ok &= check(defaults.priority == 128U, "priority default");

    std::array<std::uint8_t, 12> payload{0,1,2,3,4,5,6,7,8,9,10,11};
    Frame frame;
    frame.timestamp_us = 123456789U;
    frame.sensor = SensorType::Lidar;
    frame.dimensions = {2U, 2U, 3U};
    frame.data_ptr = payload.data();
    frame.data_size = payload.size();
    frame.priority = 255U;
    ok &= check(frame.data_ptr == payload.data(), "non-owning pointer assignment");
    ok &= check(frame.data_size == payload.size(), "payload size");
    ok &= check(frame.dimensions[2] == 3U, "third dimension");
    ok &= check(frame.priority == 255U, "maximum priority");

    Event event;
    event.timestamp_us = frame.timestamp_us;
    event.type = EventType::PedestrianDetected;
    event.confidence = 0.75F;
    event.priority = frame.priority;
    event.metadata = "smoke-test";
    ok &= check(event.timestamp_us == frame.timestamp_us, "event timestamp");
    ok &= check(event.type == EventType::PedestrianDetected, "event type");
    ok &= check(event.confidence > 0.7F, "confidence");
    ok &= check(event.priority == 255U, "event priority");
    ok &= check(event.metadata == std::string("smoke-test"), "metadata");

    if (!ok) return 1;
    std::cout << "Common type smoke test passed.\n";
    return 0;
}
