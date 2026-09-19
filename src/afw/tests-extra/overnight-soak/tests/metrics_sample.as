const rss = process_rss();
const used = pool_bytes_in_use();
const a = get_object("afw", "_AdaptiveAdapter_", "afw");
let gets = 0;
if (a.metrics !== undefined && a.metrics.getObjectCount !== undefined) {
    gets = a.metrics.getObjectCount;
}
const line = generate_uuid() + "\t" + string(rss) + "\t" +
    string(used) + "\t" + string(gets);
const sn = open_file("metrics", "logs/metrics.tsv", "a", true);
writeln(sn, line);
close(sn);
return true;
