add_executable(common_types_smoke
    common_types_smoke.cpp
)

target_include_directories(common_types_smoke PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(common_types_smoke PRIVATE cxx_std_17)

target_compile_options(common_types_smoke PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang,AppleClang>:-Wall;-Wextra;-Wpedantic>
)

add_test(NAME common_types_smoke COMMAND common_types_smoke)
