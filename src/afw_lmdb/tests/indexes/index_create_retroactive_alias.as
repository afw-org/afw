#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_create_retroactive_alias.as
//? customPurpose: Part of lmdb tests
//? description: A retroactive index_create indexes objects that have a non-UUID id.
//? sourceType: script
//?
//? test: index_create_retroactive_alias
//? description: The retroactive scan reports an object by its alias id. The index entry needs the internal uuid; the alias used to be parsed as a uuid ("Invalid uuid string.").
//? expect: "2 1 1"
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexRetroAliasType";
const q = function (v: string): integer {
    return length(retrieve_objects("lmdb", ot,
        { "filter": { "op": "eq", "property": "s", "value": v } }));
};

add_object("lmdb", ot, { s: "a" }, "retro-alias-1");
add_object("lmdb", ot, { s: "b" }, generate_uuid());

const r = index_create("lmdb", "s", undefined, [ot], undefined, undefined,
    true, false);
const result = string(r.num_indexed) + " " + string(q("a")) + " " +
    string(q("b"));

index_remove("lmdb", "s");
return result;
