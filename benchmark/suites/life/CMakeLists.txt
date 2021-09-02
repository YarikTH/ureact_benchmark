add_executable(ureact_benchmark_life
               main.cpp
               baseline.cpp
               ureact.cpp
               ureact_std_algo.cpp
               cpp_react_legacy1.cpp
               cpp_react_master.cpp
               reactive.cpp
               observable.cpp
               urp.cpp
               )

target_link_libraries(ureact_benchmark_life PRIVATE benchmark::benchmark ureact_benchmark::utils)
target_link_libraries(ureact_benchmark_life PRIVATE
                      ureact::ureact
                      CppReact::legacy1
                      CppReact::master
                      reactive::reactive
                      observable::observable
                      urp::urp
                      )

target_compile_definitions(ureact_benchmark_life PRIVATE SUITE_NAME=life)
