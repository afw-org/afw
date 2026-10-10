#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_computed_rules.as
//? customPurpose: Part of lmdb tests
//? description: A computed index (value script, and maybe a filter) names a property objects don't have. index_create refuses a filter without a value script and a computed key an object type declares (issue #516). A query that finds an object type declaring a computed name later throws: index_computed_trace.py.
//? sourceType: script
//?
//? test: index_computed_filter_needs_value
//? description: A filter on an index of a real property would leave objects that have it out of every query that uses the index, so a filter needs a value script (a computed name).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexRulesFilter";
let message: string = "";

try {
    index_create("lmdb", "dept_rf", undefined, [ot],
        "return current::object.active == true;", undefined, false, false);
}
catch (e) {
    message = e.message;
}
assert(includes(message, "filter"), "filter without value: " + message);

message = "";
try {
    index_create("lmdb", "dept_rf", undefined, [ot],
        "return current::object.active == true;", undefined, false, true);
}
catch (e) {
    message = e.message;
}
assert(includes(message, "filter"), "filter without value, test only: " + message);

assert(is_nullish(index_list("lmdb").dept_rf), "nothing was created");

return 0;


//? test: index_computed_declared_key
//? description: A value or filter script on a key an object type declares would change what that property means in queries; index_create refuses it.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexRulesDeclared";
let message: string;

add_object("lmdb", "_AdaptiveObjectType_", {
    propertyTypes: {
        FullName_rd: { dataType: "string", allowQuery: true },
        given: { dataType: "string", allowQuery: true }
    }
}, ot);

message = "";
try {
    index_create("lmdb", "FullName_rd", "current::object.given", [ot],
        undefined, undefined, false, false);
}
catch (e) {
    message = e.message;
}
assert(includes(message, "declares"), "value on a declared key: " + message);

message = "";
try {
    index_create("lmdb", "FullName_rd", "current::object.given", [ot],
        "return current::object.given != \"x\";", undefined, false, false);
}
catch (e) {
    message = e.message;
}
assert(includes(message, "declares"), "value and filter on a declared key: " + message);

assert(is_nullish(index_list("lmdb").FullName_rd), "nothing was created");

/* A plain index on the declared property is fine. */
index_create("lmdb", "given", undefined, [ot], undefined, undefined, false, false);
index_remove("lmdb", "given");

delete_object("lmdb", "_AdaptiveObjectType_", ot);

return 0;


//? test: index_computed_other_properties
//? description: With an object type, a computed name can be queried when the object type has queryable otherProperties; without them, the query parser refuses the name before any index is consulted.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const otOpen: string = "TestIndexRulesOpen";
const otClosed: string = "TestIndexRulesClosed";
let message: string;

add_object("lmdb", "_AdaptiveObjectType_", {
    propertyTypes: {
        given: { dataType: "string", allowQuery: true }
    },
    otherProperties: { dataType: "string", allowQuery: true }
}, otOpen);
add_object("lmdb", "_AdaptiveObjectType_", {
    propertyTypes: {
        given: { dataType: "string", allowQuery: true }
    }
}, otClosed);

add_object("lmdb", otOpen, { given: "Ada" }, generate_uuid());
add_object("lmdb", otClosed, { given: "Ada" }, generate_uuid());

index_create("lmdb", "FullName_ro", "current::object.given", [otOpen, otClosed],
    undefined, undefined, true, false);

const open: array = retrieve_objects("lmdb", otOpen,
    { filter: { op: "eq", property: "FullName_ro", value: "Ada" } });
assert(length(open) === 1, "queryable otherProperties");

message = "";
try {
    retrieve_objects("lmdb", otClosed,
        { filter: { op: "eq", property: "FullName_ro", value: "Ada" } });
}
catch (e) {
    message = e.message;
}
assert(includes(message, "cannot be queried"), "no otherProperties: " + message);

index_remove("lmdb", "FullName_ro");
delete_object("lmdb", "_AdaptiveObjectType_", otOpen);
delete_object("lmdb", "_AdaptiveObjectType_", otClosed);

return 0;
