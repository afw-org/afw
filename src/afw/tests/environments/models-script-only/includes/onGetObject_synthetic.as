#!/usr/bin/env afw

/* Pure-script onGetObject — no mapped backend (issue #109) */
assert(variable_exists("current::adapterId"));
assert(current::adapterId == "scriptModel");
assert(variable_exists("current::objectId"));
assert(variable_exists("current::objectType"));
assert(current::objectType == "SyntheticRead");
assert(variable_exists("current::useDefaultProcessing"));

return {
    "x": true,
    "name": current::objectId
};
