#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "offload/common/frame.hpp"
#include "offload/common/types.hpp"

namespace offload {

class OffloadWorker {
public:
    static constexpr std::size_t kMaxChunkBytes = 64 * 1024;

    explicit OffloadWorker(double simulated_link_mbps);

    bool enqueue(const Event& event);
    OffloadMetrics drain(double current_time_s);

    const OffloadMetrics& metrics() const;

private:
    struct PendingEvent {
        Event event{};
        double ready_time_s{};
    };

    double transmission_time_s(std::size_t bytes) const;
    void emit_chunk(const PendingEvent& pending,
                    std::size_t chunk_bytes,
                    double current_time_s);

    double simulated_link_mbps_;
    std::vector<PendingEvent> pending_;
    OffloadMetrics metrics_{};
};

}  // namespace offload
