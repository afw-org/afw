/* #2 lab: empty `{ }` body. 0-symbol `{ }` is not a scope (#245).
 * Should match empty_stmt (RSS and in_use flat).
 */
while (true) {
}
return 0;
