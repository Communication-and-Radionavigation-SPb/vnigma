cmake_minimum_required(VERSION 3.14)
project(vnigma
    VERSION 1.0.0
    DESCRIPTION "DAS and Venom modules interaction protocol library built with CMake"
    LANGUAGES CXX)

# /* -------------------------------------------------------------------------- */
# /*                        General settings and options                        */
# /* -------------------------------------------------------------------------- */

include(cmake/utils.cmake)
include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

# VNIGMA_SHARED_LIBS option (undefined by default) can be used to force shared/static build
option(VNIGMA_BUILD_EXAMPLES "Build vnigma examples" OFF)
option(VNIGMA_BUILD_TESTS "Build vnigma tests" OFF)
option(VNIGMA_BUILD_DOCS "Build vnigma documentation" OFF)
option(VNIGMA_INSTALL "Generate target for installing vnigma" ${is_top_level})
set_if_undefined(VNIGMA_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/vnigma" CACHE STRING
    "Install path for vnigma package-related CMake files")

if(DEFINED VNIGMA_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${VNIGMA_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(vnigma) # initialized below
add_library(vnigma::vnigma ALIAS vnigma)

# /* -------------------------------------------------------------------------- */
# /*                                   Sources                                  */
# /* -------------------------------------------------------------------------- */

include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif()

generate_export_header(vnigma EXPORT_FILE_NAME include/vnigma/${export_file_name})

include(cmake/CMakeHelpers.cmake)

add_source_folder(include/vnigma)
add_source_folder(include/vnigma/util)
add_source_folder(include/vnigma/message)
add_source_folder(include/vnigma/ports)
add_source_folder(src)
add_source_folder(src/message)
add_source_folder(src/ports)

set(sources
    ${VN_HEADER_FILES}
    ${VN_SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

# /* -------------------------------------------------------------------------- */
# /*                               Library target                               */
# /* -------------------------------------------------------------------------- */

include(CMakePackageConfigHelpers)

target_sources(vnigma PRIVATE ${sources})
target_compile_definitions(vnigma PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:VNIGMA_STATIC_DEFINE>")

target_include_directories(vnigma
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

set_target_properties(vnigma PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION})

if(VNIGMA_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(cmake/vnigma-config.cmake.in vnigma-config.cmake
        INSTALL_DESTINATION "${VNIGMA_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(vnigma-config-version.cmake
        COMPATIBILITY SameMajorVersion)

    install(TARGETS vnigma EXPORT vnigma_export
        RUNTIME COMPONENT vnigma
        LIBRARY COMPONENT vnigma NAMELINK_COMPONENT vnigma-dev
        ARCHIVE COMPONENT vnigma-dev
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY include/
        TYPE INCLUDE
        COMPONENT vnigma-dev)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/vnigma/${export_file_name}"
        COMPONENT vnigma-dev
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/vnigma")

    set(targets_file "vnigma-shared-targets.cmake")

    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "vnigma-static-targets.cmake")
    endif()

    install(EXPORT vnigma_export
        COMPONENT vnigma-dev
        FILE "${targets_file}"
        DESTINATION "${VNIGMA_INSTALL_CMAKEDIR}"
        NAMESPACE vnigma::)

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/vnigma-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/vnigma-config-version.cmake"
        COMPONENT vnigma-dev
        DESTINATION "${VNIGMA_INSTALL_CMAKEDIR}")

    if(MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if(BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:vnigma>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else()
            # TARGET_PDB_FILE does not work for pdb file generated for static library build, determining it manually
            set(pdb_file "$<TARGET_FILE_DIR:vnigma>/$<TARGET_FILE_PREFIX:vnigma>$<TARGET_FILE_BASE_NAME:vnigma>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif()

        install(FILES "${pdb_file}"
            COMPONENT vnigma-dev
            CONFIGURATIONS Debug RelWithDebInfo
            DESTINATION "${pdb_file_destination}"
            OPTIONAL)
    endif()
endif()

# /* -------------------------------------------------------------------------- */
# /*                                  Examples                                  */
# /* -------------------------------------------------------------------------- */

if(VNIGMA_BUILD_EXAMPLES)
    add_subdirectory(examples/execution)
endif()
# /* -------------------------------------------------------------------------- */
# /*                                    Tests                                   */
# /* -------------------------------------------------------------------------- */

if(VNIGMA_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# /* -------------------------------------------------------------------------- */
# /*                                Documentation                               */
# /* -------------------------------------------------------------------------- */

if(VNIGMA_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(docs include)
endif()
