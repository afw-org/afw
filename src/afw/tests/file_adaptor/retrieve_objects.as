#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: retrieve_objects.as
//? customPurpose: Part of file adaptor tests
//? description: Test file adaptor retrieve_objects.
//? sourceType: script
//?
//? test: retrieve_objects_test-1
//? description: Test file adaptor retrieve_objects.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1");

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;

//? test: retrieve_objects_query_criteria_eq_string
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "eq",
        "property": "TestString1",
        "value": "This is a test string."
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;


//? test: retrieve_objects_query_criteria_ne_string
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "ne",
        "property": "TestString1",
        "value": "xyz"
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;


//? test: retrieve_objects_query_criteria_lt_string
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lt",
        "property": "TestString1",
        "value": "this is a test string."
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;


//? test: retrieve_objects_query_criteria_lte_string
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lt",
        "property": "TestString1",
        "value": "this is a test string."
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;


//? test: retrieve_objects_query_criteria_lte_string_eq
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lte",
        "property": "TestString1",
        "value": "This is a test string."
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;


//? test: retrieve_objects_query_criteria_gt_string
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gt",
        "property": "TestString1",
        "value": "THis is a test string."
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;


//? test: retrieve_objects_query_criteria_gte_string
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gte",
        "property": "TestString1",
        "value": "THis is a test string."
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;


//? test: retrieve_objects_query_criteria_gte_string_eq
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gte",
        "property": "TestString1",
        "value": "This is a test string."
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;


//? test: retrieve_objects_query_criteria_eq_integer
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "eq",
        "property": "TestInteger1",
        "value": 42
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_ne_integer
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "ne",
        "property": "TestInteger1",
        "value": 2
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_and
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "and",
        "filters": [
            {
                "op": "eq",
                "property": "TestString1",
                "value": "This is a test string."
            },
            {
                "op": "eq",
                "property": "TestInteger1",
                "value": 42
            }
        ]
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_eq_double
//? description: Test file adaptor retrieve_objects with query criteria.
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
//? description: Test file adaptor retrieve_objects with query criteria.
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
//? description: Test file adaptor retrieve_objects with query criteria.
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
//? description: Test file adaptor retrieve_objects with query criteria.
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
//? description: Test file adaptor retrieve_objects with query criteria.
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
//? description: Test file adaptor retrieve_objects with query criteria.
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
//? description: Test file adaptor retrieve_objects with query criteria.
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
//? description: Test file adaptor retrieve_objects with query criteria.
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


//? test: retrieve_objects_query_criteria_integer_eq
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "eq",
        "property": "TestInteger1",
        "value": 42
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_integer_ne
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "ne",
        "property": "TestInteger1",
        "value": 2
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_integer_lt
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lt",
        "property": "TestInteger1",
        "value": 43
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_integer_lte
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lte",
        "property": "TestInteger1",
        "value": 43
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_integer_lte_eq
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lte",
        "property": "TestInteger1",
        "value": 42
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_integer_gt
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gt",
        "property": "TestInteger1",
        "value": 41
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_integer_gte
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gte",
        "property": "TestInteger1",
        "value": 41
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_integer_gte_eq
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gte",
        "property": "TestInteger1",
        "value": 42
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestInteger1 === 42);

return 0;


//? test: retrieve_objects_query_criteria_boolean_eq
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "eq",
        "property": "TestBoolean1",
        "value": true
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestBoolean1 === true);

return 0;


//? test: retrieve_objects_query_criteria_boolean_ne
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "ne",
        "property": "TestBoolean1",
        "value": false
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestBoolean1 === true);

return 0;


//? test: retrieve_objects_query_criteria_eq_mismatch
//? description: Test file adaptor retrieve_objects with query criteria.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "eq",
        "property": "TestString1",
        "value": true
    }
});

// we should have one object to start with
assert(length(objects) === 0);

return 0;