# GPU Selective Offload Pipeline — Phase 1

Phase 1 establishes a hardware-independent C++17 common sensor/event model and a small recorder simulator with smoke tests. CUDA, TensorRT, networking, and storage integrations are deferred.

## Common model
- Sensor types: Camera, Lidar, Radar, Imu, Gps.
- Event types: FrameReady, ErrorOccurred, ProcessingComplete, NormalDriving, PedestrianDetected, CollisionLike, RareObject.
- Timestamp: uint64 microseconds.
- Frame payload: non-owning const byte pointer, size, and three dimensions.
- Priority: uint8; 0 lowest/drop, 255 highest/immediate; default 128.

## Layout
- include/offload/common: shared enum/event definitions
- include/offload/recorder: frame representation
- apps/recorder_simulator: Phase 1 executable
- tests: common-model smoke test
- docs/architecture: common-model documentation

## Build and verify (GNU/Clang, Ninja)
```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DENABLE_SANITIZERS=ON
cmake --build build
ctest --test-dir build --output-on-failure
./build/recorder_simulator
```
Sanitizers are enabled by `ENABLE_SANITIZERS=ON`; compiler warnings are treated as errors.
