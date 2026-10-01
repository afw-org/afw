/* #2 lab: clone() never assigned. Always-copy create_managed,
 * extra-hold the root, take nested. Last statement is add() so
 * deactivate does not slot_store the clone into script_result.
 */
let a = [0, 1, 2, 3, 4];
let o = { a: 1, b: 2, c: 3 };
while (true) {
    clone(a);
    clone(o);
    add(0, 0);
}
return 0;
