let i: integer = 0;
let last: integer = -1;
for (i = 0; i < 40; i = i + 1) {
    let o = { n: i };
    last = o.n;
}
assert(last == 39);
return last;
