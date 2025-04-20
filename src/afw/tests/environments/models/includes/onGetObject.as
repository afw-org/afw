#!/usr/bin/env afw

/* assert some current:: variables exist */
assert(variable_exists("current::adapterId"));
assert(current::adapterId == 'model');

assert(variable_exists("current::adapterTypeSpecific"));
assert(current::adapterTypeSpecific.func == 'get_object');

assert(variable_exists("current::mapBackObject"));
assert(variable_exists("current::objectId"));
assert(variable_exists("current::objectType"));
assert(variable_exists("current::useDefaultProcessing"));
assert(variable_exists("current::mappedAdapterId"));
assert(variable_exists("current::mappedObjectType"));

/* the test expects one object to be returned */
return(
    { "x": true }
);