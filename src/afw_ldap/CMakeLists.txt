# Adaptive Framework LDAP extension

# Include afwdev generated variables.
include(generated/afwdev_generated_variables.cmake)

# Include basic afw project.
include(generated/basic_afw_project.cmake)

# OpenLDAP (not apr-util LDAP).
find_package(LDAP NO_MODULE PATHS cmake REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE LDAP::LDAP)
target_link_libraries(${PROJECT_NAME} PRIVATE LDAP::LDAP)

# Include basic afw install.
include(generated/basic_afw_install.cmake)
