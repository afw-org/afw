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
//? test: throw-data-clause
//? description: throw data clause sets e.data
//? expect: 0
//? source: ...

try {
    throw "x" data { k: 1 };
}
catch (e) {
    assert(e.id === "throw", e.id);
    assert(e.data.k === 1);
}
return 0;

//?
//? test: throw-id-not-found
//? description: throw id not_found sets e.id
//? expect: 0
//? source: ...

try {
    throw "missing person" id "not_found";
}
catch (e) {
    assert(e.id === "not_found", e.id);
    assert(e.message === "missing person");
    assert(is_nullish(e.data));
}
return 0;

//?
//? test: throw-data-and-id
//? description: throw data and id in either order
//? expect: 0
//? source: ...

try {
    throw "missing" data { personId: "p1" } id "not_found";
}
catch (e) {
    assert(e.id === "not_found", e.id);
    assert(e.data.personId === "p1");
}

try {
    throw "no" id "denied" data { reason: "acl" };
}
catch (e) {
    assert(e.id === "denied", e.id);
    assert(e.data.reason === "acl");
}
return 0;

//?
//? test: throw-id-conflict
//? description: throw id conflict
//? expect: 0
//? source: ...

try {
    throw "already exists" id "conflict";
}
catch (e) {
    assert(e.id === "conflict", e.id);
}
return 0;

//?
//? test: throw-id-not-allowed-literal
//? description: throw id memory as a string literal is not allowed
//? expect: error
//? source: ...

throw "no" id "memory";

//?
//? test: throw-id-not-allowed-value
//? description: throw id from a variable that is not allowed
//? expect: error:id 'memory' is not allowed on throw
//? source: ...

const name: string = "memory";
throw "no" id name;

//?
//? test: throw-id-unknown
//? description: throw id unknown name as a string literal is not allowed
//? expect: error
//? source: ...

throw "no" id "nope";

//?
//? test: throw-duplicate-id
//? description: throw id twice is a parse error
//? expect: error
//? source: ...

throw "no" id "not_found" id "denied";

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
