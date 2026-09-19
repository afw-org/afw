/* Rube Goldberg leak hunt. One request; firehose repeats.
 * Do not return functions from evaluate(compile)/eval_from_file (#342).
 * Closures and methods live in this script. */
function make_box(n) {
    let v = n;
    return {
        "get": function () { return v; },
        "add": function (x) { v = v + x; return v; },
        "nest": function () {
            return evaluate(compile<script>(script(
                "let o = { k: 1 }; { let x = 3; o.k = x; } return o;")));
        }
    };
}

const b0 = make_box(3);
assert(b0.get() === 3);
b0.add(4);
assert(b0.get() === 7);
assert(b0.nest().k === 3);

const from_file = eval_from_file("tests/helper.as");
assert(from_file.seed === 7);
assert(from_file.tag === "from-file");
from_file.seed = 9;
const from_file2 = eval_from_file("tests/helper.as");
assert(from_file2.seed === 7, "eval_from_file must be a new object");
assert(from_file.seed === 9, "first from_file mutated");

const fac = eval_from_file("tests/helper_fn.as");
assert(fac(1) === 8, "eval_from_file returning function");

const cv = compile<script>(script(
    "let o = { n: 0, a: [1, 2], f: function (x) { return x + 1; } }; " +
    "o.n = o.n + 1; o.a[0] = 9; return o;"));
let kept = [];
let i = 0;
for (i = 0; i < 10; i = i + 1) {
    const o = evaluate(cv);
    o.n = i;
    o.box = make_box(i);
    kept[length(kept)] = o;
}
assert(kept[0].a[0] === 9);
assert(kept[0].f(1) === 2);
assert(kept[3].box.get() === 3);
kept[3].box.add(4);
assert(kept[3].box.get() === 7);
assert(kept[4].box.get() === 4);

let s = "ab";
let k = 0;
for (k = 0; k < 5; k = k + 1) {
    s = s + s;
}
assert(length(s) === 64, "concat doubles");
s = evaluate(compile<script>(script("return \"xy\";"))) + s;
assert(length(s) === 66);

let fns = [];
for (let j = 0; j < 6; j = j + 1) {
    const captured = j;
    fns[length(fns)] = function () {
        const inner = evaluate(compile<script>(script(
            "return { n: 1, xs: [1, 2, 3] };")));
        inner.n = captured;
        return inner;
    };
}
let acc = 0;
for (let fn of fns) {
    const o = fn();
    acc = acc + o.n;
    assert(o.xs[2] === 3);
}
assert(acc === 15, "for-of captured n");

let outer = null;
{
    {
        outer = evaluate(compile<script>(script(
            "return { a: 1, b: \"z\" };")));
        outer.a = 9;
    }
}
assert(outer.a === 9);
assert(outer.b === "z");

function deep_return() {
    {
        {
            return evaluate(compile<script>(script(
                "let a = [ { x: 1 } ]; a[0].x = 2; return a;")));
        }
    }
}
const deep = deep_return();
assert(deep[0].x === 2);

let mixed = [];
let throw_this = false;
for (i = 0; i < 8; i = i + 1) {
    try {
        if (throw_this) {
            evaluate(compile<script>(script("throw \"odd\";")));
        }
        const o = evaluate(compile<script>(script("return { n: 0 };")));
        o.n = i;
        mixed[length(mixed)] = o;
    } catch (e) {
        const recovered = evaluate(compile<script>(script(
            "return { n: -1 };")));
        mixed[length(mixed)] = recovered;
    }
    throw_this = !throw_this;
}
assert(length(mixed) === 8);
assert(mixed[0].n === 0);
assert(mixed[1].n === -1);
return true;

const tree = clone({ "root": 0 });
tree.child = evaluate(compile<script>(script(
    "return { leaf: [ { v: 1 }, { v: 2 } ] };")));
tree.child.leaf[0].v = 9;
assert(tree.child.leaf[1].v === 2);
assert(tree.root === 0);

let units = [];
for (i = 0; i < 4; i = i + 1) {
    units[length(units)] = compile<script>(script(
        "return { i: " + string(i) + " };"));
}
let ri = 0;
for (ri = length(units) - 1; ri >= 0; ri = ri - 1) {
    const o = evaluate(units[ri]);
    assert(o.i === ri);
}

const tmpl = evaluate(compile<template>(template("A${1+2}B")));
assert(tmpl === "A3B");

const nested = evaluate(compile<script>(script(
    "return evaluate(compile<script>(script(\"return { z: 4 };\")));")));
assert(nested.z === 4);

let div0 = false;
try {
    evaluate(compile<script>(script("return 1 / 0;")));
} catch (e) {
    div0 = true;
}
assert(div0);

const name1 = "scratch/nasty-" + generate_uuid() + ".txt";
const name2 = "scratch/nasty-" + generate_uuid() + ".bin";
const w1 = open_file("nasty-w1", name1, "w", true);
writeln(w1, "hello");
writeln(w1, s);
close(w1);
const w2 = open_file("nasty-w2", name2, "w", true);
write(w2, "part-a");
write(w2, "-part-b");
close(w2);
const r1 = open_file("nasty-r1", name1, "r");
const body1 = read(r1, 400);
close(r1);
const r2 = open_file("nasty-r2", name2, "r");
const body2 = read(r2, 64);
close(r2);
assert(length(body1) > 0);
assert(body2 === "part-a-part-b", body2);
const a1 = open_file("nasty-a1", name1, "a", true);
writeln(a1, "more");
close(a1);

let a = [];
push(a, 1);
push(a, { "p": 2, "q": [3, 4] });
push(a, make_box(5));
assert(a[2].get() === 5);
assert(pop(a).get() === 5);
assert(pop(a).q[1] === 4);
shift(a);
assert(length(a) === 0);

let bag = { "n": 0 };
for (i = 0; i < 6; i = i + 1) {
    bag = { "n": i, "prev": bag };
}
assert(bag.n === 5);
assert(bag.prev.prev.n === 3);

const cat = retrieve_objects("afw", "_AdaptiveObjectType_",
    undefined, undefined, undefined, 0);
assert(length(cat) > 0);
const one = get_object("afw", "_AdaptiveObjectType_", "string");
assert(one !== undefined);

return true;
