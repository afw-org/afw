#!/usr/bin/env afw

assert(variable_exists("current::mappedAdapterId"));
assert(current::mappedAdapterId === 'file');

// fixme remove the following from context, or add in C code?
//assert(variable_exists("current::mappedValue"));
//assert(variable_exists("current::mappedObject"));
//assert(variable_exists("current::mappedObjectId"));

assert(variable_exists("current::mappedObjectType"));
assert(current::mappedObjectType === 'TestPropertyQualifiedVariables');

assert(variable_exists("current::mappedPropertyName"));
assert(current::mappedPropertyName === 'x');

return false;