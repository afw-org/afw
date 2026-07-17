# Adaptive Framework afw command

# Include afwdev generated variables.
include(generated/afwdev_generated_variables.cmake)

# Include basic afw project.
include(generated/basic_afw_project.cmake)

# libedit (NetBSD Editline) for interactive line editing and history.
# BSD-licensed and MIT-compatible. Available on all supported distros
# (Ubuntu: libedit-dev, Alpine: libedit-dev, Rocky/OpenSUSE: libedit-devel).
# Optional so builds still succeed if the package is not installed; when found,
# interactive TTY sessions get line editing and ~/.afw_history.
find_package(LibEdit NO_MODULE PATHS cmake QUIET)
if(LibEdit_FOUND)
    message(STATUS "afw_command: libedit found — interactive line editing enabled")
    # Link only: INTERFACE includes/libs come from LibEdit::LibEdit.
    # Do not pass LibEdit::LibEdit to target_include_directories — CMake would
    # treat the name as a filesystem path (e.g. .../LibEdit::LibEdit).
    target_link_libraries(${PROJECT_NAME} PRIVATE LibEdit::LibEdit)
    target_compile_definitions(${PROJECT_NAME} PRIVATE AFW_COMMAND_HAVE_LIBEDIT=1)
else()
    message(STATUS
        "afw_command: libedit not found — interactive line editing disabled "
        "(install libedit-dev / libedit-devel to enable)")
endif()

# Include basic afw install.
include(generated/basic_afw_install.cmake)
