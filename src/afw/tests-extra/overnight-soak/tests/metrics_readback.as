const sn = open_file("metrics-read", "logs/metrics.tsv", "r");
const s = read(sn, 20000);
close(sn);
assert(length(s) > 0, "metrics.tsv should have at least one sample line");
const nl = index_of(s, "\n");
const line = nl > 0 ? substring(s, 0, nl) : s;
const cols = split(line, "\t");
assert(length(cols) >= 19,
    "expected >= 19 TSV columns, got " + string(length(cols)));
return true;
