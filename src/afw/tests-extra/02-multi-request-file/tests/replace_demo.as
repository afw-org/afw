replace_object(
    "file",
    "Demo",
    "alpha",
    {
        "msg": "from-request-3",
        "n": 3
    }
);
const o = get_object("file", "Demo", "alpha");
assert(o.msg == "from-request-3");
assert(o.n == 3);
return o.n;
