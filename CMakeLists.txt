cmake_minimum_required(VERSION 3.20)

project(GPUSelectiveOffloadPipeline
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(recorder_simulator
    apps/recorder_simulator/main.cpp
)

target_include_directories(recorder_simulator PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(recorder_simulator PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall;-Wextra;-Wpedantic>
)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
