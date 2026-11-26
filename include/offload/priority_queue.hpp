#pragma once

#include <cstddef>
#include <cstdint>
#include <queue>
#include <vector>

#include "offload/common/frame.hpp"

namespace offload {

class PriorityQueue {
public:
    explicit PriorityQueue(std::size_t capacity);

    bool push(const Event& event);
    bool pop(Event& event);
    bool empty() const;
    std::size_t size() const;
    std::size_t capacity() const;
    std::uint64_t dropped_count() const;

private:
    struct Entry {
        double scheduling_priority{};
        std::uint64_t sequence{};
        Event event{};
    };

    struct Comparator {
        bool operator()(const Entry& lhs, const Entry& rhs) const;
    };

    std::priority_queue<Entry, std::vector<Entry>, Comparator> queue_;
    std::size_t capacity_;
    std::uint64_t sequence_{};
    std::uint64_t dropped_count_{};
};

}  // namespace offload
