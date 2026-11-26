#include "offload/offload_worker.hpp"

#include <algorithm>
#include <cmath>

namespace offload {

OffloadWorker::OffloadWorker(double simulated_link_mbps)
    : simulated_link_mbps_{simulated_link_mbps} {
    if (simulated_link_mbps_ <= 0.0) {
        simulated_link_mbps_ = 1.0;
    }
}

double OffloadWorker::transmission_time_s(std::size_t bytes) const {
    const double bits = static_cast<double>(bytes) * 8.0;
    const double bits_per_second = simulated_link_mbps_ * 1'000'000.0;
    return bits / bits_per_second;
}

void OffloadWorker::emit_chunk(const PendingEvent& pending,
                               std::size_t chunk_bytes,
                               double current_time_s) {
    metrics_.bytes_sent += chunk_bytes;
    metrics_.frames_seen += 1;

    const double completion_time =
        current_time_s + transmission_time_s(chunk_bytes);

    if (completion_time > pending.event.deadline_s) {
        ++metrics_.deadline_misses;
    }
}

bool OffloadWorker::enqueue(const Event& event) {
    if (event.data_ptr == nullptr || event.data_size_bytes == 0) {
        return false;
    }

    PendingEvent pending;
    pending.event = event;
    pending.ready_time_s = event.created_at_s;

    pending_.push_back(pending);
    metrics_.bytes_generated += event.data_size_bytes;
    ++metrics_.events_selected;

    return true;
}

OffloadMetrics OffloadWorker::drain(double current_time_s) {
    std::sort(pending_.begin(), pending_.end(),
              [](const PendingEvent& lhs, const PendingEvent& rhs) {
                  if (lhs.event.scheduling_priority !=
                      rhs.event.scheduling_priority) {
                      return lhs.event.scheduling_priority >
                             rhs.event.scheduling_priority;
                  }
                  return lhs.event.created_at_s < rhs.event.created_at_s;
              });

    double transmit_time = current_time_s;

    for (const PendingEvent& pending : pending_) {
        std::size_t remaining = pending.event.data_size_bytes;

        while (remaining > 0) {
            const std::size_t chunk_bytes =
                std::min(remaining, kMaxChunkBytes);

            emit_chunk(pending, chunk_bytes, transmit_time);
            transmit_time += transmission_time_s(chunk_bytes);
            remaining -= chunk_bytes;
        }
    }

    pending_.clear();
    return metrics_;
}

const OffloadMetrics& OffloadWorker::metrics() const {
    return metrics_;
}

}  // namespace offload
