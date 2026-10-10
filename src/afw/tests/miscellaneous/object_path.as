#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: object_path.as
//? customPurpose: Part of miscellaneous category tests
//? description: Object path parsing through parse_uri and compare_uri (#535)
//? sourceType: script
//?
//? test: raw-colon-in-object-id
//? description: A raw ':' is part of the object id, the same as '%3A'
//? expect: 0
//? source: ...

const vp = parse_uri("/a/b/c:d", true).valuePath;
assert(vp.adapterId === "a");
assert(vp.objectType === "b");
assert(vp.entityObjectId === "c:d");
assert(vp.normalizedPath === "/a/b/c%3Ad");
assert(vp.containsUnresolvedSubstitutions === undefined);
assert(parse_uri("/a/b/c%3Ad", true).valuePath.entityObjectId === "c:d");
assert(compare_uri("/a/b/c:d", "/a/b/c%3Ad", true) === true);
assert(compare_uri("/a/b/c:d", "/a/b/c%3Ae", true) === false);
return 0;

//?
//? test: journal-special-object-id
//? description: A journal special objectId with raw ':' separators
//? expect: 0
//? source: ...

const vp = parse_uri(
    "/journal/_AdaptiveJournalEntry_/get_next_for_consumer:rest:10",
    true).valuePath;
assert(vp.adapterId === "journal");
assert(vp.objectType === "_AdaptiveJournalEntry_");
assert(vp.entityObjectId === "get_next_for_consumer:rest:10");
assert(vp.propertyTypes === undefined);
return 0;

//?
//? test: every-pchar-delimiter-in-object-id
//? description: ':', '@' and every sub-delim are part of an object id
//? expect: 0
//? source: ...

const raw = "/a/b/c@d!$&'()+,;=e:f";
const encoded = "/a/b/c%40d%21%24%26%27%28%29%2B%2C%3B%3De%3Af";
const vp = parse_uri(raw, true).valuePath;
assert(vp.entityObjectId === "c@d!$&'()+,;=e:f");
assert(vp.normalizedPath === encoded);
assert(vp.optionsObject === undefined);
assert(parse_uri(encoded, true).valuePath.entityObjectId === vp.entityObjectId);
assert(compare_uri(raw, encoded, true) === true);
return 0;

//?
//? test: raw-colon-in-adapter-and-object-type
//? description: Adapter id and object type id take pchar too
//? expect: 0
//? source: ...

const vp = parse_uri("/a:b/c@d/e", true).valuePath;
assert(vp.adapterId === "a:b");
assert(vp.objectType === "c@d");
assert(vp.entityObjectId === "e");
assert(vp.normalizedPath === "/a%3Ab/c%40d/e");
return 0;

//?
//? test: raw-delimiters-in-property-names
//? description: Property names take ':' '=' '&' ';' raw
//? expect: 0
//? source: ...

const vp = parse_uri("/a/b/c/p:q/r=s&t;u", true).valuePath;
assert(vp.entityObjectId === "c");
assert(vp.propertyTypes[0] === "p:q");
assert(vp.propertyTypes[1] === "r=s&t;u");
assert(length(vp.propertyTypes) === 2);
assert(vp.entityPath === "/a/b/c");
assert(vp.normalizedPath === "/a/b/c/p%3Aq/r%3Ds%26t%3Bu");
assert(compare_uri("/a/b/c/p:q", "/a/b/c/p%3Aq", true) === true);
return 0;

//?
//? test: normalized-path-encodes-whole-property-name
//? description: normalizedPath has room for an encoded property name
//? expect: 0
//? source: ...

// The normalized path was sized by the decoded property name, so an
// encoded name was cut short or left as NUL octets.
assert(parse_uri("/a/b/c/p%3Aq/r", true).valuePath.normalizedPath ===
    "/a/b/c/p%3Aq/r");
assert(parse_uri("/a/b/c/*", true).valuePath.normalizedPath ===
    "/a/b/c/%2A");
