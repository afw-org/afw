/* #2 lab: `a = [i]` each iteration in one long scope.
 * Same isolate-on-assign as object_rebind. Should stay flat.
 */
let i = 0;
let a = [];
while (true) {
    a = [i];
    i = i + 1;
}
return 0;
