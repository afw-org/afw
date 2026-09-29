/* #2 lab (#407): readln in a tight loop. The Python harness writes
 * data/lines.txt (short lines plus one long enough to grow the 256
 * byte buffer) and passes --conf with rootFilePaths. Reopen after
 * close so the loop does not sit at EOF. @fixme #2 grows the line
 * in x->p; this soak shows whether that climbs.
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
