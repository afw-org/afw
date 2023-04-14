#
# Adaptive Framework libafw, afw command, afwdev, afwfcgi, and base extensions
#

# CMake 3.15 and below features are used
cmake_minimum_required (VERSION 3.15)

# Requires C standard C11 or higher
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Review what happens when extensions are off and see if they should be fixed.
#set(CMAKE_C_EXTENSIONS OFF)

# This is the afw base package
project(afw_package LANGUAGES C)

# Core library afw is required by all Adaptive Framework commands and
# extensions. Since it and its associated CMake Find*.cmake files are not
# installed yet, src/afw/cmake is appended to the CMAKE_MODULE_PATH variable
# so that the other projects in this repository can find them. This is only
# needed in this base Adaptive Framework repository.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/src/afw/cmake)

# Include the afw cmake macros. This is done instead of using find_package
# because this is the core afw package and it is not installed.
include(src/afw/cmake/afw_macros.cmake)

# Include basic afwdev generated afw package build. See this file for
# documentation and customization options.
include(generated/afw_package_basic_build.cmake)
