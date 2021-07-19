cmake_minimum_required (VERSION 3.13.0)
project (yaap)


##############################
# Setting build options
##############################
# RELEASE Build Type
if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Default build is Release")
  set(CMAKE_BUILD_TYPE "RELEASE")
endif ()

##############################
# Set additional compiler option
##############################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION 0.1
    COMPATIBILITY AnyNewerVersion
)

add_library(${PROJECT_NAME} INTERFACE)

# Specify where the header-only library is
target_include_directories( ${PROJECT_NAME}
    INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
)


install(TARGETS yaap
    EXPORT yaapTargets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/yaapConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/yaapConfig.cmake"
    INSTALL_DESTINATION lib/cmake/yaap
)

install(EXPORT yaapTargets DESTINATION lib/cmake/yaap)
install(FILES "${PROJECT_BINARY_DIR}/yaapConfigVersion.cmake"
              "${PROJECT_BINARY_DIR}/yaapConfig.cmake"
        DESTINATION lib/cmake/yaap)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)
