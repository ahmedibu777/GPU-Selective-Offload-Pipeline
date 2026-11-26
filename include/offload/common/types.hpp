#pragma once

#include <cstdint>
#include <string>

namespace offload {

enum class EventType : std::uint8_t {
    Unknown = 0,
    Normal = 1,
    Anomaly = 2,
    SafetyCritical = 3
};

struct Imu {
    double acceleration_x{};
    double acceleration_y{};
    double acceleration_z{};
    double angular_velocity_x{};
    double angular_velocity_y{};
    double angular_velocity_z{};
};

struct Gps {
    double latitude{};
    double longitude{};
    double altitude{};
    double speed_mps{};
};

struct ValueWeights {
    double rarity{};
    double uncertainty{};
    double criticality{};
    double similarity{};
};

struct LinkProfile {
    std::string name;
    double bandwidth_mbps{};
    double latency_ms{};
    double packet_loss_percent{};
};

struct OffloadMetrics {
    std::uint64_t frames_seen{};
    std::uint64_t events_selected{};
    std::uint64_t events_dropped{};
    std::uint64_t bytes_generated{};
    std::uint64_t bytes_sent{};
    std::uint64_t deadline_misses{};
    double latency_p50_ms{};
    double latency_p95_ms{};
};

}  // namespace offload
