#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_empty_string.as
//? customPurpose: Part of lmdb tests
//? description: The empty string is an index key like any other value (issue #544).
//? sourceType: script
//?
//? test: index_empty_string_every_plan
//? description: An empty string was never written to an index (LMDB keys can't be empty), so eq "", lt / le walking down and ge "" missed objects with "" when the index answered. Its key is now "\0", and a value that starts with "\0" gets another one in front, so every plan agrees.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexEmptyString";

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

/* In byte order: "" < "\u0000" < "\u0000x" < "a" < "b". f has no s. */
const idA: string = generate_uuid();
add_object("lmdb", ot, { t: "a", s_es: "", k: "x" }, idA);
add_object("lmdb", ot, { t: "b", s_es: "\u0000", k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "c", s_es: "\u0000x", k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "d", s_es: "a", k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "e", s_es: "b", k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "f", k: "x" }, generate_uuid());

index_create("lmdb", "s_es", undefined, [ot], undefined, undefined, true, false);

check({ op: "eq", property: "s_es", value: "" }, "a", "eq \"\"");
check({ op: "eq", property: "s_es", value: "\u0000" }, "b", "eq \"\\u0000\" is not \"\"");
check({ op: "lt", property: "s_es", value: "a" }, "a,b,c", "lt walks down to \"\"");
check({ op: "le", property: "s_es", value: "\u0000" }, "a,b", "le \"\\u0000\"");
check({ op: "le", property: "s_es", value: "b" }, "a,b,c,d,e", "le");
check({ op: "ge", property: "s_es", value: "" }, "a,b,c,d,e", "ge \"\"");
check({ op: "gt", property: "s_es", value: "" }, "b,c,d,e", "gt \"\"");
check({ op: "lt", property: "s_es", value: "\u0000x" }, "a,b", "lt \"\\u0000x\"");
check({ op: "or", filters: [
    { op: "eq", property: "s_es", value: "" },
    { op: "le", property: "s_es", value: "a" }
] }, "a,b,c,d", "or of two cursors on one name: no duplicate");

/* Index maintenance: a value changed from "" is gone from its key. */
modify_object("lmdb", ot, idA, [["set_property", "s_es", "z"]]);
check({ op: "eq", property: "s_es", value: "" }, "", "eq \"\" after modify");

safe_evaluate(index_remove("lmdb", "s_es"), null);

return 0;


//? test: index_empty_string_case_insensitive
//? description: The empty string in a case-insensitive index.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexEmptyStringCi";

add_object("lmdb", ot, { s_esci: "" }, generate_uuid());
add_object("lmdb", ot, { s_esci: "A" }, generate_uuid());

index_create("lmdb", "s_esci", undefined, [ot], undefined,
    ["case-insensitive-string"], true, false);

const empty: array = retrieve_objects("lmdb", ot,
    { filter: { op: "eq", property: "s_esci", value: "" } });
assert(length(empty) === 1, "eq \"\"");

const below: array = retrieve_objects("lmdb", ot,
    { filter: { op: "lt", property: "s_esci", value: "a" } });
assert(length(below) === 1, "lt \"a\"");

safe_evaluate(index_remove("lmdb", "s_esci"), null);

return 0;
