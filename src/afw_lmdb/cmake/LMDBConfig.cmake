# Check if the module has already been included
if(LMDB_FOUND)
  return()
endif()

# Check if the LMDB library is available
find_path(LMDB_INCLUDE_DIR lmdb.h)
find_library(LMDB_LIBRARY NAMES lmdb)

# Set the variables that will be made available to the rest of the project
set(LMDB_FOUND TRUE)
set(LMDB_INCLUDE_DIRS ${LMDB_INCLUDE_DIR})
set(LMDB_LIBRARIES ${LMDB_LIBRARY})

# Add the LMDB include directory to the include search path
include_directories(${LMDB_INCLUDE_DIRS})

# Set the LMDB library as an imported target
add_library(LMDB::LMDB IMPORTED INTERFACE)
set_property(TARGET LMDB::LMDB PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LMDB_INCLUDE_DIRS})
set_property(TARGET LMDB::LMDB PROPERTY INTERFACE_LINK_LIBRARIES ${LMDB_LIBRARIES})
