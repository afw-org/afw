/* #2 lab: object spread assigned to an outer slot. Previous occupant
 * last-release. Should stay flat.
 */
const extra = { port: 1 };
let o = {};
while (true) {
    o = { ...extra, host: "h" };
}
return 0;
