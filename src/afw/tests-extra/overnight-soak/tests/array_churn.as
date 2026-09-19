let a = [];
let i = 0;
for (i = 0; i < 8; i = i + 1) {
    a[i] = i;
}
assert(a[7] === 7);
a[0] = 9;
assert(a[0] === 9);
return true;
