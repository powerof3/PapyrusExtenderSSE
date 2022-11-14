# header-only library
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO powerof3/CLibUtil
    REF 8dff3f8f064ba662d987b94e1b007ff299cf4ef1
    SHA512 fdc0a82aa8e6863eefc968e45df2aaf782e1ccad083fa4fd308f6dc53a402641ad1efc26ca9eae2a2526be30def6c3d865719c4a9d82d575b218aac7ef581ba4
    HEAD_REF master
)

# Install codes
set(CLIBUTIL_SOURCE	${SOURCE_PATH}/include/ClibUtil)
file(INSTALL ${CLIBUTIL_SOURCE} DESTINATION ${CURRENT_PACKAGES_DIR}/include)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
