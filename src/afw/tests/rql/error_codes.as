#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: error_codes.as
//? customPurpose: Part of rql tests
//? description: A query the client got wrong is syntax or bad_request (HTTP 400), not general (#573)
//? sourceType: script
//?
//? test: object-missing-op
//? description: A filter object with no op is bad_request
//? expect: "bad_request"
//? source: ...

try {
    retrieve_objects("afw", "_AdaptiveFunction_", {
        "filter": { "property": "category", "value": "string" }
    });
}
catch (e) {
    return e.id;
}
return "no error";

//? test: object-bad-op
//? description: A filter object with an unknown op is bad_request
//? expect: "bad_request"
//? source: ...

try {
    retrieve_objects("afw", "_AdaptiveFunction_", {
        "filter": { "op": "nope", "property": "category", "value": "string" }
    });
}
catch (e) {
    return e.id;
}
return "no error";

//? test: object-no-value
//? description: A filter object with no value is bad_request
//? expect: "bad_request"
//? source: ...

try {
    retrieve_objects("afw", "_AdaptiveFunction_", {
        "filter": { "op": "ne", "property": "category" }
    });
}
catch (e) {
    return e.id;
}
return "no error";

//? test: object-unknown-property
//? description: An unknown query criteria property is bad_request
//? expect: "bad_request"
//? source: ...

try {
    retrieve_objects("afw", "_AdaptiveFunction_", { "nope": true });
}
catch (e) {
    return e.id;
}
return "no error";

//? test: object-bad-sort
//? description: A sort name without + or - is bad_request
//? expect: "bad_request"
//? source: ...

try {
    retrieve_objects("afw", "_AdaptiveFunction_", { "sort": ["category"] });
}
catch (e) {
    return e.id;
}
return "no error";

//? test: object-bad-regexp
//? description: A match value that is not a regexp is syntax
//? expect: "syntax"
//? source: ...

try {
    retrieve_objects("afw", "_AdaptiveFunction_", {
        "filter": { "op": "match", "property": "category", "value": "(" }
    });
}
catch (e) {
    return e.id;
}
return "no error";

//? test: object-sort-one-letter
//? description: A sort on a one-letter property name is accepted
//? expect: true
//? source: ...

const r = retrieve_objects("afw", "_AdaptiveFunction_", {
    "sort": ["+a"], "select": ["functionId"]
});
return length(r) > 0;

//? test: string-sort-twice
//? description: sort() twice in a query string is syntax
//? expect: "syntax"
//? source: ...

try {
    retrieve_objects("afw", "_AdaptiveFunction_",
        { "urlEncodedRQLString": "sort(+category)&sort(-category)" });
}
catch (e) {
    return e.id;
}
return "no error";

//? test: string-select-twice
//? description: select() twice in a query string is syntax
//? expect: "syntax"
//? source: ...

try {
    retrieve_objects("afw", "_AdaptiveFunction_",
        { "urlEncodedRQLString": "select(category)&select(functionId)" });
}
catch (e) {
    return e.id;
}
return "no error";

//? test: string-bad-relation
//? description: A malformed relation in a query string is syntax
//? expect: "syntax"
//? source: ...

try {
    retrieve_objects("afw", "_AdaptiveFunction_",
        { "urlEncodedRQLString": "category=xx=1" });
}
catch (e) {
    return e.id;
}
return "no error";

//? test: convert-bad-style
//? description: An out of range style is argument_error
//? expect: "argument_error"
//? source: ...

try {
    convert_AdaptiveQueryCriteria_to_query_string(
        { "filter": { "op": "eq", "property": "a", "value": "b" } },
        undefined, undefined, 99);
}
catch (e) {
    return e.id;
}
return "no error";

//? test: convert-object-type-without-adapter
//? description: objectType without adapterId is argument_error
//? expect: "argument_error"
//? source: ...

try {
    convert_AdaptiveQueryCriteria_to_query_string(
        { "filter": { "op": "eq", "property": "a", "value": "b" } },
        undefined, "_AdaptiveFunction_");
}
catch (e) {
    return e.id;
}
return "no error";
