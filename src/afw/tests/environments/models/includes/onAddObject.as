#!/usr/bin/env afw

/* assert some current:: variables exist */
assert(variable_exists("current::adapterId"));
assert(current::adapterId === 'model');

assert(variable_exists("current::adapterTypeSpecific"));
assert(current::adapterTypeSpecific.func === 'add_object');

assert(variable_exists("current::object"));
assert(current::object.x === true);

assert(variable_exists("current::objectId"));
assert(current::objectId === 'abc');

assert(variable_exists("current::objectType"));
assert(current::objectType === 'TestQualifiedVariables');

assert(variable_exists("current::useDefaultProcessing"));

assert(variable_exists("current::objectId"));
assert(current::objectId === 'abc');

/* the test expects one object to be returned */
return "xyz";