const sn = open_file("metrics-read", "logs/metrics.tsv", "r");
const s = read(sn, 20000);
close(sn);
assert(length(s) > 0, "metrics.tsv should have at least one sample line");
return true;
