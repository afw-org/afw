# Check if the module has already been included
if(CURL_FOUND)
  return()
endif()

# Check if the cURL library is available
find_path(CURL_INCLUDE_DIR curl/curl.h)
find_library(CURL_LIBRARY NAMES curl)

# Set the variables that will be made available to the rest of the project
set(CURL_FOUND TRUE)
set(CURL_INCLUDE_DIRS ${CURL_INCLUDE_DIR})
set(CURL_LIBRARIES ${CURL_LIBRARY})

# Add the cURL include directory to the include search path
include_directories(${CURL_INCLUDE_DIRS})

# Set the cURL library as an imported target
add_library(CURL::CURL IMPORTED INTERFACE)
set_property(TARGET CURL::CURL PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CURL_INCLUDE_DIRS})
set_property(TARGET CURL::CURL PROPERTY INTERFACE_LINK_LIBRARIES ${CURL_LIBRARIES})
