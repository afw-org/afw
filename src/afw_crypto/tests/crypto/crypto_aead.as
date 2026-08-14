#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: crypto_aead.as
//? customPurpose: Part of afw_crypto tests
//? description: AES-GCM encrypt/decrypt with generate_key and keystore.
//? sourceType: script
//?
//? test: crypto_aes_gcm_roundtrip
//? description: encrypt then decrypt with generated key
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const key = crypto_generate_key("AES-GCM");
assert(key.keyId !== undefined);
assert(key.algorithm === "AES-GCM");
assert(key.extractable === false);

const plain = base64Binary("c2VjcmV0LXBheWxvYWQ="); // secret-payload
const sealed = crypto_encrypt({
    "name": "AES-GCM"
}, key, plain);

assert(sealed.algorithm === "AES-GCM");
assert(sealed.iv !== undefined);
assert(sealed.tag !== undefined);
assert(sealed.ciphertext !== undefined);
// 12-octet IV encodes to 16 base64 chars; 16-octet tag to 24
assert(length(string(sealed.iv)) === 16);
assert(length(string(sealed.tag)) === 24);

const out = crypto_decrypt({
    "name": "AES-GCM",
    "iv": sealed.iv,
    "tag": sealed.tag
}, key, sealed.ciphertext);

assert(out === plain);

crypto_destroy_key(key);
return 0;

//?
//? test: crypto_aes_gcm_aad
//? description: AAD must match on decrypt
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const key = crypto_generate_key({ "name": "AES-GCM", "length": 256 });
const plain = random_base64Binary(64);
const aad = base64Binary("YWFk"); // aad

const sealed = crypto_encrypt({
    "name": "AES-GCM",
    "additionalData": aad
}, key, plain);

const ok = crypto_decrypt({
    "name": "AES-GCM",
    "iv": sealed.iv,
    "tag": sealed.tag,
    "additionalData": aad
}, key, sealed.ciphertext);
assert(ok === plain);

let failed = false;
try {
    crypto_decrypt({
        "name": "AES-GCM",
        "iv": sealed.iv,
        "tag": sealed.tag
    }, key, sealed.ciphertext);
} catch (e) {
    failed = true;
    assert(e.message !== undefined);
}
assert(failed === true);

crypto_destroy_key(key);
return 0;

//?
//? test: crypto_export_extractable
//? description: export only when extractable
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const key = crypto_generate_key("AES-GCM", undefined, true);
const raw = crypto_export_key(key);
// 32 raw octets -> 44 base64 characters with padding
assert(length(string(raw)) === 44);

const key2 = crypto_import_key(raw, "AES-GCM");
const plain = random_base64Binary(16);
const sealed = crypto_encrypt({ "name": "AES-GCM" }, key2, plain);
const out = crypto_decrypt({
    "name": "AES-GCM",
    "iv": sealed.iv,
    "tag": sealed.tag
}, key2, sealed.ciphertext);
assert(out === plain);

crypto_destroy_key(key);
crypto_destroy_key(key2);
return 0;
