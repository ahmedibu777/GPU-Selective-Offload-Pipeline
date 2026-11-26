#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "offload/event_selector.hpp"
#include "offload/offload_worker.hpp"
#include "offload/priority_queue.hpp"

namespace {

offload::Frame make_frame(std::uint64_t id,
                          std::mt19937& generator,
                          std::size_t payload_bytes) {
    std::uniform_real_distribution<double> score(0.0, 1.0);
    std::uniform_int_distribution<int> type(0, 3);

    offload::Frame frame;
    frame.frame_id = id;
    frame.timestamp_s = static_cast<double>(id) * 0.1;

    frame.imu.acceleration_x = score(generator);
    frame.imu.acceleration_y = score(generator);
    frame.imu.acceleration_z = score(generator);
    frame.imu.angular_velocity_x = score(generator);
    frame.imu.angular_velocity_y = score(generator);
    frame.imu.angular_velocity_z = score(generator);

    frame.gps.latitude = 12.9 + score(generator) * 0.01;
    frame.gps.longitude = 77.5 + score(generator) * 0.01;
    frame.gps.altitude = 900.0 + score(generator) * 10.0;
    frame.gps.speed_mps = score(generator) * 25.0;

    frame.event_type = static_cast<offload::EventType>(type(generator));
    frame.priority = static_cast<std::uint8_t>(type(generator));
    frame.rarity = score(generator);
    frame.uncertainty = score(generator);
    frame.criticality = score(generator);
    frame.similarity_penalty = score(generator) * 0.5;
    frame.remaining_slack_s = 0.05 + score(generator) * 0.45;

    frame.data_size_bytes = payload_bytes;
    return frame;
}

bool is_supported_policy(const std::string& policy) {
    return policy == "upload-all" || policy == "fifo" ||
           policy == "random" || policy == "value-only" ||
           policy == "deadline-only" || policy == "combined";
}

}  // namespace

int main(int argc, char** argv) {
    int frames = 100;
    std::size_t budget_bytes = 256 * 1024;
    std::string policy = "combined";

    for (int i = 1; i < argc; ++i) {
        const std::string argument = argv[i];

        if (argument == "--frames" && i + 1 < argc) {
            frames = std::atoi(argv[++i]);
        } else if (argument == "--budget-bytes" && i + 1 < argc) {
            budget_bytes =
                static_cast<std::size_t>(std::atoll(argv[++i]));
        } else if (argument == "--policy" && i + 1 < argc) {
            policy = argv[++i];
        }
    }

    if (frames <= 0) {
        std::cerr << "Frame count must be positive.\n";
        return 1;
    }

    if (!is_supported_policy(policy)) {
        std::cerr << "Unsupported policy: " << policy << "\n";
        return 1;
    }

    std::mt19937 generator(42);
    std::vector<offload::Frame> input;
    input.reserve(static_cast<std::size_t>(frames));

    std::vector<std::uint8_t> payload(4096, 1);

    for (int i = 0; i < frames; ++i) {
        offload::Frame frame =
            make_frame(static_cast<std::uint64_t>(i), generator,
                       payload.size());
        frame.data_ptr = payload.data();
        input.push_back(frame);
    }

    offload::EventSelector selector;
    selector.configure(
        offload::ValueWeights{0.30, 0.25, 0.35, 0.10},
        0.20,
        true);

    offload::PriorityQueue queue(budget_bytes / payload.size() + 1);
    offload::OffloadWorker worker(8.0);

    std::uint64_t sent_bytes = 0;

    for (const offload::Frame& frame : input) {
        offload::Event event;
        bool selected = false;

        if (policy == "upload-all" || policy == "fifo") {
            selected = true;
            event.scheduling_priority =
                -static_cast<double>(frame.frame_id);
        } else if (policy == "random") {
            selected = (generator() % 2) == 0;
            event.scheduling_priority =
                static_cast<double>(generator() % 1000);
        } else if (policy == "value-only") {
            selected = selector.select(frame, event);
            event.scheduling_priority = event.value_score;
        } else if (policy == "deadline-only") {
            selected = frame.remaining_slack_s > 0.0;
            event.scheduling_priority =
                1.0 / std::max(frame.remaining_slack_s, 0.001);
        } else {
            selected = selector.select(frame, event);
        }

        if (!selected) {
            continue;
        }

        if (event.data_ptr == nullptr) {
            event.data_ptr = frame.data_ptr;
            event.data_size_bytes = frame.data_size_bytes;
            event.frame_id = frame.frame_id;
            event.created_at_s = frame.timestamp_s;
            event.deadline_s =
                frame.timestamp_s + frame.remaining_slack_s;
            event.priority = frame.priority;
        }

        if (sent_bytes + event.data_size_bytes <= budget_bytes) {
            if (queue.push(event)) {
                sent_bytes += event.data_size_bytes;
            }
        }
    }

    offload::Event event;

    while (queue.pop(event)) {
        worker.enqueue(event);
    }

    const offload::OffloadMetrics metrics = worker.drain(0.0);

    std::cout << "policy=" << policy
              << " frames=" << frames
              << " selected=" << metrics.events_selected
              << " bytes_generated=" << metrics.bytes_generated
              << " bytes_sent=" << metrics.bytes_sent
              << " deadline_misses=" << metrics.deadline_misses
              << "\n";

    return 0;
}
