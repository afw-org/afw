let o = { n: 0 };
let i = 0;
for (i = 0; i < 8; i = i + 1) {
    o.n = i;
}
assert(o.n === 7);
return true;
