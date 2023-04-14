# Return if the module has already been included
if(AFW_APR_FOUND)
  return()
endif()

# Allow the distribution maintainer to override these.
option(AFW_APR_INCLUDE_DIRECTORIES "Path to apr include directory")
option(AFW_APR_LINK_LIBRARIES "Path to of apr library")

if (AFW_APR_INCLUDE_DIRECTORIES)
    afw_msg_variable(AFW_APR_INCLUDE_DIRECTORIES "Option set: ")
endif()
if (AFW_APR_LINK_LIBRARIES)
    afw_msg_variable(AFW_APR_LINK_LIBRARIES "Option set: ")
endif()

# If both AFW_APR_INCLUDE_DIRECTORIES and AFW_APR_LINK_LIBRARIES is specified, indicate found.
if (AFW_APR_INCLUDE_DIRECTORIES AND AFW_APR_LINK_LIBRARIES)
    set(AFW_APR_FOUND TRUE)
endif()

# If not already found, try using pkg-config.
if (NOT AFW_APR_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(AFW_APR_PKG QUIET apr-1)
    if (AFW_APR_PKG_FOUND)
        afw_msg("Found apr-1 using pkg-config") 
        set(AFW_APR_FOUND TRUE)

        # Display AFW_APR_PKG_* variables
        afw_msg("AFW_APR> Variables produced by PkgConfig:")
        afw_msg_variable(AFW_APR_PKG_LIBRARIES)
        afw_msg_variable(AFW_APR_PKG_LINK_LIBRARIES)
        afw_msg_variable(AFW_APR_PKG_LIBRARY_DIRS)
        afw_msg_variable(AFW_APR_PKG_LDFLAGS)
        afw_msg_variable(AFW_APR_PKG_LDFLAGS_OTHER)
        afw_msg_variable(AFW_APR_PKG_INCLUDE_DIRS)
        afw_msg_variable(AFW_APR_PKG_CFLAGS)
        afw_msg_variable(AFW_APR_PKG_CFLAGS_OTHER)
        afw_msg_variable(AFW_APR_PKG_VERSION)
        afw_msg_variable(AFW_APR_PKG_PREFIX)
        afw_msg_variable(AFW_APR_PKG_INCLUDEDIR)
        afw_msg_variable(AFW_APR_PKG_LIBDIR)

        if (NOT AFW_APR_INCLUDE_DIRECTORIES)
            set(AFW_APR_INCLUDE_DIRECTORIES ${AFW_APR_PKG_INCLUDE_DIRS})
        endif()
        if (NOT AFW_APR_LINK_LIBRARIES)
            set(AFW_APR_LINK_LIBRARIES ${AFW_APR_PKG_LINK_LIBRARIES})
        endif()
    endif()
endif()

# If not already found, try vcpkg unofficial package.
if (NOT AFW_APR_FOUND)
    find_package(unofficial-apr CONFIG QUIET)
    if (unofficial-apr_FOUND)
        afw_msg("Found apr-1 using unofficial-apr package") 
        set(AFW_APR_FOUND TRUE)

        if (NOT AFW_APR_INCLUDE_DIRECTORIES)
            get_target_property(AFW_APR_INCLUDE_DIRECTORIES
                unofficial::apr::libapr-1 INTERFACE_INCLUDE_DIRECTORIES)
        endif()

        # This is probably vcpkg on Windows so assume libapr-1 and that it will
        # be made available.
        if (NOT AFW_APR_LINK_LIBRARIES)
            set(AFW_APR_LINK_LIBRARIES
                "/dev/vcpkg/installed/x64-windows/lib/libapr-1.lib")
        endif()
    endif()
endif()

# If not already found, it's an error.
if (NOT AFW_APR_FOUND)
    message(FATAL_ERROR "Can't find apr-1")
endif()

# Display main variables
afw_msg("AFW_APR> Variables produced:")
afw_msg_variable(AFW_APR_INCLUDE_DIRECTORIES)
afw_msg_variable(AFW_APR_LINK_LIBRARIES)

# Add a AFW_APR::apr interface library using the variable we just set.
add_library(AFW_APR::apr UNKNOWN IMPORTED)

set_property(TARGET AFW_APR::apr PROPERTY 
    INTERFACE_INCLUDE_DIRECTORIES ${AFW_APR_INCLUDE_DIRECTORIES})

set_property(TARGET AFW_APR::apr PROPERTY 
    INTERFACE_LINK_LIBRARIES ${AFW_APR_LINK_LIBRARIES})

set_property(TARGET AFW_APR::apr APPEND PROPERTY
    IMPORTED_LOCATION "${AFW_APR_LINK_LIBRARIES}")


    # # Windows way FIXME Hoping to delete WIN32 specific apr
    # if (WIN32)
    #     # Libraries apr-1 and aprutil-1
    #     find_package(unofficial-apr CONFIG REQUIRED)
    #     target_link_libraries(${PROJECT_NAME} PUBLIC
    #         unofficial::apr::libapr-1)
    #     find_library(APRUTIL_LIBRARY libaprutil-1)
    #     target_link_libraries(${PROJECT_NAME} PUBLIC
    #         ${APRUTIL_LIBRARY})
    # # Non-Windows way
    # else()
    # endif()

