#
# CMake macros for Adaptive Framework
#

macro(afw_msg message)
    message(STATUS ${message})
endmacro()


# Produce message variable=value. A second argument can be a different prefix
# other than two spaces.
macro(afw_msg_variable variable)
    set(prefix "  ")
    if (${ARGC} EQUAL 2)
        set(prefix ${ARGV1})
    elseif(${ARGC} GREATER 2)
        message(FATAL_ERROR "Too many arguments passed to macro afw_msg_variable - (${ARGC})")
    endif()
    list(LENGTH ${variable} len)
    if(${len} GREATER 1)
        string(JOIN " " comma_seperated ${${variable}})
        afw_msg("${prefix}${variable}(list)=${comma_seperated}")
    else()
        afw_msg("${prefix}${variable} = ${${variable}}")
    endif()
endmacro()

# Produce message for all properties of a target.
macro(afw_msg_target_properties target)
    set(prefix "  ")
    afw_msg("Properties for target ${target}:")
    get_target_property(all_properties ${target} PROPERTIES)
    foreach(property ${all_properties})
        afw_msg("${prefix}${property} = ${${property}}")
    endforeach()
endmacro()
