# header-only library
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO powerof3/CLibUtil
    REF a801992250ee8c6178159cc73ce3354a742be6b4
    SHA512 3eb37eb0958bf5cf01c8cd80f673b5ab60c562757d76024937fde529b82938481ad8d6fa30d6ca21d5ef93ab8a2d92e7ad093ddc242fb2462aa234689efd31eb
    HEAD_REF master
)

# Install codes
set(CLIBUTIL_SOURCE	${SOURCE_PATH}/include/ClibUtil)
file(INSTALL ${CLIBUTIL_SOURCE} DESTINATION ${CURRENT_PACKAGES_DIR}/include)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
