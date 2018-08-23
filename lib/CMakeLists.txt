CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/lib/moses.conf
    ${PROJECT_BINARY_DIR}/lib/moses.conf)
# Create a test configuration file by copying the default configuration file
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/lib/moses-test.conf
    ${PROJECT_BINARY_DIR}/lib/moses-test.conf)

export(EXPORT ASMosesTargets
          FILE "${CMAKE_CURRENT_BINARY_DIR}/ASMoses/ASMosesTargets.cmake"
      )

set(ConfigPackageLocation lib/cmake/ASMoses)

install(EXPORT ASMosesTargets
  FILE
    ASMosesTargets.cmake
  DESTINATION
    ${ConfigPackageLocation}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(ASMosesConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/ASMOSESConfig.cmake
                              INSTALL_DESTINATION lib/ASMoses/cmake
                              PATH_VARS CMAKE_INSTALL_PREFIX)


write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/ASMOSESConfigVersion.cmake"
  VERSION ${ASMOSES_VERSION}
  COMPATIBILITY SameMajorVersion
)

INSTALL(FILES
       ${CMAKE_CURRENT_BINARY_DIR}/ASMOSESConfigVersion.cmake
       ${CMAKE_CURRENT_BINARY_DIR}/ASMOSESConfig.cmake
       DESTINATION
       ${ConfigPackageLocation})
