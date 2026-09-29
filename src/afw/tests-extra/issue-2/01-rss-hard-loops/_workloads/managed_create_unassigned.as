/* #2 lab: extra-hold after create_managed, never assigned. If the
 * extra-hold is missing, RC 1 from create is never released and this
 * climbs. Same call set as managed_create_assign.
 */
function keep(x) {
    return true;
}
function ident(x) {
    return x;
}
function before(a, b) {
    return a < b;
}
let nums = bag<integer>(0, 1, 2, 3, 4);
let o = { a: 1, b: 2, c: 3 };
let s = "a,b,c,d,e";
while (true) {
    reverse(nums);
    slice(nums, 1, 4);
    filter(keep, nums);
    map(ident, nums);
    sort(before, nums);
    bag<integer>(1, 2, 3);
    intersection<integer>(nums, nums);
    split(s, ",");
    union<integer>(nums, nums);
    keys(o);
    values(o);
    entries(o);
    /* Not the last statement: deactivate would slot_store entries(). */
    add(0, 0);
}
return 0;
