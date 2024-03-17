# header-only library
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO powerof3/CLibUtil
    REF a491e2f7919211eac6d32dd2814b7908134ec1b2
    SHA512 e6b830a061d7fc99115ee7d28de0165b8c6281006df6d053342e6b5b85606a46a846ebb30f3e4598aa976c25cfe7a9dffd1c4eb72942829d1a4615d81e62cf3c
    HEAD_REF master
)

# Install codes
set(CLIBUTIL_SOURCE	${SOURCE_PATH}/include/ClibUtil)
file(INSTALL ${CLIBUTIL_SOURCE} DESTINATION ${CURRENT_PACKAGES_DIR}/include)

vcpkg_install_copyright(FILE_LIST "${SOURCE_PATH}/LICENSE")
