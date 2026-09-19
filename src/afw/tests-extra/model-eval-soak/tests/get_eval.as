const o = get_object("model", "Demo", "seed");
assert(o.msg === "from-eval");
assert(o.tag === "soak");
o.msg = "mut";
const o2 = get_object("model", "Demo", "seed");
assert(o2.msg === "from-eval", "next get must be a new pinned value");
return true;
