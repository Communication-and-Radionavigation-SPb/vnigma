cmake_minimum_required(VERSION 3.14)
project(vnigma-add LANGUAGES CXX)

include("../../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    find_package(vnigma REQUIRED)
endif()

set(sources main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(vnigma-add)
target_sources(vnigma-add PRIVATE ${sources})
target_link_libraries(vnigma-add PRIVATE vnigma::vnigma)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(vnigma-add vnigma::vnigma)
endif()
