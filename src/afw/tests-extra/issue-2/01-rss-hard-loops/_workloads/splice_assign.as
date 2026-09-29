/* #2 lab (#405): splice copy-out, then assign. Length stays 5
 * (remove one, insert one) so this is not the array_append control.
 * Scope last-release should take the copy-out if it is not kept.
 * Assigning the removed array each trip is the watch.
 */
let a = [0, 1, 2, 3, 4];
let i = 0;
let r = [];
while (true) {
    r = splice(a, 1, 1, i);
    i = i + 1;
}
return 0;
