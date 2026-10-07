#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: unreadable_object.as
//? customPurpose: Part of file adapter tests
//? description: An object file that does not parse is an error to read, not a crash.
//? sourceType: script
//?
//? test: meta_not_an_object
//? description: An object modified to have a non-object _meta_ property is stored as written and then does not parse. Reading it used to crash copying the thrown parse error, whose context lived in the read's pool.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const id: string = generate_uuid();
add_object("file", "TestObjectType1", { "a": 1 }, id);
modify_object("file", "TestObjectType1", id, [["set_property", "_meta_", 1.5]]);

let message: any = "no error";
try {
    get_object("file", "TestObjectType1", id);
}
catch (e) {
    message = e.message;
}
assert(includes<string>(message, "_meta_"), "get_object: " + string(message));

message = "no error";
try {
    modify_object("file", "TestObjectType1", id, [["set_property", "a", 2]]);
}
catch (e) {
    message = e.message;
}
assert(includes<string>(message, "_meta_"), "modify_object: " + string(message));

delete_object("file", "TestObjectType1", id);
return 0;
