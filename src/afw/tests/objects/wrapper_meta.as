#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: wrapper_meta.as
//? customPurpose: Part of core object tests
//? description: A managed wrapper over an object view keeps its meta after the view's call is done (#532)
//? sourceType: script
//?
//? test: view-meta-outlives-call
//? description: The view's object meta is in the view's pool; the wrapper made by storing it in a variable copies it
//? expect: true
//? source: ...

function f() {
    // Storing the view in a variable makes a managed wrapper over it.
    const o = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveFunction_",
        {objectId: true, objectType: true, path: true});
    return o;
}

const w = f();
const s = stringify(w);
assert(index_of(s, '"path":"/afw/_AdaptiveObjectType_/_AdaptiveFunction_"') >= 0, s);
assert(index_of(s, '"objectId":"_AdaptiveFunction_"') >= 0, s);
assert(index_of(s, '"objectType":"_AdaptiveObjectType_"') >= 0, s);
return true;

//? test: view-meta-many-calls
//? description: Many wrappers over views, each outliving its call
//? expect: 50
//? source: ...

function f(id: string) {
    const o = get_object("afw", "_AdaptiveFunction_", id, {objectId: true, path: true});
    return o;
}

let n = 0;
for (const id of ["add", "subtract", "multiply", "divide", "mod"]) {
    for (let i = 0; i < 10; i = i + 1) {
        const w = f(id);
        if (index_of(stringify(w), '"objectId":"' + id + '"') >= 0) {
            n = n + 1;
        }
    }
}
return n;
