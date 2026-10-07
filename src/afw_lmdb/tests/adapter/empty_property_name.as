#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: empty_property_name.as
//? customPurpose: Part of lmdb tests
//? description: An object with an empty property name can be stored, read, and changed.
//? sourceType: script
//?
//? test: empty_property_name
//? description: An empty property name read back from LMDB has no octets; comparing it with another name used to pass NULL to memcmp (UBSan).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestEmptyNameType";
const id: string = generate_uuid();
add_object("lmdb", ot, { "a": 1 }, id);
modify_object("lmdb", ot, id, [["set_property", "", 2]]);
modify_object("lmdb", ot, id, [["set_property", "b", 3]]);
modify_object("lmdb", ot, id, [["set_property", "", 4]]);
const o: object = get_object("lmdb", ot, id);
assert(o[""] === 4, "empty name: " + stringify(o));
assert(o.b === 3);
delete_object("lmdb", ot, id);
return 0;
