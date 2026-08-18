#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: retrieve_objects.as
//? customPurpose: Part of file adapter tests
//? description: Test file adapter retrieve_objects.
//? sourceType: script
//?
//? test: retrieve_objects_test-1
//? description: Test file adapter retrieve_objects.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1");

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;

//?
//? test: retrieve_objects-mutable-face
//? description: ...
Issue #17: each retrieve_objects hit is a mutable face; mutate first list
element and re-retrieve — store view must not keep face-only props.
//? skip: false
//? expect: 0
//? source: ...

const list1: array = retrieve_objects("file", "TestObjectType1");
const o1: object = list1[0];
assert(o1.TestString1 === "This is a test string.");
o1.TestString1 = "face-only";
o1.marker = "from_retrieve_1";

const list2: array = retrieve_objects("file", "TestObjectType1");
const o2: object = list2[0];
assert(o2.TestString1 === "This is a test string.",
    "second retrieve must not see face mutation of first");
assert(is_nullish(property_get(o2, "marker", null)),
    "second retrieve must not see marker from first face");

return 0;


//? test: retrieve_objects_query_criteria_eq_double
//? description: Test file adapter retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "eq",
        "property": "TestDouble1",
        "value": 42.0
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestDouble1 === 42.0);

return 0;


//? test: retrieve_objects_query_criteria_ne_double
//? description: Test file adapter retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "ne",
        "property": "TestDouble1",
        "value": 2.0
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestDouble1 === 42.0);

return 0;


//? test: retrieve_objects_query_criteria_double_lt
//? description: Test file adapter retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lt",
        "property": "TestDouble1",
        "value": 43.0
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestDouble1 === 42.0);

return 0;


//? test: retrieve_objects_query_criteria_double_lte
//? description: Test file adapter retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lte",
        "property": "TestDouble1",
        "value": 43.0
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestDouble1 === 42.0);

return 0;


//? test: retrieve_objects_query_criteria_double_lte_eq
//? description: Test file adapter retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lte",
        "property": "TestDouble1",
        "value": 42.0
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestDouble1 === 42.0);

return 0;


//? test: retrieve_objects_query_criteria_double_gt
//? description: Test file adapter retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gt",
        "property": "TestDouble1",
        "value": 41.0
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestDouble1 === 42.0);

return 0;


//? test: retrieve_objects_query_criteria_double_gte
//? description: Test file adapter retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gte",
        "property": "TestDouble1",
        "value": 41.0
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestDouble1 === 42.0);

return 0;


//? test: retrieve_objects_query_criteria_double_gte_eq
//? description: Test file adapter retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gte",
        "property": "TestDouble1",
        "value": 42.0
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestDouble1 === 42.0);

return 0;


//? test: retrieve_objects_query_criteria_eq_object
//? description: Test file adapter retrieve_objects with query criteria eq object.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "eq",
        "property": "TestObject1.prop1",
        "value": "val1"
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestObject1.prop1 === "val1");

return 0;


//? test: retrieve_objects_query_criteria_rql_eq_object
//? description: Test file adapter retrieve_objects with rql query criteria eq object.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "urlEncodedRQLString": "TestObject1.prop1=val1"
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestObject1.prop1 === "val1");

return 0;


//? test: retrieve_objects_query_criteria_rql_eq_percent_value
//? description: file adapter urlEncodedRQLString decodes a valid %31 in the value
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "urlEncodedRQLString": "TestObject1.prop1=val%31"
});

const obj: object = objects[0];
assert(obj !== undefined);
assert(obj.TestObject1.prop1 === "val1");

return 0;


//? test: retrieve_objects_query_criteria_rql_eq_object_no_allowQuery
//? description: Test file adapter retrieve_objects with rql query criteria eq object when allowQuery=false.
//? expect: error:Query string error at offset +0: Property 'TestObject1.prop2' cannot be queried
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "urlEncodedRQLString": "TestObject1.prop2=val2"
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestObject1.prop2 === "val2");

return 0;


//?
//? test: retrieve_objects_to_response-requires-x-afw
//? description: ...
Issue #127: retrieve_objects_to_response is only allowed on a request with
response content type application/x-afw (CLI has no such request).
//? expect: error
//? source: ...

retrieve_objects_to_response("file", "TestObjectType1");


//?
//? test: retrieve_objects_to_stream-progressive
//? description: ...
Issue #127: progressive retrieve_objects_to_stream writes then releases each
object. Smoke — multi-object file retrieve to stdout without UAF/crash; then
materialize count still matches.
//? expect: 0
//? source: ...

// Seed extra instances so progressive path walks more than one object.
add_object("file", "TestObjectType1", {
    "TestString1": "progressive-a",
    "TestDouble1": 1.0
}, "ProgA");
add_object("file", "TestObjectType1", {
    "TestString1": "progressive-b",
    "TestDouble1": 2.0
}, "ProgB");

const sn = stream("stdout");
retrieve_objects_to_stream(sn, "file", "TestObjectType1");

// Still reachable after progressive release path (materialize holds its own refs).
const objects: array = retrieve_objects("file", "TestObjectType1",
    undefined, undefined, undefined, 0);
assert(length(objects) >= 3, "expected seed + ProgA + ProgB");

delete_object("file", "TestObjectType1", "ProgA");
delete_object("file", "TestObjectType1", "ProgB");

return 0;


//?
//? test: retrieve_objects_to_callback-count
//? description: ...
Issue #127 companion: to_callback walks all objects (script may retain; no
CB release on this path yet). Count must match materialize.
//? expect: 0
//? source: ...

add_object("file", "TestObjectType1", {
    "TestString1": "cb-a",
    "TestDouble1": 10.0
}, "CbA");
add_object("file", "TestObjectType1", {
    "TestString1": "cb-b",
    "TestDouble1": 11.0
}, "CbB");

let count: integer = 0;
function cb(theObj: object): boolean {
    count = count + 1;
    assert(theObj !== undefined);
    return false;
}

retrieve_objects_to_callback(cb, null, "file", "TestObjectType1");

const objects: array = retrieve_objects("file", "TestObjectType1",
    undefined, undefined, undefined, 0);
assert(count === length(objects),
    "callback count " + string(count) + " vs array " + string(length(objects)));

delete_object("file", "TestObjectType1", "CbA");
delete_object("file", "TestObjectType1", "CbB");

return 0;