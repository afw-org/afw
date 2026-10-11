#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_computed_meaning.as
//? customPurpose: Part of lmdb tests
//? description: A computed index (one with a value script, and maybe a filter) defines what its name means in a query. Every plan gives the same answer - the index alone, the index with every object re-tested, and a scan (issue #516).
//? sourceType: script
//?
//? test: index_computed_name_every_plan
//? description: A synthetic name whose value script reads another property. The re-test and the scan used to read the name from the object, find nothing, and drop it; an or of two cursors on the name returned an object twice (issue #516).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexComputedName";

/* The t of each object a filter returns, sorted, joined with ",". */
function tags(filter: object): string {
    const t: array = map(function (o: object): string { return o.t; },
        retrieve_objects("lmdb", ot, { filter: filter }));
    if (length(t) === 0) {
        return "";
    }
    return join(sort(function (a: string, b: string): boolean {
        return a < b;
    }, t), ",");
}

/*
 * The filter as given, and with an unindexed term that every object
 * passes (and: the index answers, every object is re-tested) or fails
 * (or: not sargable, so a scan), must all return expected.
 */
function check(filter: object, expected: string, what: string): void {
    const yes: object = { op: "eq", property: "k", value: "x" };
    const no: object = { op: "eq", property: "k", value: "never" };
    let got: string;

    got = tags(filter);
    assert(got === expected, what + " (index): " + got);
    got = tags({ op: "and", filters: [filter, yes] });
    assert(got === expected, what + " (index, re-tested): " + got);
    got = tags({ op: "or", filters: [filter, no] });
    assert(got === expected, what + " (scan): " + got);
}

add_object("lmdb", ot, { t: "a", given: "Ada", Dept: "ENG", k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "b", given: "Alan", Dept: "MATH", k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "c", Dept: "ENG", k: "x" }, generate_uuid());

index_create("lmdb", "FullName_cn", "current::object.given", [ot], undefined, undefined, true, false);
index_create("lmdb", "Dept_cn", "current::object.Dept", [ot], undefined, undefined, true, false);

const Ada: object = { op: "eq", property: "FullName_cn", value: "Ada" };
const Alan: object = { op: "eq", property: "FullName_cn", value: "Alan" };

check(Ada, "a", "eq");
check({ op: "ne", property: "FullName_cn", value: "Ada" }, "b", "ne (c has no value)");
check({ op: "ge", property: "FullName_cn", value: "Al" }, "b", "ge");
check({ op: "lt", property: "FullName_cn", value: "Al" }, "a", "lt");
check({ op: "match", property: "FullName_cn", value: "Al.*" }, "b", "match starts with");
check({ op: "or", filters: [Ada, Alan] }, "a,b", "or of two names");
check({ op: "and", filters: [Ada, { op: "eq", property: "Dept_cn", value: "ENG" }] },
    "a", "and of two computed names");
check({ op: "or", filters: [Ada, { op: "eq", property: "Dept_cn", value: "MATH" }] },
    "a,b", "or of two computed names");
check({ op: "and", filters: [Ada, { op: "eq", property: "given", value: "Ada" }] },
    "a", "and with an unindexed real property");
check({ op: "or", filters: [Ada, { op: "ge", property: "FullName_cn", value: "A" }] },
    "a,b", "or of two cursors on one name: no duplicate");

safe_evaluate(index_remove("lmdb", "FullName_cn"), null);
safe_evaluate(index_remove("lmdb", "Dept_cn"), null);

return 0;


//? test: index_computed_filter_every_plan
//? description: An index with a filter defines its name only for the objects the filter passes. An object the filter leaves out has no value under that name, in every plan.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexComputedFilter";

function tags(filter: object): string {
    const t: array = map(function (o: object): string { return o.t; },
        retrieve_objects("lmdb", ot, { filter: filter }));
    if (length(t) === 0) {
        return "";
    }
    return join(sort(function (a: string, b: string): boolean {
        return a < b;
    }, t), ",");
}

function check(filter: object, expected: string, what: string): void {
    const yes: object = { op: "eq", property: "k", value: "x" };
    const no: object = { op: "eq", property: "k", value: "never" };
    let got: string;

    got = tags(filter);
    assert(got === expected, what + " (index): " + got);
    got = tags({ op: "and", filters: [filter, yes] });
    assert(got === expected, what + " (index, re-tested): " + got);
    got = tags({ op: "or", filters: [filter, no] });
    assert(got === expected, what + " (scan): " + got);
}

add_object("lmdb", ot, { t: "a", surname: "Smith", department: "ENG", k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "b", surname: "Smith", department: "HR", k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "c", surname: "Jones", department: "ENG", k: "x" }, generate_uuid());

index_create("lmdb", "surnameByDept_cf",
    "current::object.surname", [ot],
    "return current::object.department == \"ENG\";",
    undefined, true, false);

check({ op: "eq", property: "surnameByDept_cf", value: "Smith" }, "a", "eq");
check({ op: "ne", property: "surnameByDept_cf", value: "Smith" }, "c", "ne (b has no value)");
check({ op: "ge", property: "surnameByDept_cf", value: "A" }, "a,c", "ge");
check({ op: "or", filters: [
    { op: "eq", property: "surnameByDept_cf", value: "Smith" },
    { op: "eq", property: "surname", value: "Jones" }
] }, "a,c", "or with an unindexed real property");

safe_evaluate(index_remove("lmdb", "surnameByDept_cf"), null);

return 0;


//? test: index_computed_after_remove
//? description: After index_remove, a computed name is just a property objects don't have; a query on it finds nothing.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexComputedRemoved";

add_object("lmdb", ot, { t: "a", given: "Ada" }, generate_uuid());
index_create("lmdb", "FullName_cr", "current::object.given", [ot], undefined, undefined, true, false);

const before: array = retrieve_objects("lmdb", ot,
    { filter: { op: "eq", property: "FullName_cr", value: "Ada" } });
assert(length(before) === 1, "with the index");

index_remove("lmdb", "FullName_cr");

const after: array = retrieve_objects("lmdb", ot,
    { filter: { op: "eq", property: "FullName_cr", value: "Ada" } });
assert(length(after) === 0, "after index_remove");

return 0;
