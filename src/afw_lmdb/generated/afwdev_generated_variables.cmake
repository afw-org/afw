# See the 'COPYING' file in the project root for licensing information.
#
# File list variables for subdir afw_lmdb
#
#----------------------------- N O T E -------------------------------------
#
# This file is generated by "afwdev generate".
#
# Do not change this file directly or the changes will be lost the next time
# this file is regenerated.
#
#----------------------------------------------------------------------------
#
# This files sets 3 list variables based on files and directories in srcdir
# afw_lmdb that are needed a build time:
#
#  AFWDEV_GENERATED_PUBLIC_HEADERS_LIST - All of the public header files.
#  AFWDEV_GENERATED_INCLUDE_DIRECTORIES_LIST - All of the build time include directories.
#  AFWDEV_GENERATED_SOURCE_LIST - All of the c source files to compile.
#
# After including this file, use cmake commands like list(FILTER ...) to modify
# the lists as needed.
#

# Header files that are installed for public use.
set(AFWDEV_GENERATED_PUBLIC_HEADERS_LIST 
    afw_lmdb.h
    afw_lmdb_index.h
    afw_lmdb_internal.h
    afw_lmdb_metadata.h
    generated/afw_lmdb_const_objects.h
    generated/afw_lmdb_declare_helpers.h
    generated/afw_lmdb_function_bindings.h
    generated/afw_lmdb_generated.h
    generated/afw_lmdb_runtime_object_maps.h
    generated/afw_lmdb_strings.h
    generated/afw_lmdb_version_info.h
)

# Full path of directories searched for headers at build time.
set(AFWDEV_GENERATED_INCLUDE_DIRECTORIES_LIST 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/generated
)

# Source files to compile.
set(AFWDEV_GENERATED_SOURCE_LIST 
    afw_lmdb_adapter.c
    afw_lmdb_adapter_factory.c
    afw_lmdb_adapter_session.c
    afw_lmdb_extension.c
    afw_lmdb_function_lmdb.c
    afw_lmdb_index.c
    afw_lmdb_internal.c
    afw_lmdb_journal.c
    afw_lmdb_metadata.c
    generated/afw_lmdb_const_objects.c
    generated/afw_lmdb_function_bindings.c
    generated/afw_lmdb_generated.c
    generated/afw_lmdb_register_adapter_type_lmdb.c
    generated/afw_lmdb_register_error_rv_decoder_lmdb.c
    generated/afw_lmdb_runtime_object_maps.c
    generated/afw_lmdb_strings.c
)
