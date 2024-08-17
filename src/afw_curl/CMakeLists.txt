# Adaptive Framework afw_curl extension

# Include afwdev generated variables.
include(generated/afwdev_generated_variables.cmake)

# Include basic afw project.
include(generated/basic_afw_project.cmake)

# Add additional dependencies.
find_package(CURL NO_MODULE PATHS cmake REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE CURL::CURL)
target_link_libraries(${PROJECT_NAME} PRIVATE CURL::CURL)

# Include basic afw install.
include(generated/basic_afw_install.cmake)
