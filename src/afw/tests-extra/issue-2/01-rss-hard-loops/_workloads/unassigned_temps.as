/* #2 lab (#406): unmanaged temps created and never assigned.
 * Scope last-release is supposed to take them. add() mints an
 * unmanaged integer in the body frame; nothing stores it.
 */
while (true) {
    add(1, 1);
}
return 0;
