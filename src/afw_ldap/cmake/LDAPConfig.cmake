# Check if the module has already been included
if(LDAP_FOUND)
  return()
endif()

# Check if the LDAP library is available
find_path(LDAP_INCLUDE_DIR ldap.h)
find_library(LDAP_LIBRARY NAMES ldap ldap_r)

# Check if the LDAP library is available on Windows
if(WIN32)
  find_library(WLDAP32_LIBRARY NAMES wldap32)
endif()

# Set the variables that will be made available to the rest of the project
set(LDAP_FOUND TRUE)
set(LDAP_INCLUDE_DIRS ${LDAP_INCLUDE_DIR})
set(LDAP_LIBRARIES ${LDAP_LIBRARY})

# Add the Windows LDAP library if it was found
if(WIN32 AND WLDAP32_LIBRARY)
  list(APPEND LDAP_LIBRARIES ${WLDAP32_LIBRARY})
endif()

# Add the LDAP include directory to the include search path
include_directories(${LDAP_INCLUDE_DIRS})

# Set the LDAP library as an imported target
add_library(LDAP::LDAP IMPORTED INTERFACE)
set_property(TARGET LDAP::LDAP PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LDAP_INCLUDE_DIRS})
set_property(TARGET LDAP::LDAP PROPERTY INTERFACE_LINK_LIBRARIES ${LDAP_LIBRARIES})
