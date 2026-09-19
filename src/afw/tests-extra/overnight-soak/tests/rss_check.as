/* After firehose: fail if RSS grew more than 64 MiB from the first
 * sample (includes warmup). Tight enough to catch overnight climb;
 * 20s smoke that plateaued ~23→46 MiB still passes. */
const f = open_file("rss-first-check", "logs/rss-first.txt", "r");
const first = integer(read(f, 32));
close(f);
const l = open_file("rss-last-check", "logs/rss-last.txt", "r");
const last = integer(read(l, 32));
close(l);
const grow_kb = last - first;
assert(grow_kb < 65536,
    "RSS grew " + string(grow_kb) + " KB (first=" + string(first) +
    " last=" + string(last) + ")");
return grow_kb;
