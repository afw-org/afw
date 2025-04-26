#!/usr/bin/env afw

/* assert some current:: variables exist */
assert(variable_exists("current::adapterId"));
assert(current::adapterId === 'model');

assert(variable_exists("current::object"));
assert(current::object.new === "obj");

assert(variable_exists("current::adapterTypeSpecific"));
assert(current::adapterTypeSpecific.func === 'replace_object');

assert(variable_exists("current::objectId"));
assert(current::objectId === 'xyz');

assert(variable_exists("current::objectType"));
assert(current::objectType === 'TestQualifiedVariables');

assert(variable_exists("current::useDefaultProcessing"));

/* the test expects one object to be returned */
return true;