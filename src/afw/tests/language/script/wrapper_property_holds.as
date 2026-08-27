#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: wrapper_property_holds.as
//? customPurpose: Part of language/script tests
//? description: ...
Issue #2 script-evaluation-aware wrapper: overlay holds and last-release
walk. Generic memory objects still do not own property values. Script-mutable
creates (add_properties with no target, object construct, create_array,
array(), clone) must be or become a face so o.fn = function… is a slot.
Literal {} / [] already wrap (closures.as). Throw-path: throw_rewind.as.
//? sourceType: script
//?
//? test: add_properties-empty-assign-function
//? description: Factory empty add_properties(); o.fn = function; call after return
//? skip: false
//? expect: 0
//? source: ...

function factory() {
    let n = 7;
    let o = add_properties(undefined, {});
    o.get = function() {
        return n;
    };
    return o;
}

const bag = factory();
assert(bag.get() === 7);
return 0;

//?
//? test: add_properties-overwrite-releases-old
//? description: Overwrite o.fn on add_properties() object; new binding is used
//? skip: false
//? expect: 0
//? source: ...

function factory() {
    let a = 1;
    let b = 2;
    let o = add_properties(undefined, {});
    o.fn = function() {
        return a;
    };
    o.fn = function() {
        return b;
    };
    return o;
}

assert(factory().fn() === 2);
return 0;

//?
//? test: object-construct-computed-name-function
//? description: Object construct with computed name stores a closure
//? skip: false
//? expect: 0
//? source: ...

function factory() {
    let n = 4;
    let k = "get";
    return {
        [k]: function() {
            return n;
        }
    };
}

assert(factory().get() === 4);
return 0;

//?
//? test: nested-if-assign-property-defining-scope
//? description: if { o.fn = tick } holds the defining scope, not the inner if
//? skip: false
//? expect: 0
//? source: ...

function factory() {
    let n = 5;
    const tick = function() {
        return n;
    };
    let o = {};
    if (true) {
        o.fn = tick;
    }
    return o;
}

assert(factory().fn() === 5);
return 0;

//?
//? test: create_array-assign-function
//? description: create_array then a[i] = function; call after factory returns
//? skip: false
//? expect: 0
//? source: ...

function factory() {
    let n = 8;
    let a = create_array(1);
    a[0] = function() {
        return n;
    };
    return a;
}

assert(factory()[0]() === 8);
return 0;

//?
//? test: array-constructor-function-element
//? description: array(function…) element callable after factory returns
//? skip: false
//? expect: 0
//? source: ...

function factory() {
    let n = 9;
    return array(function() {
        return n;
    });
}

assert(factory()[0]() === 9);
return 0;

//?
//? test: array-overwrite-element
//? description: Overwrite a[0] with a new closure; new binding is used
//? skip: false
//? expect: 0
//? source: ...

function factory() {
    let a = 1;
    let b = 2;
    let list = [];
    list[0] = function() {
        return a;
    };
    list[0] = function() {
        return b;
    };
    return list;
}

assert(factory()[0]() === 2);
return 0;

//?
//? test: clone-then-assign-function
//? description: clone({}) then o.fn = function; call after factory returns
//? skip: false
//? expect: 0
//? source: ...

function factory() {
    let n = 6;
    let o = clone({});
    o.get = function() {
        return n;
    };
    return o;
}

assert(factory().get() === 6);
return 0;

//?
//? test: clone-array-then-assign-function
//? description: clone([]) then a[0] = function; call after factory returns
//? skip: false
//? expect: 0
//? source: ...

function factory() {
    let n = 8;
    let a = clone([]);
    a[0] = function() {
        return n;
    };
    return a;
}

assert(factory()[0]() === 8);
return 0;

//?
//? test: nested-eval-escaped-property-closure
//? description: evaluate(compile) factory returns object with closure
//? skip: false
//? expect: 0
//? source: ...

const src = script(""
    + "function factory() {"
    + "    let n = 3;"
    + "    let o = add_properties(undefined, {});"
    + "    o.get = function() { return n; };"
    + "    return o;"
    + "}"
    + "return factory();");
const bag = evaluate(compile<script>(src));
assert(bag.get() === 3);
return 0;
