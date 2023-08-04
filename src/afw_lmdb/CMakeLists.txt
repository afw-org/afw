# Adaptive Framework LMDB extension

# Include afwdev generated variables.
include(generated/afwdev_generated_variables.cmake)

# Include basic afw project.
include(generated/basic_afw_project.cmake)

# Add additional dependencies.
find_package(lmdb CONFIG QUIET)
if (lmdb_FOUND)
    if(TARGET lmdb::lmdb)
        target_link_libraries(${PROJECT_NAME} PRIVATE lmdb::lmdb)
    else()
        target_link_libraries(${PROJECT_NAME} PRIVATE lmdb)
    endif()
else()
    find_package(LMDB NO_MODULE PATHS cmake REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE LMDB::LMDB)
    target_link_libraries(${PROJECT_NAME} PRIVATE LMDB::LMDB)
endif() 

# Include basic afw install.
include(generated/basic_afw_install.cmake)
