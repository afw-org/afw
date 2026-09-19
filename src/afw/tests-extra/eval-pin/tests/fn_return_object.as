function f() {
    let o = { k: 1 };
    o.k = 2;
    return o;
}
const x = f();
assert(x.k === 2);
x.k = 3;
assert(x.k === 3);
const y = f();
assert(y.k === 2, "next call must not share the previous object");
return true;
