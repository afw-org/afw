const base = environment::AFW_WORLD_HTTP_URL;
const url = base + "/tests-extra/world/vfs/hello.txt";
const r = http_get(url);
assert(r.response_code === 200);
/* http_get stores the body as base64. hello\n is aGVsbG8K. */
assert(r.response === "aGVsbG8K");
return true;
