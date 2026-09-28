#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>

#include "offload/common/constants.hpp"
#include "offload/common/types.hpp"

int main() {
    using namespace offload;
    using namespace offload::constants;

    constexpr std::uint32_t kWidth = 100;
    constexpr std::uint32_t kHeight = 100;
    constexpr std::size_t kElementCount =
        static_cast<std::size_t>(kWidth) * static_cast<std::size_t>(kHeight);

    std::cout << "GPU Selective Offload Pipeline - Recorder Simulator v0.1\n";

    // Frame::data_ptr is a non-owning view. The simulator owns this buffer
    // and keeps it alive for the complete lifetime of frame.
    std::array<float, kElementCount> test_buffer{};

    for (std::size_t i = 0; i < test_buffer.size(); ++i) {
        test_buffer[i] =
            static_cast<float>(i) / static_cast<float>(test_buffer.size());
    }

    const auto now = std::chrono::system_clock::now();
    const auto duration = now.time_since_epoch();
    const std::uint64_t timestamp_us =
        static_cast<std::uint64_t>(
            std::chrono::duration_cast<std::chrono::microseconds>(duration).count());

    Frame frame;
    frame.timestamp = timestamp_us;
    frame.sensor_type = SensorType::Camera;
    frame.width = kWidth;
    frame.height = kHeight;
    frame.data_size = test_buffer.size() * sizeof(float);
    frame.data_ptr = test_buffer.data();
    frame.priority = 200;
    frame.confidence = 250;

    const bool frame_valid =
        frame.timestamp > 0 &&
        frame.width == kWidth &&
        frame.height == kHeight &&
        frame.data_size == test_buffer.size() * sizeof(float) &&
        frame.data_ptr != nullptr &&
        frame.priority >= kMinPriority &&
        frame.priority <= kMaxPriority &&
        frame.confidence >= kMinConfidence &&
        frame.confidence <= kMaxConfidence;

    if (!frame_valid) {
        std::cerr << "ERROR: minimal Frame validation failed\n";
        return 1;
    }

    std::cout
        << "\nFrame constructed successfully:\n"
        << "  - Timestamp: " << frame.timestamp << " us\n"
        << "  - Sensor: " << static_cast<int>(frame.sensor_type)
        << " (Camera)\n"
        << "  - Dimensions: " << frame.width << "x" << frame.height << "\n"
        << "  - Data size: " << frame.data_size << " bytes\n"
        << "  - Priority: " << static_cast<int>(frame.priority) << "\n"
        << "  - Confidence: " << static_cast<int>(frame.confidence) << "\n"
        << "  - Data[0]: " << frame.data_ptr[0] << "\n";

    Event event;
    event.timestamp = timestamp_us;
    event.event_type = EventType::FrameReady;
    event.associated_frame_id = 0;
    event.priority = frame.priority;
    event.confidence = frame.confidence;
    event.metadata = "recorder_simulator";

    const bool event_valid =
        event.timestamp == frame.timestamp &&
        event.event_type == EventType::FrameReady &&
        !event.metadata.empty();

    if (!event_valid) {
        std::cerr << "ERROR: minimal Event validation failed\n";
        return 1;
    }

    std::cout
        << "\nEvent constructed successfully:\n"
        << "  - Event type: "
        << static_cast<int>(event.event_type)
        << " (FrameReady)\n"
        << "  - Metadata: " << event.metadata << "\n";

    std::cout << "\nRecorder simulator completed successfully.\n";
    return 0;
}
