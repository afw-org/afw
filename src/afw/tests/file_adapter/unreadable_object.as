#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: unreadable_object.as
//? customPurpose: Part of file adapter tests
//? description: A property named _meta_ is refused; an object file that does not parse is an error to read, not a crash.
//? sourceType: script
//?
//? test: meta_not_an_object
//? description: An object file with a non-object _meta_ does not parse. Reading it used to crash copying the thrown parse error, whose context lived in the read's pool. The file is a fixture (TestUnreadable/MetaNotObject.json) since adapters now refuse to store a _meta_ property (#497).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let message: any = "no error";
try {
    get_object("file", "TestUnreadable", "MetaNotObject");
}
catch (e) {
    message = e.message;
}
assert(includes<string>(message, "_meta_"), "get_object: " + string(message));

message = "no error";
try {
    modify_object("file", "TestUnreadable", "MetaNotObject",
        [["set_property", "a", 2]]);
}
catch (e) {
    message = e.message;
}
assert(includes<string>(message, "_meta_"), "modify_object: " + string(message));

return 0;

//? test: meta_property_refused
//? description: #497: add, replace, modify, and update refuse a property named _meta_ (also nested), and the object stays readable.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

function refused(what: string, f: function): void {
    let message: any = "no error";
    try {
        f();
    }
    catch (e) {
        message = e.message;
    }
    assert(includes<string>(message, "_meta_"), what + ": " + string(message));
}

const id: string = generate_uuid();
add_object("file", "TestObjectType1", { "a": 1 }, id);

refused("modify set_property", function () {
    modify_object("file", "TestObjectType1", id,
        [["set_property", "_meta_", 1.5]]);
});
refused("modify nested name", function () {
    modify_object("file", "TestObjectType1", id,
        [["set_property", ["b", "_meta_"], 1.5]]);
});

let inner: object = { "c": 1 };
inner["_meta_"] = 1.5;
refused("modify nested value", function () {
    modify_object("file", "TestObjectType1", id,
        [["set_property", "b", inner]]);
});
refused("update_object", function () {
    update_object("file", "TestObjectType1", id, { "b": [inner] });
});

let o: object = { "a": 2 };
o["_meta_"] = 1.5;
refused("replace_object", function () {
    replace_object("file", "TestObjectType1", id, o);
});
refused("add_object", function () {
    add_object("file", "TestObjectType1", o, generate_uuid());
});

const got = get_object("file", "TestObjectType1", id);
assert(got.a === 1, "object unchanged and readable");

delete_object("file", "TestObjectType1", id);
return 0;
