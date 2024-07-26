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