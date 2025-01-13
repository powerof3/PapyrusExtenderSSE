# header-only library
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO powerof3/CLibUtil
    REF 93cc4e36498923a12ae017fc5141ac0c90207a03
    SHA512 f598af90a10b22661248217131de4a31a1fc124ebd23442dfe6e1d248c8a98a356c68a0cc369fe31938ab6ceb38bf5fff31695a0f84308b01efe9794bb32562e
    HEAD_REF master
)

# Install codes
set(CLIBUTIL_SOURCE	${SOURCE_PATH}/include/ClibUtil)
file(INSTALL ${CLIBUTIL_SOURCE} DESTINATION ${CURRENT_PACKAGES_DIR}/include)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
