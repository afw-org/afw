#
# Adaptive Framework afwdev command
#
# This cmake file runs make_afwdev.py to make afwdev
# and installs it in the bin directory.
#

project(afwdev LANGUAGES NONE)

if (WIN32)
    add_custom_target(${PROJECT_NAME} ALL
        COMMAND py make_afwdev.py --dir ${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
else()
    add_custom_target(${PROJECT_NAME} ALL
        COMMAND ./make_afwdev.py --dir ${CMAKE_CURRENT_BINARY_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

# Rules to run at install time.
include(GNUInstallDirs)
install(
    PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/afwdev
    TYPE BIN
)
