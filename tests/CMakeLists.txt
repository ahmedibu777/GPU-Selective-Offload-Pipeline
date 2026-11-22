add_executable(common_types_smoke common_types_smoke.cpp)
target_include_directories(common_types_smoke PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(common_types_smoke PRIVATE -Wall -Wextra -Wpedantic -Werror)
add_test(NAME common_types_smoke COMMAND common_types_smoke)
