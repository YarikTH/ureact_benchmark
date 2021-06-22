function(add_react_test name library)
    add_executable(${name} ${name}.cpp)
    
    target_link_libraries(${name} PRIVATE doctest::doctest ${library})
    
    add_test(NAME ${name} COMMAND ${name})
endfunction()

add_react_test(test_ureact ureact::ureact)
add_react_test(test_cpp_react_legacy1 CppReact::legacy1)
add_react_test(test_cpp_react_master CppReact::master)
add_react_test(test_urp urp::urp)
add_react_test(test_observable observable::observable)
add_react_test(test_cpp-react cpp::react)
add_react_test(test_reactive reactive::reactive)
