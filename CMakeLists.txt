cmake_minimum_required(VERSION 3.15...3.28)

project(rk_automation)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/CompileFlags.cmake)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


add_library(subproc_stream_cpp STATIC ${CMAKE_CURRENT_SOURCE_DIR}/include/subproc_stream.hpp)
set_target_properties(subproc_stream_cpp PROPERTIES LINKER_LANGUAGE CXX)

add_executable(example ${CMAKE_CURRENT_LIST_DIR}/examples/example.cpp)
target_include_directories(example PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(example PUBLIC subproc_stream_cpp)

set_common_options_on_targets()

# if (SKBUILD)
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

  find_package(Python
    REQUIRED COMPONENTS Interpreter Development.Module
    OPTIONAL_COMPONENTS Development.SABIModule
  )
	
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/nanobind)
  
  find_package(nanobind CONFIG REQUIRED)
  nanobind_add_module(
    subproc_stream_ext 
    STABLE_ABI
    NB_STATIC
    ${CMAKE_CURRENT_LIST_DIR}/src/subproc_stream_ext.cpp
  ) 
  target_link_libraries(subproc_stream_ext PRIVATE subproc_stream_cpp)
  target_link_libraries(subproc_stream_ext PUBLIC ${CMAKE_DL_LIBS})
  install(TARGETS subproc_stream_ext LIBRARY DESTINATION subproc_stream)

# endif()


# enable_testing()
# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   GIT_REPOSITORY https://github.com/google/googletest.git
#   GIT_TAG v1.14.0
# )

# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)
# add_executable(testing "tests/test_hash.cpp")
# target_link_libraries(testing PRIVATE api_hash GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
# add_test(testingapihash testing)
