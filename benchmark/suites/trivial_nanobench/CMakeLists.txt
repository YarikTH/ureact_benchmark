add_executable(ureact_benchmark_trivial_nanobench
               main.cpp
               ureact.cpp)

target_link_libraries(ureact_benchmark_trivial_nanobench PRIVATE nanobench::nanobench ureact_benchmark::utils)
target_link_libraries(ureact_benchmark_trivial_nanobench PRIVATE ureact::ureact)

target_compile_definitions(ureact_benchmark_trivial_nanobench PRIVATE SUITE_NAME=trivial)
