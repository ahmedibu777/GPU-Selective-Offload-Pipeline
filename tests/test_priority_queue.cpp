#include <cassert>

#include "offload/priority_queue.hpp"

int main() {
    offload::PriorityQueue queue(2);

    offload::Event low;
    low.scheduling_priority = 1.0;
    low.data_size_bytes = 10;

    offload::Event high;
    high.scheduling_priority = 10.0;
    high.data_size_bytes = 20;

    assert(queue.push(low));
    assert(queue.push(high));
    assert(!queue.push(low));
    assert(queue.dropped_count() == 1);

    offload::Event popped;
    assert(queue.pop(popped));
    assert(popped.scheduling_priority == 10.0);

    assert(queue.pop(popped));
    assert(popped.scheduling_priority == 1.0);

    assert(!queue.pop(popped));

    return 0;
}
