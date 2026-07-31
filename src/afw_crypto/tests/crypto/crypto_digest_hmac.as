#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: crypto_digest_hmac.as
//? customPurpose: Part of afw_crypto tests
//? description: Test digest and HMAC with known vectors.
//? sourceType: script
//?
//? test: crypto_digest_sha256_empty
//? description: SHA-256 of empty message
//? expect: 0
//? source: ...
#!/usr/bin/env afw

// Base64 of SHA-256("") = 47DEQpj8HBSa+/TImW+5JCeuQeRkm5NMpJWZG3hSuFU=
const d = crypto_digest("SHA-256", base64Binary(""));
assert(string(d) === "47DEQpj8HBSa+/TImW+5JCeuQeRkm5NMpJWZG3hSuFU=");

return 0;

//?
//? test: crypto_hmac_roundtrip
//? description: HMAC produce and verify with raw key material
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const key = random_base64Binary(32);
const data = base64Binary("aGVsbG8="); // "hello"
const mac = crypto_hmac("HMAC-SHA-256", key, data);
assert(crypto_hmac_verify("HMAC-SHA-256", key, data, mac) === true);
assert(crypto_hmac_verify("HMAC-SHA-256", key, data, random_base64Binary(32)) === false);

return 0;