assert(parse_uri("/a/b/c/x%20y", true).valuePath.normalizedPath ===
    "/a/b/c/x%20y");
return 0;

//?
//? test: compare-uri-property-names
//? description: compare_uri compares the property names of both paths
//? expect: 0
//? source: ...

assert(compare_uri("/a/b/c/x", "/a/b/c/x", true) === true);
assert(compare_uri("/a/b/c/x", "/a/b/c/y", true) === false);
assert(compare_uri("/a/b/c/x", "/a/b/c/x/y", true) === false);
assert(compare_uri("/a/b/c/x/y", "/a/b/c/x", true) === false);
assert(compare_uri("/a/b/c", "/a/b/c/x", true) === false);
return 0;

//?
//? test: asterisk-inside-a-segment-is-literal
//? description: Only a whole "*" or "**" segment is a wildcard
//? expect: 0
//? source: ...

let vp = parse_uri("/a/b/a*b", true).valuePath;
assert(vp.entityObjectId === "a*b");
assert(vp.normalizedPath === "/a/b/a%2Ab");
assert(vp.containsUnresolvedSubstitutions === undefined);

vp = parse_uri("/a/b/c/*x", true, "/x/y/z/p").valuePath;
assert(vp.propertyTypes[0] === "*x");
assert(vp.substitutionOccurred === undefined);

vp = parse_uri("/a*/b*/c", true, "/x/y/z").valuePath;
assert(vp.adapterId === "a*");
assert(vp.objectType === "b*");
assert(vp.substitutionOccurred === undefined);

// '**' is a wildcard only as the entity object id.
vp = parse_uri("/**/b/c", true).valuePath;
assert(vp.adapterId === "**");
assert(vp.containsUnresolvedSubstitutions === undefined);

// An encoded '*' is a literal object id.
vp = parse_uri("/a/b/%2A", true, "/x/y/z").valuePath;
assert(vp.entityObjectId === "*");
assert(vp.substitutionOccurred === undefined);
assert(vp.containsUnresolvedSubstitutions === undefined);
return 0;

//?
//? test: wildcards-without-current-path
//? description: '*' and '**' segments without a current path are unresolved
//? expect: 0
//? source: ...

let vp = parse_uri("/a/b/*", true).valuePath;
assert(vp.entityObjectId === "*");
assert(vp.containsUnresolvedSubstitutions === true);

vp = parse_uri("/*/b/c", true).valuePath;
assert(vp.adapterId === "*");
assert(vp.containsUnresolvedSubstitutions === true);

vp = parse_uri("/a/b/**", true).valuePath;
assert(vp.substitutedEntireObjectId === true);
assert(vp.containsUnresolvedSubstitutions === true);

vp = parse_uri("/a/b/c/*", true).valuePath;
assert(vp.propertyTypes[0] === "*");
assert(vp.containsUnresolvedSubstitutions === true);
return 0;

//?
//? test: wildcards-with-current-path
//? description: '*' and '**' segments take their parts from the current path
//? expect: 0
//? source: ...

let vp = parse_uri("/*/*/c:d", true, "/x/y/z").valuePath;
assert(vp.adapterId === "x");
assert(vp.objectType === "y");
assert(vp.entityObjectId === "c:d");
assert(vp.substitutedAdapterId === true);
assert(vp.substitutedObjectTypeId === true);
assert(vp.containsUnresolvedSubstitutions === undefined);

vp = parse_uri("/a/b/*", true, "/x/y/z:w").valuePath;
assert(vp.entityObjectId === "z:w");
assert(vp.substitutedEntityObjectId === true);

vp = parse_uri("/a/b/*/q", true, "/x/y/z/p").valuePath;
assert(vp.entityObjectId === "z");
assert(vp.propertyTypes[0] === "q");

vp = parse_uri("/a/b/c/*", true, "/x/y/z/p:q").valuePath;
assert(vp.propertyTypes[0] === "p:q");
assert(vp.substitutedPropertyName === true);

