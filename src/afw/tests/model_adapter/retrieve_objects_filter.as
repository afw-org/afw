#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: retrieve_objects_filter.as
//? customPurpose: Part of model adapter tests
//? description: Query filters on a model object type with mapped property names
//? sourceType: script
//?
//? test: in-and-out
//? description: in / out with more than one value (the list was squeezed to one value and threw)
//? expect: 0
//? source: ...

add_object("file", "TestObjectType1", { TestString1: "filter-a" }, "FilterA");
add_object("file", "TestObjectType1", { TestString1: "filter-b" }, "FilterB");

const inList = retrieve_objects("model", "MyObjectType1", { filter:
    { op: "in", property: "MyTestString1", value: ["filter-a", "filter-b", "x"] } });
assert(length(inList) === 2, "in");

const outList = retrieve_objects("model", "MyObjectType1", { filter:
    { op: "and", filters: [
        { op: "in", property: "MyTestString1", value: ["filter-a", "filter-b"] },
        { op: "out", property: "MyTestString1", value: ["filter-a", "x"] }
    ] } });
assert(length(outList) === 1, "out");
assert(outList[0].MyTestString1 === "filter-b");

const rql = retrieve_objects("model", "MyObjectType1", {
    urlEncodedRQLString: "MyTestString1=in=(filter-a,filter-b)" });
assert(length(rql) === 2, "in, query string");
return 0;

//? test: boolean-string
//? description: a string value on a boolean property is read as a boolean (MyTestBoolean1=false matched true)
//? expect: 0
//? source: ...

add_object("file", "TestObjectType1", { TestString1: "bool-t", TestBoolean1: true }, "BoolT");
add_object("file", "TestObjectType1", { TestString1: "bool-f", TestBoolean1: false }, "BoolF");

const f = retrieve_objects("model", "MyObjectType1", {
    urlEncodedRQLString: "MyTestBoolean1=false" });
assert(length(f) === 1, "false");
assert(f[0].MyTestString1 === "bool-f");

const t = retrieve_objects("model", "MyObjectType1", { filter:
    { op: "eq", property: "MyTestBoolean1", value: "true" } });
assert(length(t) === 1, "true");
assert(t[0].MyTestString1 === "bool-t");
return 0;
