#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: crud_hooks.as
//? customPurpose: Part of model script-only tests
//? description: issue #109 pure-script write hooks without mappedAdapterId
//? sourceType: script
//?
//? test: add_modify_replace_delete
//? description: issue #109 add/modify/replace/delete fully handled by on* hooks
//? expect: 0
//? source: ...

const uuid: string = generate_uuid();

const addResult = add_object(
    "scriptModel",
    "SyntheticWrite",
    { "id": uuid, "ok": true },
    uuid
);
assert(addResult.objectId === uuid);

const got = get_object("scriptModel", "SyntheticWrite", uuid);
assert(got !== undefined);
assert(got.id === uuid);
assert(got.ok === true);

modify_object(
    "scriptModel",
    "SyntheticWrite",
    uuid,
    [["set_property", "ok", false]]
);

replace_object(
    "scriptModel",
    "SyntheticWrite",
    uuid,
    { "id": uuid, "ok": false }
);

delete_object("scriptModel", "SyntheticWrite", uuid);

return 0;
