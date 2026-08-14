#!/usr/bin/env afw

/* Pure-script onRetrieveObjects — no mapped backend (issue #109) */
assert(variable_exists("current::adapterId"));
assert(current::adapterId == "scriptModel");
assert(variable_exists("current::returnObject"));
assert(variable_exists("current::useDefaultProcessing"));

current::returnObject({ "x": true, "name": "one" });
current::returnObject({ "x": false, "name": "two" });

return null;
