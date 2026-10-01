/* #2 lab: clone() of array/object then assign. Always-copy
 * create_managed, last-release of the execute result after the copy,
 * take nested, then slot_store.
 */
let a = [0, 1, 2, 3, 4];
let o = { a: 1, b: 2, c: 3 };
let ra = [];
let ro = {};
while (true) {
    ra = clone(a);
    ro = clone(o);
}
return 0;
