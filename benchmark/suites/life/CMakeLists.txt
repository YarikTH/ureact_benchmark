add_executable(ureact_benchmark_life
               main.cpp
               ureact.cpp
               cpp_react_legacy1.cpp
               cpp_react_master.cpp
               # urp.cpp
               )

target_link_libraries(ureact_benchmark_life PRIVATE benchmark::benchmark ureact_benchmark::utils)
target_link_libraries(ureact_benchmark_life PRIVATE
                      ureact::ureact
                      CppReact::legacy1
                      CppReact::master
                      #urp::urp
                      )

target_compile_definitions(ureact_benchmark_life PRIVATE SUITE_NAME=life)
