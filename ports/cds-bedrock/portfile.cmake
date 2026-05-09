get_filename_component(SOURCE_PATH "${CURRENT_PORT_DIR}/../.." ABSOLUTE)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DCDS_BEDROCK_BUILD_TESTS=OFF
        -DCDS_BEDROCK_BUILD_EXAMPLES=OFF
)

vcpkg_cmake_install()
vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/cds-bedrock)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
