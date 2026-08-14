# See the 'COPYING' file in the project root for licensing information.
#
# Find NetBSD Editline (libedit). BSD-licensed and MIT-compatible.
# Provides LibEdit::LibEdit imported interface target.
#

if(LibEdit_FOUND)
  return()
endif()

# Allow maintainers to override paths.
if(NOT LibEdit_INCLUDE_DIRS)
    set(LibEdit_INCLUDE_DIRS "")
endif()
if(NOT LibEdit_LIBRARIES)
    set(LibEdit_LIBRARIES "")
endif()

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND AND (LibEdit_INCLUDE_DIRS STREQUAL "" OR LibEdit_LIBRARIES STREQUAL ""))
    pkg_check_modules(LibEdit_PKG QUIET libedit)
    if(LibEdit_PKG_FOUND)
        if(LibEdit_INCLUDE_DIRS STREQUAL "" AND LibEdit_PKG_INCLUDE_DIRS)
            set(LibEdit_INCLUDE_DIRS ${LibEdit_PKG_INCLUDE_DIRS})
        endif()
        if(LibEdit_LIBRARIES STREQUAL "" AND LibEdit_PKG_LIBRARIES)
            set(LibEdit_LIBRARIES ${LibEdit_PKG_LIBRARIES})
        endif()
    endif()
endif()

if(LibEdit_INCLUDE_DIRS STREQUAL "")
    find_path(LibEdit_FIND_INCLUDE_DIR
        NAMES histedit.h
        DOC "libedit histedit.h include directory")
    if(LibEdit_FIND_INCLUDE_DIR)
        set(LibEdit_INCLUDE_DIRS ${LibEdit_FIND_INCLUDE_DIR})
    endif()
endif()

if(LibEdit_LIBRARIES STREQUAL "")
    find_library(LibEdit_FIND_LIBRARY
        NAMES edit
        DOC "libedit library")
    if(LibEdit_FIND_LIBRARY)
        set(LibEdit_LIBRARIES ${LibEdit_FIND_LIBRARY})
    endif()
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibEdit
    REQUIRED_VARS LibEdit_LIBRARIES LibEdit_INCLUDE_DIRS)

if(LibEdit_FOUND AND NOT TARGET LibEdit::LibEdit)
    add_library(LibEdit::LibEdit INTERFACE IMPORTED)
    set_property(TARGET LibEdit::LibEdit PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES "${LibEdit_INCLUDE_DIRS}")
    set_property(TARGET LibEdit::LibEdit PROPERTY
        INTERFACE_LINK_LIBRARIES "${LibEdit_LIBRARIES}")
endif()

mark_as_advanced(LibEdit_INCLUDE_DIRS LibEdit_LIBRARIES
    LibEdit_FIND_INCLUDE_DIR LibEdit_FIND_LIBRARY)
