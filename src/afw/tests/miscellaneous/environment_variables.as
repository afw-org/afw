#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: environment_variables.as
//? customPurpose: Process environment object (_AdaptiveEnvironmentVariables_)
//? description: Issue #71 and process env value typing (string vs hexBinary boundary)
//? sourceType: script
//?
//? test: retrieve_one_current
//? description: retrieve_objects of _AdaptiveEnvironmentVariables_ returns exactly one current (issue #71)
//? expect: 0
//? source: ...

const objs = retrieve_objects("afw", "_AdaptiveEnvironmentVariables_");
assert(length(objs) === 1, "expected one current object, got " + string(length(objs)));
const e2 = get_object("afw", "_AdaptiveEnvironmentVariables_", "current");
assert(e2 !== null, "get_object current required");
return 0;

//?
//? test: environment_qualifier_home_string
//? description: environment::HOME works and HOME is a string dataType
//? expect: 0
//? source: ...

const e = get_object("afw", "_AdaptiveEnvironmentVariables_", "current");
const home = e.HOME;
assert(home !== null && home !== undefined, "HOME should exist");
assert(meta(home).dataType === "string", "HOME should be string, got " + string(meta(home).dataType));
assert(environment::HOME === home, "environment:: qualifier should match property");
return 0;

//?
//? test: get_object_and_retrieve_same_singleton
//? description: get_object and retrieve both see a single process-env current
//? expect: 0
//? source: ...

const via_get = get_object("afw", "_AdaptiveEnvironmentVariables_", "current");
const via_retrieve = retrieve_objects("afw", "_AdaptiveEnvironmentVariables_");
assert(length(via_retrieve) === 1);
/* HOME present and string via both paths */
assert(meta(via_get.HOME).dataType === "string");
assert(meta(via_retrieve[0].HOME).dataType === "string");
return 0;
