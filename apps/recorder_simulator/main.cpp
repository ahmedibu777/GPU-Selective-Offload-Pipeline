#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>

#include "offload/common/types.hpp"
#include "offload/recorder/frame.hpp"

int main() {
    using offload::common::Event;
    using offload::common::EventType;
    using offload::common::SensorType;
    using offload::recorder::Frame;

    std::array<std::uint8_t, 12> payload{};
    Frame frame;
    frame.timestamp_us = 123456789U;
    frame.sensor = SensorType::Camera;
    frame.dimensions = {2U, 2U, 3U};
    frame.data_ptr = payload.data();
    frame.data_size = payload.size();
    frame.priority = 200;

    Event event;
    event.timestamp_us = frame.timestamp_us;
    event.type = EventType::FrameReady;
    event.confidence = 1.0F;
    event.priority = frame.priority;
    event.metadata = "recorder_simulator";

    if (frame.data_ptr == nullptr || frame.data_size != payload.size() ||
        frame.dimensions[0] != 2U || event.type != EventType::FrameReady) {
        std::cerr << "Phase 1 smoke validation failed\n";
        return 1;
    }
    std::cout << "GPU Selective Offload Pipeline - Phase 1 simulator\n";
    std::cout << "Frame timestamp_us: " << frame.timestamp_us << "\n";
    std::cout << "Payload bytes: " << frame.data_size << "\n";
    std::cout << "Priority: " << static_cast<unsigned int>(frame.priority) << "\n";
    return 0;
}
