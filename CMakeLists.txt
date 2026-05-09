cmake_minimum_required(VERSION 3.21)

project(cds-bedrock
    VERSION 0.1.0
    DESCRIPTION "Foundational data structures built from scratch in C11"
    HOMEPAGE_URL "https://github.com/pranavdadhe1806/CDS-Bedrock"
    LANGUAGES C
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(PROJECT_IS_TOP_LEVEL)
    set(CDS_BEDROCK_DEFAULT_BUILD_EXTRAS ON)
else()
    set(CDS_BEDROCK_DEFAULT_BUILD_EXTRAS OFF)
endif()

option(CDS_BEDROCK_BUILD_TESTS "Build CDS-Bedrock tests" ${CDS_BEDROCK_DEFAULT_BUILD_EXTRAS})
option(CDS_BEDROCK_BUILD_EXAMPLES "Build CDS-Bedrock examples" ${CDS_BEDROCK_DEFAULT_BUILD_EXTRAS})
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

add_library(cds-bedrock
    src/value.c
    src/bedrock_array.c
    src/bedrock_list.c
    src/bedrock_stack.c
    src/bedrock_queue.c
    src/bedrock_bst.c
    src/bedrock_hmap.c
    src/heap.c
    src/graph.c
)

add_library(cds-bedrock::cds-bedrock ALIAS cds-bedrock)

target_compile_features(cds-bedrock PUBLIC c_std_11)
set_target_properties(cds-bedrock PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    EXPORT_NAME cds-bedrock
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

target_include_directories(cds-bedrock
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(UNIX AND NOT APPLE)
    target_link_libraries(cds-bedrock PUBLIC m)
endif()

if(MSVC)
    target_compile_options(cds-bedrock PRIVATE /W4)
else()
    target_compile_options(cds-bedrock PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(CDS_BEDROCK_BUILD_EXAMPLES)
    add_executable(cds-bedrock-example examples/example.c)
    target_link_libraries(cds-bedrock-example PRIVATE cds-bedrock::cds-bedrock)
endif()

if(CDS_BEDROCK_BUILD_TESTS)
    enable_testing()

    set(CDS_BEDROCK_TESTS
        test_bedrock_array
        test_bedrock_list
        test_bedrock_stack
        test_bedrock_queue
        test_stack
        test_queue
        test_bst
        test_bedrock_hmap
        test_heap
        test_graph
    )

    foreach(test_name IN LISTS CDS_BEDROCK_TESTS)
        add_executable(${test_name} tests/${test_name}.c)
        target_link_libraries(${test_name} PRIVATE cds-bedrock::cds-bedrock)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()

install(TARGETS cds-bedrock
    EXPORT cds-bedrockTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

set(CDS_BEDROCK_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/cds-bedrock")

install(EXPORT cds-bedrockTargets
    FILE cds-bedrockTargets.cmake
    NAMESPACE cds-bedrock::
    DESTINATION ${CDS_BEDROCK_CONFIG_INSTALL_DIR}
)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/cds-bedrockConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/cds-bedrockConfig.cmake"
    INSTALL_DESTINATION ${CDS_BEDROCK_CONFIG_INSTALL_DIR}
)

write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/cds-bedrockConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${PROJECT_BINARY_DIR}/cds-bedrockConfig.cmake"
    "${PROJECT_BINARY_DIR}/cds-bedrockConfigVersion.cmake"
    DESTINATION ${CDS_BEDROCK_CONFIG_INSTALL_DIR}
)

install(FILES LICENSE
    DESTINATION ${CMAKE_INSTALL_DATADIR}/licenses/cds-bedrock
)
