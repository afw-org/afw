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
//? description: Test file adaptor retrieve_objects with query criteria eq string.
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


//? test: retrieve_objects_query_criteria_rql_eq_string
//? description: Test file adaptor retrieve_objects with query criteria eq string, using rql.
//? expect: 0
//? source: ...

/*
const urlEncoded = url_encode("TestString1=This is a test string.");
const criteria = {
    "urlEncodedRQLString": urlEncoded
};
*/

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "urlEncodedRQLString": "TestString1=This%20is%20a%20test%20string."
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;


//? test: retrieve_objects_query_criteria_ne_string
//? description: Test file adaptor retrieve_objects with query criteria ne string.
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
//? description: Test file adaptor retrieve_objects with query criteria lt string.
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


//? test: retrieve_objects_query_criteria_lt_string_none
//? description: Test file adaptor retrieve_objects with query criteria lt string (no match).
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lt",
        "property": "TestString1",
        "value": "This is a test string."
    }
});

// make sure we have no entries
assert(length(objects) === 0);

return 0;


//? test: retrieve_objects_query_criteria_lte_string_eq
//? description: Test file adaptor retrieve_objects with query criteria lte string (equals).
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


//? test: retrieve_objects_query_criteria_lte_string_lt
//? description: Test file adaptor retrieve_objects with query criteria lte string (less than).
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lte",
        "property": "TestString1",
        "value": "ahis is a test string."
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;


//? test: retrieve_objects_query_criteria_lte_string_none
//? description: Test file adaptor retrieve_objects with query criteria lte string (no match).
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "lte",
        "property": "TestString1",
        "value": "THis is a test string."
    }
});

assert(length(objects) === 0);

return 0;


//? test: retrieve_objects_query_criteria_gt_string_gt
//? description: Test file adaptor retrieve_objects with query criteria gt string (greater than).
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


//? test: retrieve_objects_query_criteria_gt_string_none
//? description: Test file adaptor retrieve_objects with query criteria gt string (no match).
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gt",
        "property": "TestString1",
        "value": "this is a test string."
    }
});

assert(length(objects) === 0);

return 0;


//? test: retrieve_objects_query_criteria_gte_string_gt
//? description: Test file adaptor retrieve_objects with query criteria gt string (greater than).
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
//? description: Test file adaptor retrieve_objects with query criteria gte string (equals).
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


//? test: retrieve_objects_query_criteria_gte_string_none
//? description: Test file adaptor retrieve_objects with query criteria gte string (no match).
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "gte",
        "property": "TestString1",
        "value": "this is a test string."
    }
});

assert(length(objects) === 0);

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


//? test: retrieve_objects_query_criteria_contains_array
//? description: Test file adaptor retrieve_objects with query criteria contains array.
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "contains",
        "property": "TestArray1",
        "value": "one"
    }
});

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestArray1[0] === "one");
assert(obj.TestArray1[1] === "two");

return 0;


//? test: retrieve_objects_query_criteria_contains_array_none
//? description: Test file adaptor retrieve_objects with query criteria contains array (no match).
//? expect: 0
//? source: ...

const objects: array = retrieve_objects("file", "TestObjectType1", {
    "filter": {
        "op": "contains",
        "property": "TestArray1",
        "value": "three"
    }
});

// we should have one object to start with
assert(length(objects) === 0);

return 0;


//? test: retrieve_objects_query_criteria_eq_object
//? description: Test file adaptor retrieve_objects with query criteria eq object.
//? expect: 0
//? skip: true
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
//? description: Test file adaptor retrieve_objects with rql query criteria eq object.
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
//? description: Test file adaptor retrieve_objects with rql query criteria eq object when allowQuery=false.
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