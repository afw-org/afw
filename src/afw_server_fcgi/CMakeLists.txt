# Adaptive Framework FCGI server

# Include afwdev generated variables.
include(generated/afwdev_generated_variables.cmake)

# Include basic afw project.
include(generated/basic_afw_project.cmake)

# Add additional dependencies.
find_package(afw_fcgi PATHS cmake REQUIRED)
# target_include_directories(${PROJECT_NAME} PRIVATE afw_fcgi::fcgi)
# target_link_libraries(${PROJECT_NAME} PRIVATE afw_fcgi::fcgi)
target_include_directories(${PROJECT_NAME} PRIVATE ${AFW_FCGI_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${AFW_FCGI_LIBRARIES})

# Include basic afw install.
include(generated/basic_afw_install.cmake)
