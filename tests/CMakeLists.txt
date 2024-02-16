cmake_minimum_required(VERSION 3.16)
project(vnigma-tests)

#----------------------------------------------------------------------------------------------------------------------
# general settings and options
#----------------------------------------------------------------------------------------------------------------------

include("../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
endif()

#----------------------------------------------------------------------------------------------------------------------
# testing framework
#----------------------------------------------------------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE) # do not override parent project's runtime settings on Windows
set(INSTALL_GTEST OFF)

# For simplicity, always build googletest as static library. This prevents vnigma-tests executable from
# complaining about missing googletest DLLs on Windows.
set(BUILD_SHARED_LIBS OFF)

FetchContent_MakeAvailable(googletest)

#----------------------------------------------------------------------------------------------------------------------
# tests dependencies
#----------------------------------------------------------------------------------------------------------------------

if(is_top_level)
    find_package(vnigma REQUIRED)
endif()

#----------------------------------------------------------------------------------------------------------------------
# tests sources
#----------------------------------------------------------------------------------------------------------------------

set(sources
    mock_devices.hpp
    baud_test.cpp
    control_test.cpp
    buffer_test.cpp
    binary_test.cpp
    buffer_manip_test.cpp
    message_variant_test.cpp
    serial_port_test.cpp
    number_util_test.cpp
    control_message_test.cpp
    das_message_test.cpp
    das_set_frequency_test.cpp
    das_set_reset_test.cpp
    das_set_config_test.cpp
    das_get_config_test.cpp
    das_sd_p.h
    das_greed_send_data_test.cpp
    das_scoped_send_data_test.cpp
    payload_test.cpp
    das_ack_test.cpp
    das_handshake_test.cpp
    buffer_to_message_variant_test.cpp
    data_test.cpp
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

#----------------------------------------------------------------------------------------------------------------------
# tests target
#----------------------------------------------------------------------------------------------------------------------

add_executable(vnigma-tests)
target_sources(vnigma-tests PRIVATE ${sources})

target_link_libraries(vnigma-tests
    PRIVATE
        vnigma::vnigma
        gtest_main)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(vnigma-tests vnigma::vnigma)
endif()

include(GoogleTest)
gtest_discover_tests(vnigma-tests)
