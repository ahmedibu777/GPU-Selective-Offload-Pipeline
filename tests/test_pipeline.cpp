#include <cassert>
#include <cstdint>
#include <vector>

#include "offload/event_selector.hpp"
#include "offload/offload_worker.hpp"
#include "offload/priority_queue.hpp"

int main() {
    std::vector<std::uint8_t> payload(4096, 3);

    offload::EventSelector selector;
    selector.configure(
        offload::ValueWeights{0.3, 0.2, 0.4, 0.1},
        0.2,
        true);

    offload::PriorityQueue queue(10);
    offload::OffloadWorker worker(8.0);

    for (std::uint64_t i = 0; i < 10; ++i) {
        offload::Frame frame;
        frame.frame_id = i;
        frame.timestamp_s = static_cast<double>(i) * 0.1;
        frame.rarity = 0.9;
        frame.uncertainty = 0.8;
        frame.criticality = 0.7;
        frame.similarity_penalty = 0.1;
        frame.remaining_slack_s = 0.5;
        frame.data_ptr = payload.data();
        frame.data_size_bytes = payload.size();

        offload::Event event;
        assert(selector.select(frame, event));
        assert(queue.push(event));
    }

    offload::Event event;

    while (queue.pop(event)) {
        assert(worker.enqueue(event));
    }

    const offload::OffloadMetrics metrics = worker.drain(0.0);

    assert(metrics.events_selected == 10);
    assert(metrics.bytes_sent == 10 * payload.size());

    return 0;
}
