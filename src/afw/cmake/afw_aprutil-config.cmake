# Return if the module has already been included
if(AFW_APRUTIL_FOUND)
  return()
endif()

# Allow the distribution maintainer to override these.
option(AFW_APRUTIL_INCLUDE_DIRECTORIES "Path to apr-util include directory")
option(AFW_APRUTIL_LINK_LIBRARIES "Path to of apr-util library")
if (AFW_APRUTIL_INCLUDE_DIRECTORIES)
    afw_msg_variable(AFW_APRUTIL_INCLUDE_DIRECTORIES "Option set: ")
endif()
if (AFW_APRUTIL_LINK_LIBRARIES)
    afw_msg_variable(AFW_APRUTIL_LINK_LIBRARIES  "Option set: ")
endif()

# If maintainer supplied everything needed, indicate found.
if (AFW_APRUTIL_INCLUDE_DIRECTORIES AND AFW_APRUTIL_LINK_LIBRARIES)
    set(AFW_APRUTIL_FOUND TRUE)
endif()

# If not already found, try using pkg-config.
if (NOT AFW_APRUTIL_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(AFW_APRUTIL_PKG QUIET apr-util-1)
    if (AFW_APRUTIL_PKG_FOUND)
        afw_msg("pkg-config located apr-util-1") 
        set(AFW_APRUTIL_FOUND TRUE)

        # Display AFW_APRUTIL_PKG_* variables
        afw_msg("AFW_APRUTIL> Variables produced by PkgConfig:")
        afw_msg_variable(AFW_APRUTIL_PKG_LIBRARIES)
        afw_msg_variable(AFW_APRUTIL_PKG_LINK_LIBRARIES)
        afw_msg_variable(AFW_APRUTIL_PKG_LIBRARY_DIRS)
        afw_msg_variable(AFW_APRUTIL_PKG_LDFLAGS)
        afw_msg_variable(AFW_APRUTIL_PKG_LDFLAGS_OTHER)
        afw_msg_variable(AFW_APRUTIL_PKG_INCLUDE_DIRS)
        afw_msg_variable(AFW_APRUTIL_PKG_CFLAGS)
        afw_msg_variable(AFW_APRUTIL_PKG_CFLAGS_OTHER)
        afw_msg_variable(AFW_APRUTIL_PKG_VERSION)
        afw_msg_variable(AFW_APRUTIL_PKG_PREFIX)
        afw_msg_variable(AFW_APRUTIL_PKG_INCLUDEDIR)
        afw_msg_variable(AFW_APRUTIL_PKG_LIBDIR)

        if (NOT AFW_APRUTIL_INCLUDE_DIRECTORIES)
            set(AFW_APRUTIL_INCLUDE_DIRECTORIES ${AFW_APRUTIL_PKG_INCLUDE_DIRS})
        endif()

        if (NOT AFW_APRUTIL_LINK_LIBRARIES)
            set(AFW_APRUTIL_LINK_LIBRARIES ${AFW_APRUTIL_PKG_LIBRARIES})
        endif()
    endif()
endif()

# If not already found, try checking where apr-1 was found. This counts on AFW_APR
# being found first, which is not optimal so should probably be fixed.
if (NOT AFW_APRUTIL_FOUND)
    if (AFW_APR_FOUND)
        afw_msg("Found apr-util-1 using apr-1 variables") 
        set(AFW_APRUTIL_FOUND TRUE)

        if (NOT AFW_APRUTIL_INCLUDE_DIRECTORIES)
            set(AFW_APRUTIL_INCLUDE_DIRECTORIES ${AFW_APR_INCLUDE_DIRECTORIES})
        endif()

        # This is probably vcpkg on Windows so assume libaprutil-1 and that it will
        # be made available.
        if (NOT AFW_APRUTIL_LINK_LIBRARIES)
            set(AFW_APRUTIL_LINK_LIBRARIES
                "/dev/vcpkg/installed/x64-windows/lib/libaprutil-1")
        endif()
    endif()
endif()

# If not already found, it's an error.
if (NOT AFW_APRUTIL_FOUND)
    message(FATAL_ERROR "Can't find apr-util-1")
endif()

# Display main variables
afw_msg("AFW_APRUTIL> Variables produced:")
afw_msg_variable(AFW_APRUTIL_INCLUDE_DIRECTORIES)
afw_msg_variable(AFW_APRUTIL_LINK_LIBRARIES)

# Add a AFW_APR::apr-util interface library using the variable we just set.
add_library(AFW_APR::apr-util UNKNOWN IMPORTED)

set_property(TARGET AFW_APR::apr-util PROPERTY 
    INTERFACE_INCLUDE_DIRECTORIES ${AFW_APRUTIL_INCLUDE_DIRECTORIES})

set_property(TARGET AFW_APR::apr-util PROPERTY 
    INTERFACE_LINK_LIBRARIES ${AFW_APRUTIL_LINK_LIBRARIES})

set_property(TARGET AFW_APR::apr-util APPEND PROPERTY
    IMPORTED_LOCATION "${AFW_APRUTIL_LINK_LIBRARIES}")
