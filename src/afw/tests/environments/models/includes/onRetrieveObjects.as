#!/usr/bin/env afw

/* assert some current:: variables exist */
assert(variable_exists("current::returnObject"));
assert(variable_exists("current::mapBackObject"));
assert(variable_exists("current::useDefaultProcessing"));

assert(variable_exists("current::adapterId"));
assert(current::adapterId == 'model');

assert(variable_exists("current::objectType"));
assert(current::objectType == 'TestQualifiedVariables');

assert(variable_exists("current::queryCriteria"));
assert(current::queryCriteria.filter.op === "eq");
assert(current::queryCriteria.filter.property === "x");
assert(current::queryCriteria.filter.value === "true");

assert(variable_exists("current::mappedAdapterId"));
assert(current::mappedAdapterId == "file");

assert(variable_exists("current::mappedObjectType"));
assert(current::mappedObjectType == "TestQualifiedVariables");

assert(variable_exists("current::adapterTypeSpecific"));
assert(current::adapterTypeSpecific.func == "retrieve_objects");

/* the test expects one object to be returned */
current::returnObject(
    { "x": true }
);