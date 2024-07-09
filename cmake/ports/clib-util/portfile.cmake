# header-only library
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO powerof3/CLibUtil
    REF 88d78d94464a04e582669beac56346edbbc4a662
    SHA512 960cf62e5317356f7c0d994e49f56effb89c415377e9c865e801c5ec28b57e9ec0fd2a9fd54136cd2382addedb6745cd5cc062c46cab5cccb1f634999491c9e1
    HEAD_REF master
)

# Install codes
set(CLIBUTIL_SOURCE	${SOURCE_PATH}/include/ClibUtil)
file(INSTALL ${CLIBUTIL_SOURCE} DESTINATION ${CURRENT_PACKAGES_DIR}/include)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
