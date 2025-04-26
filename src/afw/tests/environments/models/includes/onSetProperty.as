#!/usr/bin/env afw

assert(variable_exists("current::adapterId"));
assert(current::adapterId === 'model');

assert(variable_exists("current::object"));
assert(current::object.x === true);

assert(variable_exists("current::objectId"));
assert(current::objectId === 'abc');

assert(variable_exists("current::objectType"));
assert(current::objectType === 'TestPropertyQualifiedVariables');

// fixme remove these or add in C code?
//assert(variable_exists("current::propertyName"));
//assert(variable_exists("current::value"));
