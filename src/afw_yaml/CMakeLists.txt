# Adaptive Framework afw_yaml Extension

# Include afwdev generated variables.
include(generated/afwdev_generated_variables.cmake)

# Include basic afw project.
include(generated/basic_afw_project.cmake)

# Add additional dependencies.
find_package(yaml CONFIG QUIET)
if (yaml_FOUND)
    target_include_directories(${PROJECT_NAME} PRIVATE ${yaml_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${yaml_LIBRARIES})
else()
    find_package(YAML_C NO_MODULE PATHS cmake REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE YAML_C::YAML_C)
    target_link_libraries(${PROJECT_NAME} PRIVATE YAML_C::YAML_C)
endif()

# Include basic afw install.
include(generated/basic_afw_install.cmake)
