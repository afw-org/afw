/* #2 lab: test_script copy-out is create_managed_clone. Extra-hold
 * only on the result object. The compile unit is last-released in
 * FINALLY. Missing extra-hold on the clone is leftover RC 1.
 */
while (true) {
    test_script("t", "d", "return 1;", 1);
    add(0, 0);
}
return 0;
