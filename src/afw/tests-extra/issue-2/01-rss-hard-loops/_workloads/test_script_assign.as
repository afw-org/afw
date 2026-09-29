/* #2 lab: test_script copy-out assigned. create_managed_clone is
 * RC 1; extra-hold only, then slot_store get_assignable. Pair with
 * test_script_unassigned.
 */
let r = {};
while (true) {
    r = test_script("t", "d", "return 1;", 1);
}
return 0;
