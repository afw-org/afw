/* #2 lab: add_properties with no target held in the body frame.
 * Should stay flat.
 */
const extra = { port: 1 };
const more = { host: "h" };
while (true) {
    const o = add_properties(undefined, extra, more);
}
return 0;
