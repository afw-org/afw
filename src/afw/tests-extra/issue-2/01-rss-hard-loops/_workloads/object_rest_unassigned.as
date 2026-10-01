/* #2 lab: object pattern rest never assigned (last stmt add()).
 * Dest p unmanaged, like array rest. unmanaged_new_p left a
 * child of managed_p.
 */
const obj = { a: 1, b: 2, c: 3, d: 4 };
while (true) {
    const { a, ...rest } = obj;
    add(0, 0);
}
return 0;
