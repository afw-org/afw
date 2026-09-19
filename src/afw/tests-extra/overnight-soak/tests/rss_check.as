/* After firehose: fail if process::rss grew more than 64 MiB from the
 * first sample (includes warmup). process::rss is bytes. */
const f = open_file("rss-first-check", "logs/rss-first.txt", "r");
const first = integer(read(f, 32));
close(f);
const l = open_file("rss-last-check", "logs/rss-last.txt", "r");
const last = integer(read(l, 32));
close(l);
const grow = last - first;
assert(grow < 67108864,
    "RSS grew " + string(grow) + " bytes (first=" + string(first) +
    " last=" + string(last) + ")");
return grow;
