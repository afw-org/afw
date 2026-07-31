#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: crypto_bind_parameters_template.as
//? customPurpose: Part of afw_crypto tests
//? description: ...
Issue #74 bindParameters-style secrets: a conf property typed as template can
be a single ${...} substitution that evaluates to an object (not a string).
That is how LDAP adapter bindParameters is meant to supply { dn, password }
without embedding a cleartext password in conf. These tests exercise that
template rule plus open_file/read + crypto_decrypt composition.
//? sourceType: script
//?
//? test: template_single_substitution_returns_object
//? description: ...
Pure single-substitution template with no surrounding text preserves the
data type of the substitution result (object).
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const bindParameters = evaluate(compile(template(
    "${return { \"dn\": \"cn=service,dc=example,dc=org\", \"password\": \"not-secret\" };}"
)));

assert(bindParameters.dn === "cn=service,dc=example,dc=org");
assert(bindParameters.password === "not-secret");

return 0;

//?
//? test: template_with_surrounding_text_returns_string
//? description: ...
Any literal text (or multiple parts) forces string concatenation, so the
result is not an object. This is the easy footgun for bindParameters.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const s = evaluate(compile(template(
    "x${return { \"dn\": \"cn=x\" };}"
)));

// Concatenation path: object was stringified into the result, not kept as object
assert(string(s) !== undefined);
// Must not be a bind-shaped object (property access would succeed on objects only)
let looksLikeObject = false;
try {
    /* if s were an object, .dn would be "cn=x"; on string this throws */
    if (s.dn === "cn=x") {
        looksLikeObject = true;
    }
} catch (e) {
    looksLikeObject = false;
}
assert(looksLikeObject === false);

const s2 = evaluate(compile(template(
    "prefix-${return \"secret-pass\";}"
)));
assert(s2 === "prefix-secret-pass");

return 0;

//?
//? test: bindParameters_template_decrypt_password
//? description: ...
bindParameters-shaped template body: seal a password as AES-GCM binary,
decrypt inside a single ${...} script, convert octets to UTF-8 via stream
write/read, return { dn, password } object. Models the LDAP conf pattern
without putting the cleartext password in conf or environment.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const bindParameters = evaluate(compile(template("${
const key = crypto_generate_key(\"AES-GCM\", undefined, true);
/* UTF-8 octets of secret-pass as base64Binary */
const passwordOctets = base64Binary(\"c2VjcmV0LXBhc3M=\");
const sealed = crypto_encrypt({ \"name\": \"AES-GCM\" }, key, passwordOctets);
const plain = crypto_decrypt({
    \"name\": \"AES-GCM\",
    \"iv\": sealed.iv,
    \"tag\": sealed.tag
}, key, sealed.ciphertext);
/* binary -> UTF-8 string (string(base64Binary) is base64 text, not octets) */
const w = open_file(\"bp-w\", \"data/bind_pw.bin\", \"wb\");
write_internal(w, plain);
close(w);
const r = open_file(\"bp-r\", \"data/bind_pw.bin\", \"r\");
const bindPassword = read(r, 1024);
close(r);
crypto_destroy_key(key);
return {
    \"dn\": \"cn=service,dc=example,dc=org\",
    \"password\": bindPassword
};
}")));

assert(bindParameters.dn === "cn=service,dc=example,dc=org");
assert(bindParameters.password === "secret-pass");

return 0;

//?
//? test: bindParameters_template_file_sealed_blob
//? description: ...
Write a sealed password blob to a rootFilePaths file first, then a
bindParameters-style template reads the file, decrypts with a key from
environment (live getenv key ref), and returns the bind object. Closest to
ops: sealed file on disk, seal key in env, conf template only.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

/* Known 32-octet key as base64 (not the LDAP password). */
const sealKeyMaterial = base64Binary(
    "MDEyMzQ1Njc4OWFiY2RlZjAxMjM0NTY3ODlhYmNkZWY="
); /* 0123456789abcdef0123456789abcdef */

/* Produce sealed blob and store under data/ for the template to read. */
const key = crypto_import_key(sealKeyMaterial, "AES-GCM", undefined, true);
const passwordOctets = base64Binary("c2VjcmV0LXBhc3M="); /* secret-pass */
const sealed = crypto_encrypt({ "name": "AES-GCM" }, key, passwordOctets);

/* Store as simple Adaptive-ish lines: iv, tag, ciphertext as base64 text */
const out = open_file("seal-w", "data/ldap_bind.sealed.txt", "w");
writeln(out, string(sealed.iv));
writeln(out, string(sealed.tag));
writeln(out, string(sealed.ciphertext));
close(out);
crypto_destroy_key(key);

/*
 * Template body only uses functions + environment:: (no outer locals).
 * Env AFW_CRYPTO_TEST_SEAL_KEY is set by the test harness (config.py).
 */
const bindParameters = evaluate(compile(template("${
const key = crypto_import_key({
    \"from\": \"environment\",
    \"name\": \"AFW_CRYPTO_TEST_SEAL_KEY\",
    \"encoding\": \"base64\"
}, \"AES-GCM\");
const f = open_file(\"seal-r\", \"data/ldap_bind.sealed.txt\", \"r\");
const ivB64 = readln(f);
const tagB64 = readln(f);
const ctB64 = readln(f);
close(f);
const plain = crypto_decrypt({
    \"name\": \"AES-GCM\",
    \"iv\": base64Binary(ivB64),
    \"tag\": base64Binary(tagB64)
}, key, base64Binary(ctB64));
const w = open_file(\"bp-w2\", \"data/bind_pw2.bin\", \"wb\");
write_internal(w, plain);
close(w);
const r = open_file(\"bp-r2\", \"data/bind_pw2.bin\", \"r\");
const bindPassword = read(r, 1024);
close(r);
crypto_destroy_key(key);
return {
    \"dn\": \"cn=service,dc=example,dc=org\",
    \"password\": bindPassword
};
}")));

assert(bindParameters.dn === "cn=service,dc=example,dc=org");
assert(bindParameters.password === "secret-pass");

return 0;
