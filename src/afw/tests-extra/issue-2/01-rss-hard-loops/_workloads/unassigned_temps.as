/* #2 lab (#406): unmanaged temps created and never assigned.
 * add() mints an unmanaged integer in the body frame. It is also
 * the last statement, so deactivate slot_stores that integer into
 * script_result (scalar; not a create_managed leftover-RC watch).
 */
while (true) {
    add(1, 1);
}
return 0;
