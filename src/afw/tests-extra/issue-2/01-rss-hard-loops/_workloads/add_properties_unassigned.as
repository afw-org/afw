/* #2 lab: add_properties with no target, never assigned (last stmt
 * add()). Should stay flat.
 */
const extra = { port: 1 };
const more = { host: "h" };
while (true) {
    add_properties(undefined, extra, more);
    add(0, 0);
}
return 0;
