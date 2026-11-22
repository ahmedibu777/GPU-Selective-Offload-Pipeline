# Common Data Model — Phase 1

Hardware-independent C++17 schema contract for frames and events.

## Timestamp
Frame and Event timestamps are uint64 microseconds since Unix epoch (UTC).

## Frame
Frame uses a sensor enum, three uint32 dimensions, byte payload pointer and byte size. The payload pointer is non-owning: the caller owns storage and must ensure the buffer outlives the Frame. No GPU-specific ownership is assumed.

## Sensors
Camera, Lidar, Radar, Imu, Gps.

## Events
FrameReady, ErrorOccurred, ProcessingComplete, NormalDriving, PedestrianDetected, CollisionLike, RareObject.

Event confidence is a float; priority is uint8 (0 lowest/drop, 255 highest/immediate, default 128). Metadata is an owning string.

## Scope
No CUDA, TensorRT, networking, database, or GPU dependencies are required for this Phase 1 model.
