#include "offload/priority_queue.hpp"

namespace offload {

PriorityQueue::PriorityQueue(std::size_t capacity)
    : capacity_{capacity} {}

bool PriorityQueue::Comparator::operator()(const Entry& lhs,
                                           const Entry& rhs) const {
    if (lhs.scheduling_priority != rhs.scheduling_priority) {
        return lhs.scheduling_priority < rhs.scheduling_priority;
    }
    return lhs.sequence > rhs.sequence;
}

bool PriorityQueue::push(const Event& event) {
    if (queue_.size() >= capacity_) {
        ++dropped_count_;
        return false;
    }

    queue_.push(Entry{event.scheduling_priority, sequence_++, event});
    return true;
}

bool PriorityQueue::pop(Event& event) {
    if (queue_.empty()) {
        return false;
    }

    event = queue_.top().event;
    queue_.pop();
    return true;
}

bool PriorityQueue::empty() const {
    return queue_.empty();
}

std::size_t PriorityQueue::size() const {
    return queue_.size();
}

std::size_t PriorityQueue::capacity() const {
    return capacity_;
}

std::uint64_t PriorityQueue::dropped_count() const {
    return dropped_count_;
}

}  // namespace offload
