# Check if the module has already been included
if(YAML_C_FOUND)
  return()
endif()

# Check if the libyaml library is available
find_path(YAML_C_INCLUDE_DIR yaml.h)
find_library(YAML_C_LIBRARY NAMES yaml)

# Set the variables that will be made available to the rest of the project
set(YAML_C_FOUND TRUE)
set(YAML_C_INCLUDE_DIRS ${YAML_C_INCLUDE_DIR})
set(YAML_C_LIBRARIES ${YAML_C_LIBRARY})

# Add the libyaml include directory to the include search path
include_directories(${YAML_C_INCLUDE_DIRS})

# Set the libyaml library as an imported target
add_library(YAML_C::YAML_C IMPORTED INTERFACE)
set_property(TARGET YAML_C::YAML_C PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${YAML_C_INCLUDE_DIRS})
set_property(TARGET YAML_C::YAML_C PROPERTY INTERFACE_LINK_LIBRARIES ${YAML_C_LIBRARIES})
