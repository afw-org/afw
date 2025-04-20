#!/usr/bin/env afw

/* assert some current:: variables exist */

assert(variable_exists("current::adapterId"));
assert(current::adapterId === 'model');

assert(variable_exists("current::modifyEntries"));
assert(current::modifyEntries[0][0] === 'set_property');

assert(variable_exists("current::adapterTypeSpecific"));
assert(current::adapterTypeSpecific.func === 'modify_object');

assert(variable_exists("current::objectId"));
assert(current::objectId === 'xyz');

assert(variable_exists("current::objectType"));
assert(current::objectType === 'TestQualifiedVariables');

assert(variable_exists("current::useDefaultProcessing"));

/* the test expects one object to be returned */
return true;