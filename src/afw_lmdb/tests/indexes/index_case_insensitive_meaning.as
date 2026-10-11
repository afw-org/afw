#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_case_insensitive_meaning.as
//? customPurpose: Part of lmdb tests
//? description: A "case-insensitive-string" index makes its name compare case-insensitively in a query, whatever the plan - the index alone, the index with every object re-tested, or a scan (issue #516).
//? sourceType: script
//?
//? test: index_case_insensitive_every_plan
//? description: The re-test and the scan compared case-sensitively, so and(s eq "SMITH", ...) and or(s eq "SMITH", ...) found nothing while s eq "SMITH" alone found both (issue #516 comment).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexCiMeaning";

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

add_object("lmdb", ot, { t: "a", s_cim: "Smith", n: 1, k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "b", s_cim: "smith", n: 2, k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "c", s_cim: "Jones", n: 3, k: "x" }, generate_uuid());

index_create("lmdb", "s_cim", undefined, [ot], undefined,
    ["case-insensitive-string"], true, false);

check({ op: "eq", property: "s_cim", value: "SMITH" }, "a,b", "eq");
check({ op: "ne", property: "s_cim", value: "SMITH" }, "c", "ne");
check({ op: "lt", property: "s_cim", value: "k" }, "c",
    "lt (\"Smith\" < \"k\" only when case counts)");
check({ op: "ge", property: "s_cim", value: "S" }, "a,b", "ge");
check({ op: "gt", property: "s_cim", value: "SMITH" }, "", "gt");
check({ op: "in", property: "s_cim", value: ["SMITH", "jones"] }, "a,b,c", "in");
check({ op: "and", filters: [
    { op: "eq", property: "s_cim", value: "SMITH" },
    { op: "ge", property: "n", value: 1 }
] }, "a,b", "and with an unindexed property");
check({ op: "or", filters: [
    { op: "eq", property: "s_cim", value: "SMITH" },
    { op: "ge", property: "s_cim", value: "a" }
] }, "a,b,c", "or of two cursors on one name: no duplicate");

/*
 * match is case-insensitive too: the pattern is lowercased like the
 * values, except its escapes (\S is not \s).
 */
check({ op: "match", property: "s_cim", value: "smi.*" }, "a,b", "match, lowercase pattern");
check({ op: "match", property: "s_cim", value: "SMI.*" }, "a,b", "match, uppercase pattern");
check({ op: "match", property: "s_cim", value: "[S]\\S+" }, "a,b", "match, class and \\S escape");
check({ op: "match", property: "s_cim", value: "J\\s*ONES" }, "c", "match, \\s escape");
check({ op: "match", property: "s_cim", value: "\\p{Lu}.*" }, "", "match, \\p{Lu} never matches a lowercased value");

safe_evaluate(index_remove("lmdb", "s_cim"), null);

return 0;
