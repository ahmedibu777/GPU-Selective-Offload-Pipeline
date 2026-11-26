#include <cassert>
#include <cstdint>
#include <vector>

#include "offload/offload_worker.hpp"

int main() {
    std::vector<std::uint8_t> payload(200 * 1024, 9);

    offload::Event event;
    event.data_ptr = payload.data();
    event.data_size_bytes = payload.size();
    event.created_at_s = 0.0;
    event.deadline_s = 10.0;
    event.scheduling_priority = 100.0;

    offload::OffloadWorker worker(8.0);
    assert(worker.enqueue(event));

    const offload::OffloadMetrics metrics = worker.drain(0.0);

    assert(metrics.bytes_sent == payload.size());
    assert(metrics.deadline_misses == 0);

    return 0;
}
