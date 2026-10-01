/* #2 lab: test_script copy-out is create_managed. Extra-hold only
 * on the result object. The compile unit is last-released in
 * FINALLY. unmanaged_new_p left a child heap of managed_p.
 */
while (true) {
    test_script("t", "d", "return 1;", 1);
    add(0, 0);
}
return 0;
