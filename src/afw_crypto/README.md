# afw_crypto

Optional Adaptive Framework extension (`libafwcrypto`) providing cryptographic
Adaptive Script functions via **OpenSSL libcrypto**.

See also: repository design notes in `designs/secrets-and-afw-crypto.md` (issue #74).

## Load

```json
{
    "type": "application",
    "extensions": ["afw_crypto"]
}
```

Or `extension_load("afw_crypto")` when the host allows it. Load the extension
**before** any conf template that calls `crypto_*`.

Build requires OpenSSL headers and `libcrypto` (`libssl-dev` / `openssl-devel`).
The srcdir is optional (`optionalChoiceDefault: true` in `afw-package.json`).

## Functions

| Function | Role |
|----------|------|
| `crypto_version_info` | OpenSSL + extension version, algorithm list |
| `crypto_digest` | SHA-256 / SHA-512 (`pure`; no execute access) |
| `crypto_hmac` / `crypto_hmac_verify` | HMAC-SHA-256 / HMAC-SHA-512 |
| `crypto_import_key` / `crypto_generate_key` / `crypto_export_key` / `crypto_destroy_key` | Process keystore |
| `crypto_encrypt` / `crypto_decrypt` | AES-GCM (auto IV; tag on algorithm object) — **hard path** |
| `crypto_seal` / `crypto_unseal` | AES-GCM convenience bag — **easy path** |
| `crypto_derive_key` | PBKDF2-HMAC-SHA256 (default 600000 iterations, min 100000) |

Binary parameters accept **base64Binary** or **hexBinary**. Keys may be:

```adaptive
rawKeyBytes                                    // base64Binary / hexBinary
cryptoKeyObject                                // has keyId from import/generate
{ "from": "environment", "name": "APP_KEY", "encoding": "base64" }  // live getenv
{ "from": "file", "path": "secrets/app.key" }  // under rootFilePaths
{ "from": "material", "data": keyBytes }
```

Environment key refs use **live `getenv`**, not the ambient `environment::` snapshot.

## Passwords and UTF-8 text

Encrypt/decrypt/seal/unseal work on **octets** (`base64Binary` / `hexBinary`).

- **`string(binary)`** / display formatting → base64 (or hex) **printable text**, not the UTF-8 of the octets. That is why **`decode_to_string(binary)`** exists.
- **`decode_to_string(binary)`** → octets as UTF-8 (throws if invalid). Use for passwords.
- **`encode_as_base64Binary(text)`** → UTF-8 string as binary before encrypt/seal.
- **`stringify(object)`** → Adaptive/JSON-like text for an object (1st arg; optional 3rd = whitespace). The 2nd (replacer) is not implemented yet.

### Easy path: `crypto_seal` / `crypto_unseal`

```adaptive
const key = crypto_generate_key("AES-GCM");
const sealed = crypto_seal(key, encode_as_base64Binary("secret-pass"));
const password = decode_to_string(crypto_unseal(key, sealed));

/* Portable file: pure JSON with base64 *strings* (not typed binary) */
const bag = {
    "algorithm": sealed.algorithm,
    "iv": string(sealed.iv),
    "tag": string(sealed.tag),
    "ciphertext": string(sealed.ciphertext)
};
const jsonText = stringify(bag);
const password2 = decode_to_string(crypto_unseal(key, jsonText));
crypto_destroy_key(key);
```

### Hard path: `crypto_encrypt` / `crypto_decrypt`

```adaptive
const key = crypto_generate_key("AES-GCM");
const sealed = crypto_encrypt(
    { "name": "AES-GCM" },
    key,
    encode_as_base64Binary("secret-pass")
);
const plain = crypto_decrypt(
    { "name": "AES-GCM", "iv": sealed.iv, "tag": sealed.tag },
    key,
    sealed.ciphertext
);
const password = decode_to_string(plain);
crypto_destroy_key(key);
```

## LDAP `bindParameters` (template → object)

LDAP adapter **`bindParameters`** is a **template** evaluated at adapter start. Template evaluation rule:

| Template content | Result type |
|------------------|-------------|
| **Only** one `${…}` / `#{…}` (no other text) | Type of the substitution (can be an **object**) |
| Any surrounding text or multiple parts | Always **string** (concat) |

So conf can supply the whole bind object from a single substitution that decrypts a sealed secret (password not stored cleartext in conf; seal **key** may live in the process environment):

```text
"bindParameters": "${
  const key = crypto_import_key({
      \"from\": \"environment\",
      \"name\": \"AFW_SEAL_KEY\",
      \"encoding\": \"base64\"
  }, \"AES-GCM\");
  /* read sealed iv / tag / ciphertext from a file under rootFilePaths … */
  /* easy: crypto_unseal(key, sealedObjectOrJsonString)
     hard: crypto_decrypt({ name, iv, tag }, key, ciphertext) */
  return {
      \"dn\": \"cn=service,dc=example,dc=org\",
      \"password\": decode_to_string(plain)
  };
}"
```

Footgun: `"x${return { … };}"` becomes a **string**, not an object — LDAP will reject it.

Regression tests: `tests/crypto/crypto_bind_parameters_template.as`.

## Admin / Fiddle

After load, functions appear under **Documentation → Reference → Functions → crypto**
(same `_AdaptiveFunction_` objects as core). Fiddle can evaluate them via the
action/HTTP path (`POST /afw`).

## Interactive `readpass`

Not provided by this extension. Prefer sealed files + decrypt (or env for the
**key**, not the password) for non-interactive hosts.
