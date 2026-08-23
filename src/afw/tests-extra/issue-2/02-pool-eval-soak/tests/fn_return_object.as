function f() {
    let o = { k: 1 };
    o.k = 2;
    return o;
}
const x = f();
assert(x.k == 2);
return x.k;
