/* #2 lab: clone nested object/array, assign to a variable, mutate
 * a property/element, clone that occupant. Last-release of the
 * execute result after the copy, take nested, then slot_store of
 * the child. Member get is caller does not release.
 */
let o = { child: { x: 1, inner: { y: 2 } }, arr: [0, 1, 2, 3, 4] };
let v = {};
let e = [];
let c = {};
let i = 0;
while (true) {
    v = clone(o).child;
    e = clone(o).arr;
    c = clone(v);
    v.x = i;
    e[0] = i;
    i = i + 1;
}
return 0;
