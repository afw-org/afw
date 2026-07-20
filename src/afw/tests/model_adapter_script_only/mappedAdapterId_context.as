#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: mappedAdapterId_context.as
//? customPurpose: Part of model script-only tests
//? description: issue #109 current::mappedAdapterId nullish when conf omits it
//? sourceType: script
//?
//? test: mappedAdapterId_nullish_in_onGetObject
//? description: issue #109 onGetObject sees nullish mappedAdapterId
//? expect: 0
//? source: ...

const obj = get_object("scriptModel", "ContextCheck", "ctx1");
assert(obj !== undefined);
assert(obj.mappedAbsent === true);
assert(obj.snapshotOk === true, "issue #9 qualifier(\"current\") snapshot in onGetObject");

return 0;
