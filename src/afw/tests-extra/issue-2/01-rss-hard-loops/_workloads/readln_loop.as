/* #2 lab (#407): readln in a tight loop. The Python harness writes
 * data/lines.txt (short lines plus one long enough to grow the line)
 * and passes --conf with rootFilePaths. Reopen after close so the
 * loop does not sit at EOF. Line scratch is a memory writer that
 * copies out into x->p.
 */
while (true) {
    const sn = open_file("ln", "data/lines.txt", "r");
    let line = readln(sn);
    while (line !== "") {
        line = readln(sn);
    }
    close(sn);
}
return 0;
