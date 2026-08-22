/* #2 lab: `a = [i]` each iteration in one long scope.
 * Same unmanaged-face overwrite as object_rebind.
 */
let i = 0;
let a = [];
while (true) {
    a = [i];
    i = i + 1;
}
return 0;
