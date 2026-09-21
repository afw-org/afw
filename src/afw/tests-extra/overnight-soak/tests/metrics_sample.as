/* Live process:: and _AdaptiveServer_/current (#329 / #74). */
const rss = process::rss;
const used = process::poolBytesInUse;
const peak_used = process::peakPoolBytesInUse;
const chunks = process::poolChunkBytes;
const peak_chunks = process::peakPoolChunkBytes;
const region_used = process::memoryRegionBytesInUse;
const region_count = process::memoryRegionRegionsInUse;
const region_free_bytes = process::memoryRegionFreeListBytes;
const region_free_count = process::memoryRegionFreeListCount;
const region_hits = process::memoryRegionGetHits;
const region_misses = process::memoryRegionGetMisses;
const region_over = process::memoryRegionFreeOverCap;
const region_peak_used = process::memoryRegionPeakBytesInUse;
const region_peak_free = process::memoryRegionPeakFreeListBytes;
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
    string(requests) + "\t" + string(gets) + "\t" +
    string(region_used) + "\t" + string(region_count) + "\t" +
    string(region_free_bytes) + "\t" + string(region_free_count) + "\t" +
    string(region_hits) + "\t" + string(region_misses) + "\t" +
    string(region_over) + "\t" +
    string(region_peak_used) + "\t" + string(region_peak_free);
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
