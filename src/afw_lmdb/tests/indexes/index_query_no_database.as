#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_query_no_database.as
//? customPurpose: Part of lmdb tests
//? description: A query on an indexed property scans when the index database does not exist yet.
//? sourceType: script
//?
//? test: index_query_type_never_written
//? description: An index on all object types keeps one database per object type, created by the first write of that type. A query on a type nothing was written to found no database and failed with "Unable to open database" instead of scanning.
//? expect: 1
//? source: ...
#!/usr/bin/env afw

index_create("lmdb", "q", undefined, [], undefined, undefined, false, false);

add_object("lmdb", "TestIndexNoDbWritten", { q: "x" }, generate_uuid());

const not_written = length(retrieve_objects("lmdb", "TestIndexNoDbNeverWritten",
    { "filter": { "op": "eq", "property": "q", "value": "x" } }));
const written = length(retrieve_objects("lmdb", "TestIndexNoDbWritten",
    { "filter": { "op": "eq", "property": "q", "value": "x" } }));

index_remove("lmdb", "q");
assert(not_written === 0, "a query on a type never written should scan and find nothing");
return written;
