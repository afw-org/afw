function f() {
    let o = { x: 0 };
    {
        let inner = { y: 5 };
        o.x = inner.y;
    }
    return o;
}
assert(f().x === 5);
return true;