vp = parse_uri("/a/b/**", true, "/x/y/z:w/p").valuePath;
assert(vp.entityObjectId === "z:w");
assert(vp.propertyTypes[0] === "p");
assert(vp.substitutedEntireObjectId === true);
assert(vp.containsUnresolvedSubstitutions === undefined);
return 0;

//?
//? test: options-after-object-type
//? description: ';' after the object type still starts options
//? expect: 0
//? source: ...

let vp = parse_uri("/a/b;objectId&normalize/c:d", true).valuePath;
assert(vp.objectType === "b");
assert(vp.optionsObject.objectId === true);
assert(vp.optionsObject.normalize === true);
assert(vp.entityObjectId === "c:d");

vp = parse_uri("/a/b;objectId", true).valuePath;
assert(vp.optionsObject.objectId === true);
assert(vp.entityObjectId === undefined);

// After the object type, ';' is only an object id octet.
vp = parse_uri("/a/b/c;objectId", true).valuePath;
assert(vp.entityObjectId === "c;objectId");
assert(vp.optionsObject === undefined);
return 0;

//?
//? test: relative-paths
//? description: A relative path is an entity object id and property names
//? expect: 0
//? source: ...

// A raw ':' in the first segment of a relative URI makes it a scheme, so
// it must be encoded there (RFC 3986 4.2).
let vp = parse_uri("c%3Ad/p", true).valuePath;
assert(vp.adapterId === "*");
assert(vp.objectType === "*");
assert(vp.entityObjectId === "c:d");
assert(vp.propertyTypes[0] === "p");
assert(vp.containsUnresolvedSubstitutions === true);

// A literal '*' is not a wildcard, so the current path is not used.
vp = parse_uri("a*b", true, "/x/y/z").valuePath;
assert(vp.adapterId === "*");
assert(vp.entityObjectId === "a*b");
assert(vp.containsUnresolvedSubstitutions === true);

vp = parse_uri("*", true, "/x/y/z").valuePath;
assert(vp.adapterId === "x");
assert(vp.objectType === "y");
assert(vp.entityObjectId === "z");
return 0;

//?
//? test: unchanged-forms
//? description: Forms that parsed before parse the same way
//? expect: 0
//? source: ...

let vp = parse_uri("/a/b/c", true).valuePath;
assert(vp.normalizedPath === "/a/b/c");
assert(vp.entityObjectId === "c");

vp = parse_uri("/a/b/", true).valuePath;
assert(vp.objectType === "b");
assert(vp.entityObjectId === undefined);

vp = parse_uri("/a", true).valuePath;
assert(vp.adapterId === "a");
assert(vp.objectType === undefined);

vp = parse_uri("/a/b/c%20d", true).valuePath;
assert(vp.entityObjectId === "c d");
return 0;

//?
//? test: path-errors
//? description: Paths that are still errors
//? expect: 0
//? source: ...

function parse_error(path: string): string {
    try {
        parse_uri(path, true);
    }
    catch (e) {
        return e.message;
    }
    return "no error for " + path;
}

const object_path_errors = [
    "/a//c",        // empty object type
    "/a/b//c",      // empty object id
    "/a/b/c//p",    // empty property name
    "/a/b/c/",      // trailing slash after object id
    "/a/b/**/p",    // nothing may follow '**'
    "/a/b;/c",      // empty option name
    "/a/b;x=/c"     // empty option value
];
for (const path of object_path_errors) {
    const message = parse_error(path);
    assert(includes(message, "Error parsing object path"),
        path + ": " + message);
}

// Not pchar and not percent-encoded.
for (const path of ["/a/b/c d", "/a/b/c[d", "/a/b/c\"d", "/a/b/c%4",
    "/a/b/c%"])
{
    const message = parse_error(path);
    assert(!includes(message, "no error for"), message);
}
return 0;
