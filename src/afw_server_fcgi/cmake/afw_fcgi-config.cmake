# Return if the module has already been included
if(AFW_FCGI_FOUND)
  return()
endif()

# Indicate AFW_FCGI has been found.
set(AFW_FCGI_FOUND TRUE)

# Allow the distribution maintainer to override these.
option(AFW_FCGI_INCLUDE_DIRS "Path to fcgi include directory" "")
option(AFW_FCGI_LIBRARIES "Name of fcgi library" "")
option(AFW_FCGI_LIBRARY_DIRS "Path to fcgi library directory" "")

afw_msg("afw_fcgi-config> Options passed:")
afw_msg_variable(AFW_FCGI_INCLUDE_DIRS)
afw_msg_variable(AFW_FCGI_LIBRARIES)
afw_msg_variable(AFW_FCGI_LIBRARY_DIRS)

# If maintainer didn't supply any of the afw fcgi info, use
# pkg_check_modules.
if (NOT AFW_FCGI_INCLUDE_DIRS OR
    NOT AFW_FCGI_LIBRARIES OR 
    NOT AFW_FCGI_LIBRARY_DIRS)

    # Use pkg-config to find the fcgi library.
    find_package(PkgConfig)
    pkg_check_modules(AFW_FCGI_PKG QUIET fcgi)

    # If pkg-config fcgi use it.
    if (AFW_FCGI_PKG_FOUND)
        afw_msg("fcgi found by pkg-config")

        if (NOT AFW_FCGI_INCLUDE_DIRS)
            set(AFW_FCGI_INCLUDE_DIRS ${AFW_FCGI_PKG_INCLUDE_DIRS})
        endif()

        if (NOT AFW_FCGI_LIBRARIES)
            set(AFW_FCGI_LIBRARIES ${AFW_FCGI_PKG_LIBRARIES})
        endif()

        if (NOT AFW_FCGI_LIBRARY_DIRS)
            set(AFW_FCGI_LIBRARY_DIRS ${AFW_FCGI_PKG_LIBRARY_DIRS})
        endif()

    # If pkg-config didn't find it, try finding it manually.
    else()
        afw_msg("fcgi not found using pkg-config so attempting to find manually")

        if (NOT AFW_FCGI_INCLUDE_DIRS)
            afw_msg("Using find_path for fcgiapp.h")
            find_path(AFW_FCGI_FIND_INCLUDE_DIRS
                NAMES fcgiapp.h
                PATH_SUFFIXES fastcgi
                REQUIRED)
            mark_as_advanced(AFW_FCGI_FIND_INCLUDE_DIRS)
            set(AFW_FCGI_INCLUDE_DIRS ${AFW_FCGI_FIND_INCLUDE_DIRS})
        endif()

        if (NOT AFW_FCGI_LIBRARIES OR NOT AFW_FCGI_LIBRARY_DIRS)
            afw_msg("Using find_library for fcgi")
            find_library(AFW_FCGI_FIND_LIBRARY_DIRS NAMES fcgi REQUIRED)
            mark_as_advanced(AFW_FCGI_FIND_LIBRARY_DIRS)
            if (NOT AFW_FCGI_LIBRARIES)
                get_filename_component(AFW_FCGI_LIBRARIES
                    ${AFW_FCGI_FIND_LIBRARY_DIRS} NAME_WLE)
                string(SUBSTRING ${AFW_FCGI_LIBRARIES} 0 3 AFW_FCGI_LIBRARIES_START)
                mark_as_advanced(AFW_FCGI_LIBRARIES_START)
                if (AFW_FCGI_LIBRARIES_START STREQUAL "lib")
                    string(SUBSTRING ${AFW_FCGI_LIBRARIES} 3 -1 AFW_FCGI_LIBRARIES)
                endif()
            endif()
            if (NOT AFW_FCGI_LIBRARY_DIRS)
                get_filename_component(AFW_FCGI_LIBRARY_DIRS
                    ${AFW_FCGI_FIND_LIBRARY_DIRS} DIRECTORY)
            endif()
        endif()

    endif()
endif()

# Display main variables
afw_msg("afw_fcgi-config> Variables produced:")
afw_msg_variable(AFW_FCGI_INCLUDE_DIRS)
afw_msg_variable(AFW_FCGI_LIBRARIES)
afw_msg_variable(AFW_FCGI_LIBRARY_DIRS)

# Add a afw_fcgi::fcgi interface library using the variable we just set.
add_library(afw_fcgi::fcgi UNKNOWN IMPORTED)

set_property(TARGET afw_fcgi::fcgi  PROPERTY 
    INTERFACE_INCLUDE_DIRECTORIES ${AFW_FCGI_INCLUDE_DIRS})

set_property(TARGET afw_fcgi::fcgi  PROPERTY 
    INTERFACE_LINK_LIBRARIES ${AFW_FCGI_LIBRARIES})
    
set_property(TARGET afw_fcgi::fcgi  PROPERTY 
    INTERFACE_LINK_DIRECTORIES ${AFW_FCGI_LIBRARY_DIRS})

set_property(TARGET afw_fcgi::fcgi APPEND PROPERTY
    IMPORTED_LOCATION "${AFW_FCGI_LIBRARIES}")
