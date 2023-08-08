#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: retrieve_objects.as
//? customPurpose: Part of file adaptor tests
//? description: Test file adaptor retrieve_objects.
//? sourceType: script
//?
//? test: retrieve_objects_test-1
//? description: Test file adaptor retrieve_objects.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const objects: array = retrieve_objects("file", "TestObjectType1");

// we should have one object to start with
const obj: object = objects[0];
assert(obj !== undefined);

// make sure the mapped property exists
assert(obj.TestString1 === "This is a test string.");

return 0;

//? test: retrieve_objects_query_criteria_eq_string
//? description: Test file adaptor retrieve_objects with query criteria.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

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


//? test: retrieve_objects_query_criteria_eq_integer
//? description: Test file adaptor retrieve_objects with query criteria.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

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

//? test: retrieve_objects_query_criteria_and
//? description: Test file adaptor retrieve_objects with query criteria.
//? skip: false
//? expect: 0
//? source: ...
#!/usr/bin/env afw

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