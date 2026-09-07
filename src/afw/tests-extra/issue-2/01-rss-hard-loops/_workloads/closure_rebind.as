/* #2 lab: rebind a closure that captures a per-iteration let.
 * slot_store of the function value last-releases the previous closure.
 * Should stay flat.
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
