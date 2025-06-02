# header-only library
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO powerof3/CLibUtil
    REF a7a877a0551592cba58f7fc36d94d1a6d1a14db9
    SHA512 cfebe14121c41f2400bc85deeb881d631ecf0a9216bca7e963b5c7ef5de46d4048752dae1552915fb9927d25f71d430c6d2f92be195a8130ec15a5457491d473
    HEAD_REF master
)

# Install codes
set(CLIBUTIL_SOURCE	${SOURCE_PATH}/include/ClibUtil)
file(INSTALL ${CLIBUTIL_SOURCE} DESTINATION ${CURRENT_PACKAGES_DIR}/include)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
