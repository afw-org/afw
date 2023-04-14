#
# Adaptive Framework core afw library
#

# -----------------------------------------------------------------------------
# Include afwdev_generated_variables.cmake and filter out the non-applicable os
# specific source files from the lists before being used in the project.
# -----------------------------------------------------------------------------

include(generated/afwdev_generated_variables.cmake)
if (WIN32)
    list(FILTER AFWDEV_GENERATED_SOURCE_LIST EXCLUDE REGEX ".*/nix/.*")
    list(FILTER AFWDEV_GENERATED_PUBLIC_HEADERS_LIST EXCLUDE REGEX ".*/nix/.*")
else()
    list(FILTER AFWDEV_GENERATED_SOURCE_LIST EXCLUDE REGEX ".*/win/.*")
    list(FILTER AFWDEV_GENERATED_PUBLIC_HEADERS_LIST EXCLUDE REGEX ".*/win/.*")
endif()

# -----------------------------------------------------------------------------
# Include basic afw project and add additional dependencies and customizations.
# -----------------------------------------------------------------------------

include(generated/basic_afw_project.cmake)

# This library can now be referred to as afw::afw in the other projects
# built as part of afw_base.
add_library(afw::afw ALIAS ${PROJECT_NAME})

# Headers to be installed.
set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY PUBLIC_HEADER
    ${AFWDEV_GENERATED_PUBLIC_HEADERS_LIST}
)

# Includes come from source at build time and are installed in includes/afw.
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${AFWDEV_GENERATED_INCLUDE_DIRECTORIES_LIST};${AFW_PACKAGE_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${AFW_PACKAGE_INSTALL_INCLUDEDIR}>"
)

# Library is installed in lib/afw.
target_link_directories(${PROJECT_NAME} PUBLIC
    "$<INSTALL_INTERFACE:${AFW_PACKAGE_INSTALL_LIBDIR}>"
)

# Using AFW provided afw_apr-config, include apr.
set(AFW_APR_DIR cmake)
find_package(AFW_APR REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${AFW_APR_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${AFW_APR_LINK_LIBRARIES})

# Using AFW provided afw_aprutil-config, include apr-util.
set(AFW_APRUTIL_DIR cmake)
find_package(AFW_APRUTIL REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${AFW_APRUTIL_INCLUDE_DIRECTORIES})
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${AFW_APRUTIL_LINK_LIBRARIES})

# Libraries dw and unwind used for traceback on linux
if (UNIX)
    find_library(DW_LIBRARY dw REQUIRED)
    find_library(UNWIND_LIBRARY unwind REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${DW_LIBRARY}
        ${UNWIND_LIBRARY}
    )
endif()

# Dbghelp.dll is needed to build os/win on Windows
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE Dbghelp)
endif()

# Using cmake provided FindICU, include components uc and i18n
find_package(ICU COMPONENTS uc i18n REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC ICU::uc ICU::i18n)

# Using cmake provided FindLibXml2, include component LibXml2
find_package(LibXml2 REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC LibXml2::LibXml2)

# -----------------------------------------------------------------------------
# Do custom install for afw core library that includes public headers, etc.
# -----------------------------------------------------------------------------

# Produce and install configuration and macros
include(CMakePackageConfigHelpers)

# Set vesrsion and soversion for the library.
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR})

# Produce the CMake configuration file.
configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}-config.cmake.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
INSTALL_DESTINATION
    ${AFW_PACKAGE_INSTALL_CMAKEDIR}
)

# Produce the CMake version file.
write_basic_package_version_file(
    "${PROJECT_NAME}-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install the CMake configuration file.
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
    cmake/afw_macros.cmake
    cmake/afw_apr-config.cmake
    cmake/afw_aprutil-config.cmake
    DESTINATION
        ${AFW_PACKAGE_INSTALL_CMAKEDIR}
)

# Produce exports file for use by other projects  
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}_EXPORTED_TARGETS
    ARCHIVE DESTINATION ${AFW_PACKAGE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${AFW_PACKAGE_INSTALL_LIBDIR}
        NAMELINK_SKIP
    PUBLIC_HEADER DESTINATION ${AFW_PACKAGE_INSTALL_INCLUDEDIR}
)

# Install Export file.
install(EXPORT ${PROJECT_NAME}_EXPORTED_TARGETS
    DESTINATION ${AFW_PACKAGE_INSTALL_CMAKEDIR}
    NAMESPACE afw::
)

# Install headers and library.
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${AFW_PACKAGE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${AFW_PACKAGE_INSTALL_LIBDIR}
        NAMELINK_ONLY
    PUBLIC_HEADER DESTINATION ${AFW_PACKAGE_INSTALL_INCLUDEDIR}
)

# Create a pkgconfig .pc file for use by other projects.
configure_file("${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}.pc.in"
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc @ONLY )

# Install .pc file.
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${AFW_PACKAGE_INSTALL_DATAROOTDIR}/pkgconfig ) 
