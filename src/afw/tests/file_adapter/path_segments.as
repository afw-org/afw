#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: path_segments.as
//? customPurpose: Part of file adapter tests
//? description: ...
An object type id or object id is one file name under the adapter
root. "." or "..", or a '/' or '\', is rejected before any file is
opened, read, or written.
//? sourceType: script
//?
//? test: get-id-parent
//? description: get_object with a ../ object id
//? expect: error:File adapter object id can not contain '/', '\', or NUL
//? source: ...

get_object("file", "TestObjectType1", "../TestObjectType1/Test1")

//?
//? test: get-type-parent
//? description: get_object with a .. object type id
//? expect: error:File adapter object type id is not a valid file name
//? source: ...

get_object("file", "..", "afw")

//?
//? test: add-type-parent
//? description: add_object with a .. object type id
//? expect: error:File adapter object type id is not a valid file name
//? source: ...

add_object("file", "..", { "x": 1 }, "path_segments_should_not_exist")

//?
//? test: replace-id-slash
//? description: replace_object with a / in the object id
//? expect: error:File adapter object id can not contain '/', '\', or NUL
//? source: ...

replace_object("file", "TestObjectType1", "a/b", { "x": 1 })

//?
//? test: retrieve-type-slash
//? description: retrieve_objects with a / in the object type id
//? expect: error:File adapter object type id can not contain '/', '\', or NUL
//? source: ...

retrieve_objects("file", "TestObjectType1/../TestObjectType1")

//?
//? test: plain-id-ok
//? description: a plain id still works
//? expect: 0
//? source: ...

const o = get_object("file", "TestObjectType1", "Test1");
assert(o !== undefined);
return 0;
