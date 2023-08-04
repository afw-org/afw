# See the 'COPYING' file in the project root for licensing information.
#
# Basic afw project for subdir afw_ubjson
#
#----------------------------- N O T E -------------------------------------
#
# This file is generated by "afwdev generate".
#
# Do not change this file directly or the changes will be lost the next time
# this file is regenerated.
#
#----------------------------------------------------------------------------
#
# This file contains the project() and other commands always needed to build
# an afw project. After including this file, add other cmake commands for
# dependencies and other customizations, as needed.
#

# project
project(
    afwubjson
    LANGUAGES C
    VERSION ${afw_package_srcdirs_afw_ubjson_version}
)

# Make sure redistributes are available on Windows.
include(InstallRequiredSystemLibraries)

# Add project library.
add_library(${PROJECT_NAME} SHARED)

# Cause the appropriate symbols to be exported for this library
target_compile_definitions(${PROJECT_NAME}
    PRIVATE AFW_UBJSON_DECLARE_EXPORT
)

# Package project must be built first.
add_dependencies(${PROJECT_NAME} ${AFW_PACKAGE_PROJECT_NAME})

# Compile options from afw-package.json.
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<C_COMPILER_ID:GNU>: ${afw_package_compileOptions_GNU} >
)

# Add source files to project.
target_sources(${PROJECT_NAME} PRIVATE ${AFWDEV_GENERATED_SOURCE_LIST})

# Since this project is part of the same build that builds the afw library,
# which has not been installed yet, set INSTALL_RPATH to be able to find it.
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH 
    "$ORIGIN/../${AFW_PACKAGE_INSTALL_LIBDIR}")

# Uses AFW core library.
target_link_libraries(${PROJECT_NAME} PRIVATE afw::afw)
target_include_directories(${PROJECT_NAME} PRIVATE afw::afw)

# Add own include directories.
target_include_directories(${PROJECT_NAME}
    PRIVATE ${AFWDEV_GENERATED_INCLUDE_DIRECTORIES_LIST} )

# Include root project directory and build directory for generated headers.
target_include_directories(${PROJECT_NAME}
    PRIVATE ${AFW_PACKAGE_SOURCE_DIR}/generated ${CMAKE_BINARY_DIR})

