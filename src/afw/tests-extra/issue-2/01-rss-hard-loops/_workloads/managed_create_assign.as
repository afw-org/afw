/* #2 lab: extra-hold after create_managed, then assign. Same class
 * as splice_assign (#405): create starts at RC 1;
 * get_assignable_for_scope_lifetime would bump and leak on assign.
 * Covers reverse / slice / filter / map / sort / array<integer> / intersection /
 * split / union / keys / values / entries. Functors are compiled
 * once. Source arrays stay length-stable (no splice).
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
let nums = array<integer>(0, 1, 2, 3, 4);
let o = { a: 1, b: 2, c: 3 };
let s = "a,b,c,d,e";
let r_reverse = [];
let r_slice = [];
let r_filter = [];
let r_map = [];
let r_sort = [];
let r_array = [];
let r_intersection = [];
let r_split = [];
let r_union = [];
let r_keys = [];
let r_values = [];
let r_entries = [];
while (true) {
    r_reverse = reverse(nums);
    r_slice = slice(nums, 1, 4);
    r_filter = filter(nums, keep);
    r_map = map(nums, ident);
    r_sort = sort(nums, before);
    r_array = array<integer>(1, 2, 3);
    r_intersection = intersection<integer>(nums, nums);
    r_split = split(s, ",");
    r_union = union<integer>(nums, nums);
    r_keys = keys(o);
    r_values = values(o);
    r_entries = entries(o);
}
return 0;
