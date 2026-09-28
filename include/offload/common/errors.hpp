#pragma once

#include <cstdint>

namespace offload {

/// Error categories shared by the common data model.
///
/// InvalidFrame, InvalidEvent, BufferUnavailable, and UnsupportedSensor are
/// recoverable conditions. InternalError represents a fatal/invariant failure.
/// Exception classes are intentionally deferred to a later phase.
enum class ErrorCode : std::uint8_t {
    None = 0,
    InvalidFrame = 1,
    InvalidEvent = 2,
    BufferUnavailable = 3,
    UnsupportedSensor = 4,
    InternalError = 255
};

[[nodiscard]] constexpr bool is_fatal(ErrorCode code) noexcept {
    return code == ErrorCode::InternalError;
}

}  // namespace offload
