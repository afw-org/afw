/* #2 lab: object pattern rest assigned. Dest p unmanaged, like
 * array rest. unmanaged_new_p left a child of managed_p.
 */
const obj = { a: 1, b: 2, c: 3, d: 4 };
let a;
let rest;
while (true) {
    ({ a, ...rest } = obj);
}
return 0;
