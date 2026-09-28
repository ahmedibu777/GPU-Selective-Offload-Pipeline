# Common Data Model — Phase 1

This document defines the shared semantic contract for the first C++ data
model. Sensor and Recorder behavior are intentionally deferred.

## Timestamp

All Frame::timestamp and Event::timestamp values are std::uint64_t
microseconds since the Unix epoch (1970-01-01 00:00:00 UTC).

Components must not reinterpret the value as milliseconds, nanoseconds, or a
different epoch.

## Priority

priority is std::uint8_t in the inclusive range 0..255.

- 0 = lowest priority
- 255 = highest priority
- 128 = default priority

Future selector/scheduler logic must use larger values as higher priority.

## Confidence

confidence is std::uint8_t in the inclusive range 0..255.

- 0 = lowest confidence
- 255 = highest confidence
- 0 = Phase 1 default

## Frame memory ownership

Frame::data_ptr is a non-owning view of an externally managed float buffer.
The caller owns the allocation and lifetime. A Frame must not outlive the
referenced buffer.

No shared_ptr, vector, or GPU-specific ownership wrapper is introduced until
the memory strategy for CPU, pinned-host, and device memory is finalized.

## Event association and metadata

Event::associated_frame_id == 0 means that no particular frame is associated
with the event. Phase 1 does not assign an ID to Frame.

Event::metadata is a simple owning std::string; it is intentionally kept small
so later event-processing code can evolve without introducing a larger
metadata framework.

## Error semantics

ErrorCode::InvalidFrame, InvalidEvent, BufferUnavailable, and UnsupportedSensor
describe recoverable conditions.

ErrorCode::InternalError represents a fatal/invariant condition.

Exception classes and richer error-reporting infrastructure are deferred until
actual operations require them.
