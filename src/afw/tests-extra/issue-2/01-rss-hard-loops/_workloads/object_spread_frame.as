/* #2 lab: object spread held in the body frame. Script-built
 * container; frame slot isolates. Should stay flat.
 */
const extra = { port: 1 };
while (true) {
    const o = { ...extra, host: "h" };
}
return 0;
