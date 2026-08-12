#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: error_codes.as
//? customPurpose: Part of core error-code tests
//? description: _AdaptiveError_.id for script, language, and adapter codes
//? sourceType: script
//?
//? test: throw-id
//? description: script throw sets id throw
//? expect: 0
//? source: ...

try {
    throw "x" { k: 1 };
}
catch (e) {
    assert(e.id === "throw", e.id);
    assert(e.message === "x");
    assert(e.data.k === 1);
}
return 0;

//?
//? test: assertion-id
//? description: assert failure is assertion_failed
//? expect: 0
//? source: ...

try {
    assert(false, "nope");
}
catch (e) {
    assert(e.id === "assertion_failed", e.id);
}
return 0;

//?
//? test: arg-error-divide-by-zero
//? description: integer divide by zero is arg_error
//? expect: 0
//? source: ...

try {
    let x = 1 / 0;
}
catch (e) {
    assert(e.id === "arg_error", e.id);
}
return 0;

//?
//? test: syntax-id
//? description: Adaptive parse error is syntax
//? expect: 0
//? source: ...

try {
    compile<script>(script("let x ="));
}
catch (e) {
    assert(e.id === "syntax", e.id);
}
return 0;

//?
//? test: not-found-object
//? description: missing runtime object is not_found
//? expect: 0
//? source: ...

try {
    get_object("afw", "_AdaptiveObjectType_", "no-such-object");
}
catch (e) {
    assert(e.id === "not_found", e.id);
}
return 0;

//?
//? test: not-found-adapter
//? description: missing adapter is not_found
//? expect: 0
//? source: ...

try {
    get_object("no-such-adapter", "Type", "id");
}
catch (e) {
    assert(e.id === "not_found", e.id);
}
return 0;

//?
//? test: method-not-supported-journal-retrieve
//? description: retrieve of journal entries is method_not_supported
//? expect: 0
//? source: ...

try {
    retrieve_objects("afw", "_AdaptiveJournalEntry_");
}
catch (e) {
    assert(e.id === "method_not_supported", e.id);
}
return 0;
