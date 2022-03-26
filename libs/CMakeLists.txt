set(BENCHMARK_ENABLE_TESTING OFF
    CACHE BOOL "Enable testing of the benchmark library."
    FORCE
)
add_subdirectory(benchmark)


add_subdirectory(nanobench)

# doctest
add_subdirectory(doctest)

# ureact
add_subdirectory(ureact)

# urp
add_library(urp INTERFACE)
target_include_directories(
    urp
    INTERFACE ${CMAKE_CURRENT_LIST_DIR}/usingstdcpp2019
)
target_compile_features(urp INTERFACE cxx_std_17)
add_library(urp::urp ALIAS urp)

# tbb needed for cpp.react
set(TBB_BUILD_SHARED n CACHE BOOL "Build TBB shared library" FORCE)
set(TBB_BUILD_STATIC y CACHE BOOL "Build TBB static library" FORCE)
set(TBB_BUILD_TBBMALLOC n CACHE BOOL "Build TBB malloc library" FORCE)
set(TBB_BUILD_TBBMALLOC_PROXY n
    CACHE BOOL "Build TBB malloc proxy library"
    FORCE
)
set(TBB_BUILD_TESTS n
    CACHE BOOL "Build TBB tests and enable testing infrastructure"
    FORCE
)
set(TBB_NO_DATE y
    CACHE BOOL "Do not save the configure date in the version string"
    FORCE
)
set(TBB_BUILD_PYTHON n CACHE BOOL "Build TBB Python bindings" FORCE)
set(TBB_SET_SOVERSION n
    CACHE BOOL "Set the SOVERSION (shared library build version suffix)?"
    FORCE
)
set(TBB_INSTALL_TARGETS n
    CACHE BOOL "Include build targets for 'make install'"
    FORCE
)

add_subdirectory(tbb)

# CppReact.Master
add_library(CppReactMaster cpp.react_master/src/detail/graph_impl.cpp)

target_include_directories(CppReactMaster PUBLIC cpp.react_master/include)

target_compile_features(CppReactMaster PUBLIC cxx_std_17)

target_link_libraries(CppReactMaster PUBLIC tbb_static)

add_library(CppReact::master ALIAS CppReactMaster)

# CppReact.Legacy1
add_library(
    CppReactLegacy1
    cpp.react_legacy1/src/engine/PulsecountEngine.cpp
    cpp.react_legacy1/src/engine/SubtreeEngine.cpp
    cpp.react_legacy1/src/engine/ToposortEngine.cpp
    cpp.react_legacy1/src/logging/EventLog.cpp
    cpp.react_legacy1/src/logging/EventRecords.cpp
)

target_include_directories(CppReactLegacy1 PUBLIC cpp.react_legacy1/include)

target_compile_features(CppReactLegacy1 PUBLIC cxx_std_11)

target_link_libraries(CppReactLegacy1 PUBLIC tbb_static)

add_library(CppReact::legacy1 ALIAS CppReactLegacy1)

# observable
add_library(observable INTERFACE)
target_include_directories(
    observable
    INTERFACE ${CMAKE_CURRENT_LIST_DIR}/observable/observable/include
)
target_compile_features(observable INTERFACE cxx_std_14)
add_library(observable::observable ALIAS observable)

# cpp-react
add_library(cpp-react INTERFACE)
target_include_directories(
    cpp-react
    INTERFACE ${CMAKE_CURRENT_LIST_DIR}/cpp-react
)
target_compile_features(cpp-react INTERFACE cxx_std_14)
add_library(cpp::react ALIAS cpp-react)

# reactive
add_library(reactive INTERFACE)
target_include_directories(
    reactive
    INTERFACE ${CMAKE_CURRENT_LIST_DIR}/reactive/src
)
target_compile_features(reactive INTERFACE cxx_std_17)
add_library(reactive::reactive ALIAS reactive)

# react
add_library(react INTERFACE)
target_include_directories(
    react
    INTERFACE ${CMAKE_CURRENT_LIST_DIR}/react/include
)
target_compile_features(react INTERFACE cxx_std_11)
add_library(react::react ALIAS react)

# RxCpp
add_library(RxCpp INTERFACE)
target_include_directories(
    RxCpp
    INTERFACE ${CMAKE_CURRENT_LIST_DIR}/RxCpp/Rx/v2/src
)
target_compile_features(RxCpp INTERFACE cxx_std_11)
add_library(RxCpp::RxCpp ALIAS RxCpp)

# tensorflow
#
