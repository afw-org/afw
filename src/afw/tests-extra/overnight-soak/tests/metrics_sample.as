/* Live process:: and _AdaptiveServer_/current (#329 / #74). */
const rss = process::rss;
const used = process::poolBytesInUse;
const peak_used = process::peakPoolBytesInUse;
const chunks = process::poolChunkBytes;
const peak_chunks = process::peakPoolChunkBytes;
let concurrent = 0;
let max_conc = 0;
let requests = 0;
try {
    const s = get_object("afw", "_AdaptiveServer_", "current");
    concurrent = s.concurrent;
    max_conc = s.maxConcurrent;
    requests = s.requestCount;
} catch (e) {
}
const a = get_object("afw", "_AdaptiveAdapter_", "afw");
let gets = 0;
if (a.metrics !== undefined && a.metrics.getObjectCount !== undefined) {
    gets = a.metrics.getObjectCount;
}
const line = generate_uuid() + "\t" +
    string(rss) + "\t" +
    string(used) + "\t" + string(peak_used) + "\t" +
    string(chunks) + "\t" + string(peak_chunks) + "\t" +
    string(concurrent) + "\t" + string(max_conc) + "\t" +
    string(requests) + "\t" + string(gets);
const sn = open_file("metrics", "logs/metrics.tsv", "a", true);
writeln(sn, line);
close(sn);

let have_first = true;
try {
    const r = open_file("rss-first-r", "logs/rss-first.txt", "r");
    close(r);
} catch (e) {
    have_first = false;
}
if (!have_first) {
    const w = open_file("rss-first-w", "logs/rss-first.txt", "w", true);
    write(w, string(rss));
    close(w);
}
const last = open_file("rss-last-w", "logs/rss-last.txt", "w", true);
write(last, string(rss));
close(last);
return true;
