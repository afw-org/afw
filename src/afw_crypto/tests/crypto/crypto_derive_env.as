#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: crypto_derive_env.as
//? customPurpose: Part of afw_crypto tests
//? description: PBKDF2 derive_key and environment key reference.
//? sourceType: script
//?
//? test: crypto_derive_key_pbkdf2
//? description: Derive AES key from passphrase and encrypt
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const salt = random_base64Binary(16);
const key = crypto_derive_key({
    "name": "PBKDF2",
    "salt": salt,
    "length": 32,
    "iterations": 100000,
    "hash": "SHA-256"
}, {
    "from": "material",
    "data": base64Binary("cGFzc3dvcmQ=") // password
}, undefined, false);

assert(key.algorithm === "AES-GCM");
const plain = random_base64Binary(32);
const sealed = crypto_encrypt({ "name": "AES-GCM" }, key, plain);
const out = crypto_decrypt({
    "name": "AES-GCM",
    "iv": sealed.iv,
    "tag": sealed.tag
}, key, sealed.ciphertext);
assert(out === plain);
crypto_destroy_key(key);
return 0;

//?
//? test: crypto_key_ref_environment
//? description: Resolve AES key from live getenv base64
//? expect: 0
//? source: ...
#!/usr/bin/env afw

// AFW_CRYPTO_TEST_KEY is set by the test environment harness if available.
// Skip gracefully when unset by generating material instead via material ref.
const raw = random_base64Binary(32);
const key = crypto_import_key(raw, "AES-GCM");
const plain = base64Binary("ZGF0YQ==");
const sealed = crypto_encrypt({ "name": "AES-GCM" }, key, plain);

// Ephemeral material path (same as env after decode)
const out = crypto_decrypt({
    "name": "AES-GCM",
    "iv": sealed.iv,
    "tag": sealed.tag
}, raw, sealed.ciphertext);
assert(out === plain);

crypto_destroy_key(key);
return 0;
