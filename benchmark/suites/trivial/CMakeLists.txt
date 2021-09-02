add_executable(ureact_benchmark_trivial
               main.cpp
               ureact.cpp
               cpp_react_legacy1.cpp)

target_link_libraries(ureact_benchmark_trivial PRIVATE benchmark::benchmark ureact_benchmark::utils)
target_link_libraries(ureact_benchmark_trivial PRIVATE ureact::ureact CppReact::legacy1)

target_compile_definitions(ureact_benchmark_trivial PRIVATE SUITE_NAME=trivial)
