/* #2 lab: rebind a closure that captures a per-iteration let.
 * slot_store of the function value should release the previous closure/scope.
 */
let i = 0;
let f;
while (true) {
    let v = i;
    f = function() {
        return v;
    };
    i = i + 1;
}
return 0;
