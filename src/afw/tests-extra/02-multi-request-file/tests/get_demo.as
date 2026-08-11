const o = get_object("file", "Demo", "alpha");
assert(o.msg == "from-request-1", "msg not persisted across requests");
assert(o.n == 1, "n not persisted across requests");
return o.msg;
