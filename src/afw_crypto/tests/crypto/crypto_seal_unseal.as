#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: crypto_seal_unseal.as
//? customPurpose: Part of afw_crypto tests
//? description: ...
Easy path (crypto_seal / crypto_unseal) and hard path (encrypt/decrypt +
manual fields / stringify) for sealed secrets.
//? sourceType: script
//?
//? test: easy_seal_unseal_object
//? description: crypto_seal then crypto_unseal on the sealed object
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const key = crypto_generate_key("AES-GCM");
const sealed = crypto_seal(key, encode_as_base64Binary("secret-pass"));
assert(sealed.algorithm === "AES-GCM");
assert(sealed.iv !== undefined);
assert(sealed.tag !== undefined);
assert(sealed.ciphertext !== undefined);

const plain = crypto_unseal(key, sealed);
assert(decode_to_string(plain) === "secret-pass");
crypto_destroy_key(key);
return 0;

//?
//? test: easy_seal_unseal_json_string
//? description: ...
JSON-friendly bag (base64 strings) via stringify, then unseal from JSON
string — portable file/conf shape without Adaptive base64Binary() literals.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const key = crypto_generate_key("AES-GCM");
const sealed = crypto_seal(key, encode_as_base64Binary("secret-pass"));

/* Pure JSON: iv/tag/ciphertext as base64 *strings* (not typed Adaptive binary) */
const bag = {
    "algorithm": sealed.algorithm,
    "keyLength": sealed.keyLength,
    "iv": string(sealed.iv),
    "tag": string(sealed.tag),
    "ciphertext": string(sealed.ciphertext)
};
const jsonText = stringify(bag);

const plain = crypto_unseal(key, jsonText);
assert(decode_to_string(plain) === "secret-pass");

/* Also unseal after compile(json(...)) → object with string fields */
const o = evaluate(compile(json(jsonText)));
assert(decode_to_string(crypto_unseal(key, o)) === "secret-pass");

crypto_destroy_key(key);
return 0;

//?
//? test: easy_seal_file_roundtrip
//? description: seal → write file → read → unseal
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const key = crypto_generate_key("AES-GCM");
const sealed = crypto_seal(key, encode_as_base64Binary("file-secret"));
const bag = {
    "algorithm": sealed.algorithm,
    "iv": string(sealed.iv),
    "tag": string(sealed.tag),
    "ciphertext": string(sealed.ciphertext)
};
const sn = open_file("seal-out", "data/easy_seal.json", "w");
write(sn, stringify(bag));
close(sn);

const sn2 = open_file("seal-in", "data/easy_seal.json", "r");
const jsonText = read(sn2, 65536);
close(sn2);

assert(decode_to_string(crypto_unseal(key, jsonText)) === "file-secret");
crypto_destroy_key(key);
return 0;

//?
//? test: hard_encrypt_decrypt_manual
//? description: hard path — crypto_encrypt / crypto_decrypt with algorithm bag
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const key = crypto_generate_key("AES-GCM");
const sealed = crypto_encrypt(
    { "name": "AES-GCM" },
    key,
    encode_as_base64Binary("hard-way")
);
const plain = crypto_decrypt({
    "name": "AES-GCM",
    "iv": sealed.iv,
    "tag": sealed.tag
}, key, sealed.ciphertext);
assert(decode_to_string(plain) === "hard-way");
crypto_destroy_key(key);
return 0;

//?
//? test: hard_line_file_format
//? description: hard path — line-oriented iv/tag/ct file then manual decrypt
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const key = crypto_generate_key("AES-GCM");
const sealed = crypto_encrypt(
    { "name": "AES-GCM" },
    key,
    encode_as_base64Binary("line-secret")
);
const out = open_file("hard-w", "data/hard_seal.txt", "w");
writeln(out, string(sealed.iv));
writeln(out, string(sealed.tag));
writeln(out, string(sealed.ciphertext));
close(out);

const f = open_file("hard-r", "data/hard_seal.txt", "r");
const ivB64 = readln(f);
const tagB64 = readln(f);
const ctB64 = readln(f);
close(f);

const plain = crypto_decrypt({
    "name": "AES-GCM",
    "iv": base64Binary(ivB64),
    "tag": base64Binary(tagB64)
}, key, base64Binary(ctB64));
assert(decode_to_string(plain) === "line-secret");
crypto_destroy_key(key);
return 0;
