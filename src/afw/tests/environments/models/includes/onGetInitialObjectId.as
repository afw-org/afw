#!/usr/bin/env afw

assert(variable_exists("current::mappedAdapterId"));
assert(current::mappedAdapterId === 'file');

// fixme this may require getting some mapping adapter configs in place
//assert(variable_exists("current::mappedObject"));

// fixme this one may not be available ever
//assert(variable_exists("current::mappedObjectId"));

assert(variable_exists("current::adapterId"));
assert(current::adapterId === 'model');

assert(variable_exists("current::objectId"));
assert(current::objectId === 'abc');

/* if the test passed in what it wants to return for objectId, then use it */
if (variable_exists("current::adapterTypeSpecific")) {
    if (current::adapterTypeSpecific.objectId != undefined)
        return current::adapterTypeSpecific.objectId;
}

// return a made up objectId that the test will assert
return "abcdefg";