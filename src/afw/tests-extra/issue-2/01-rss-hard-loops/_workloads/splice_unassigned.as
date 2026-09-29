/* #2 lab (#405): splice copy-out, never assigned. Length stays 5
 * (remove one, insert one). Last statement is add() so deactivate
 * does not slot_store the removed array into script_result.
 * Extra-hold on the copy-out is the watch (assign is splice_assign).
 */
let a = [0, 1, 2, 3, 4];
let i = 0;
while (true) {
    splice(a, 1, 1, i);
    i = i + 1;
    add(0, 0);
}
return 0;
