#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_number_option.as
//? customPurpose: Part of lmdb tests
//? description: The "integer" and "double" index options say an index's values are numbers when no object type does, so a query's string value (a query string's are all strings) seeks the number's key (issue #544).
//? sourceType: script
//?
//? test: index_integer_option_untyped
//? description: Objects with no object type and an "integer" index. A string value sought the string's key among the integers' sortable keys - gt "9" found nothing, lt "10" found everything - while a scan converts per object.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexIntegerOption";

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

function count(qs: string): integer {
    return length(retrieve_objects("lmdb", ot, { urlEncodedRQLString: qs }));
}

add_object("lmdb", ot, { t: "a", n_io: -3, k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "b", n_io: 2, k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "c", n_io: 9, k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "d", n_io: 10, k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "e", n_io: 100, k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "f", k: "x" }, generate_uuid());

index_create("lmdb", "n_io", undefined, [ot], undefined, ["integer"], true, false);

check({ op: "gt", property: "n_io", value: "9" }, "d,e", "gt \"9\"");
check({ op: "lt", property: "n_io", value: "10" }, "a,b,c", "lt \"10\"");
check({ op: "eq", property: "n_io", value: "9" }, "c", "eq \"9\"");
check({ op: "ge", property: "n_io", value: "-3" }, "a,b,c,d,e", "ge \"-3\"");
check({ op: "gt", property: "n_io", value: 9 }, "d,e", "gt 9 (a number)");
check({ op: "eq", property: "n_io", value: "x" }, "", "eq a string that is not a number");
check({ op: "lt", property: "n_io", value: "x" }, "", "lt a string that is not a number");

assert(count("n_io=gt=9") === 2, "query string n_io=gt=9");
assert(count("n_io=lt=10") === 3, "query string n_io=lt=10");
assert(count("n_io=-3") === 1, "query string n_io=-3");

safe_evaluate(index_remove("lmdb", "n_io"), null);

return 0;


//? test: index_double_option_untyped
//? description: Objects with no object type and a "double" index. A string value is a double's key when it is a double ("9.0"; double("9") is not a double), and an integer filter value is the double of the same number, as a scan converts it.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexDoubleOption";

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

add_object("lmdb", ot, { t: "a", n_do: 1.5, k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "b", n_do: 9.0, k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "c", n_do: 9.5, k: "x" }, generate_uuid());
add_object("lmdb", ot, { t: "d", n_do: 10.25, k: "x" }, generate_uuid());

index_create("lmdb", "n_do", undefined, [ot], undefined, ["double"], true, false);

check({ op: "ge", property: "n_do", value: "9.0" }, "b,c,d", "ge \"9.0\"");
check({ op: "lt", property: "n_do", value: "9.5" }, "a,b", "lt \"9.5\"");
check({ op: "eq", property: "n_do", value: "9.0" }, "b", "eq \"9.0\"");
check({ op: "ge", property: "n_do", value: 9 }, "b,c,d", "ge 9 (an integer)");
check({ op: "lt", property: "n_do", value: 9.5 }, "a,b", "lt 9.5 (a double)");
check({ op: "ge", property: "n_do", value: "9" }, "", "ge \"9\" (not a double)");
check({ op: "lt", property: "n_do", value: "x" }, "", "lt a string that is not a number");

safe_evaluate(index_remove("lmdb", "n_do"), null);

return 0;


//? test: index_integer_option_computed
//? description: A computed name can't be declared in an object type (issue #516), so the option is how its index says its values are numbers.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexIntegerOptionComputed";

add_object("lmdb", ot, { age: 5 }, generate_uuid());
add_object("lmdb", ot, { age: 20 }, generate_uuid());

index_create("lmdb", "AgeYears_ic", "current::object.age", [ot], undefined,
    ["integer"], true, false);

const older: array = retrieve_objects("lmdb", ot,
    { urlEncodedRQLString: "AgeYears_ic=gt=9" });
assert(length(older) === 1 && older[0].age === 20, "AgeYears_ic=gt=9");

safe_evaluate(index_remove("lmdb", "AgeYears_ic"), null);

return 0;


//? test: index_integer_option_other_value
//? description: A value that is not an integer, in an "integer" index, is its own key; a query finds it by that value, and a query value that is no integer compares only with such values.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexIntegerOptionOther";

add_object("lmdb", ot, { n_iov: 5 }, generate_uuid());
add_object("lmdb", ot, { n_iov: "abc" }, generate_uuid());

index_create("lmdb", "n_iov", undefined, [ot], undefined, ["integer"], true, false);

const text: array = retrieve_objects("lmdb", ot,
    { filter: { op: "eq", property: "n_iov", value: "abc" } });
assert(length(text) === 1 && text[0].n_iov === "abc", "eq \"abc\"");

const five: array = retrieve_objects("lmdb", ot,
    { filter: { op: "eq", property: "n_iov", value: "5" } });
assert(length(five) === 1 && five[0].n_iov === 5, "eq \"5\"");

/* "b" is no integer: 5 doesn't compare with it, "abc" does. */
const below: array = retrieve_objects("lmdb", ot,
    { filter: { op: "lt", property: "n_iov", value: "b" } });
assert(length(below) === 1 && below[0].n_iov === "abc", "lt \"b\"");

safe_evaluate(index_remove("lmdb", "n_iov"), null);

return 0;


//? test: index_number_option_rules
//? description: index_create refuses both options, and an option that contradicts the data type an object type declares.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexNumberOptionRules";
let message: string;

add_object("lmdb", "_AdaptiveObjectType_", {
    propertyTypes: {
        n_nr: { dataType: "integer", allowQuery: true }
    }
}, ot);

message = "";
try {
    index_create("lmdb", "m_nr", undefined, [ot], undefined,
        ["integer", "double"], false, false);
}
catch (e) {
    message = e.message;
}
assert(includes(message, "only one"), "integer and double: " + message);

message = "";
try {
    index_create("lmdb", "n_nr", undefined, [ot], undefined,
        ["double"], false, false);
}
catch (e) {
    message = e.message;
}
assert(includes(message, "declares"), "double on an integer property: " + message);

assert(is_nullish(index_list("lmdb").m_nr), "m_nr was not created");
assert(is_nullish(index_list("lmdb").n_nr), "n_nr was not created");

/* The option that agrees is fine. */
index_create("lmdb", "n_nr", undefined, [ot], undefined, ["integer"], false, false);
index_remove("lmdb", "n_nr");

delete_object("lmdb", "_AdaptiveObjectType_", ot);

return 0;
