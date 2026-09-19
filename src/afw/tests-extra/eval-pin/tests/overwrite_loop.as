let o = { n: -1 };
let i = 0;
for (i = 0; i < 30; i = i + 1) {
    o = { n: i };
}
assert(o.n === 29);
return o.n;
