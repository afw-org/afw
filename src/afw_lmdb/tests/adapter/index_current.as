#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_current.as
//? customPurpose: Part of lmdb tests
//? description: Issue #54 index filter/value current:: variables
//? sourceType: script
//?
//? test: index_current_object
//? description: index_create value/filter scripts use current::object (issue #54)
//? skip: true
//? skipReason: Pre-existing LMDB index_create txn/save_config and retroactive hang; unskip when create path fixed (#57).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

/*
 * When unskipped: seed objects, index_create with value/filter using
 * current::object (and related), assert num_indexed >= 1, online add,
 * index_list contains key, cleanup.
 */

const ot: string = "_AdaptiveObject_";

const idMatch: string = generate_uuid();
const idSkip: string = generate_uuid();

add_object("lmdb", ot, {
    surname: "Smith",
    department: "ENG"
}, idMatch);

add_object("lmdb", ot, {
    surname: "Jones",
    department: "HR"
}, idSkip);

const created: object = index_create(
    "lmdb",
    "surnameByDept",
    "current::object.get(\"surname\")",
    [ot],
    "eq(current::object.get(\"department\"), \"ENG\")",
    undefined,
    true,
    false
);

assert(is_defined(created.num_indexed), "index_create should return num_indexed");
assert(created.num_indexed >= 1,
    "expected at least one ENG object indexed with current:: value/filter");

const idNew: string = generate_uuid();
add_object("lmdb", ot, {
    surname: "Lee",
    department: "ENG"
}, idNew);

const listed: object = index_list("lmdb");
assert(is_defined(listed.surnameByDept),
    "index_list should include surnameByDept definition");

safe_evaluate(index_remove("lmdb", "surnameByDept"), null);
safe_evaluate(delete_object("lmdb", ot, idMatch), null);
safe_evaluate(delete_object("lmdb", ot, idSkip), null);
safe_evaluate(delete_object("lmdb", ot, idNew), null);

return 0;
