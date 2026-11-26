cmake_minimum_required(VERSION 3.20)

project(GPUSelectiveOffloadPipeline
    VERSION 1.0.0
    DESCRIPTION "CPU-first selective data offload pipeline for autonomous vehicles"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

option(OFFLOAD_ENABLE_SANITIZERS
    "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

if(OFFLOAD_ENABLE_SANITIZERS)
    if(MSVC)
        message(FATAL_ERROR
            "ASan/UBSan configuration is intended for GCC/Clang on Linux.")
    else()
        add_compile_options(
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
            -g
        )
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

include(GNUInstallDirs)
include(CTest)

add_library(offload_core
    src/event_selector.cpp
    src/priority_queue.cpp
    src/offload_worker.cpp
)

target_include_directories(offload_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(offload_core PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Wconversion -Wshadow>
)

add_executable(offload_pipeline src/main.cpp)
target_link_libraries(offload_pipeline PRIVATE offload_core)

if(BUILD_TESTING)
    enable_testing()

    add_executable(test_event_selector tests/test_event_selector.cpp)
    target_link_libraries(test_event_selector PRIVATE offload_core)
    add_test(NAME event_selector COMMAND test_event_selector)

    add_executable(test_priority_queue tests/test_priority_queue.cpp)
    target_link_libraries(test_priority_queue PRIVATE offload_core)
    add_test(NAME priority_queue COMMAND test_priority_queue)

    add_executable(test_offload_worker tests/test_offload_worker.cpp)
    target_link_libraries(test_offload_worker PRIVATE offload_core)
    add_test(NAME offload_worker COMMAND test_offload_worker)

    add_executable(test_pipeline tests/test_pipeline.cpp)
    target_link_libraries(test_pipeline PRIVATE offload_core)
    add_test(NAME pipeline COMMAND test_pipeline)
endif()
