#pragma once

#include <cstdint>

namespace offload::constants {

/// Timestamp convention for all pipeline timestamps:
/// microseconds since the Unix epoch (1970-01-01 00:00:00 UTC).
inline constexpr std::uint64_t kTimestampUnitMicroseconds = 1;

/// Priority scale used by future selectors/schedulers:
/// 0 is the lowest priority and 255 is the highest priority.
inline constexpr std::uint8_t kMinPriority = 0;
inline constexpr std::uint8_t kMaxPriority = 255;
inline constexpr std::uint8_t kDefaultPriority = 128;

/// Confidence scale:
/// 0 is the lowest confidence and 255 is the highest confidence.
inline constexpr std::uint8_t kMinConfidence = 0;
inline constexpr std::uint8_t kMaxConfidence = 255;
inline constexpr std::uint8_t kDefaultConfidence = 0;

/// Common-data-model API version for Phase 1.
inline constexpr std::uint8_t kApiVersionMajor = 0;
inline constexpr std::uint8_t kApiVersionMinor = 1;

// No maximum frame size or GPU alignment requirement is fixed in Phase 1.
// Those details are deferred until the GPU memory strategy is finalized.

}  // namespace offload::constants
