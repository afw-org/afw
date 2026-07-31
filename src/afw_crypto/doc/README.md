# afw_crypto

Optional Adaptive Framework extension providing cryptographic Adaptive Script functions via **OpenSSL libcrypto**.

## Load

```json
{
  "type": "application",
  "extensions": ["afw_crypto"]
}
```

Or `extension_load("afw_crypto")` when your host allows it.

## Functions (v1)

| Function | Role |
|----------|------|
| `crypto_version_info` | OpenSSL + extension version, algorithm list |
| `crypto_digest` | SHA-256 / SHA-512 |
| `crypto_hmac` / `crypto_hmac_verify` | HMAC-SHA-256 / HMAC-SHA-512 |
| `crypto_import_key` / `crypto_generate_key` / `crypto_export_key` / `crypto_destroy_key` | Process keystore |
| `crypto_encrypt` / `crypto_decrypt` | AES-GCM |
| `crypto_derive_key` | PBKDF2-HMAC-SHA256 (default 600000 iterations, min 100000) |

Binary parameters accept **base64Binary** or **hexBinary**. Keys may be raw binary, CryptoKey objects (`keyId`), or references:

```adaptive
{ "from": "environment", "name": "APP_KEY", "encoding": "base64" }
{ "from": "file", "path": "secrets/app.key" }  // under rootFilePaths
{ "from": "material", "data": keyBytes }
```

Environment key refs use **live `getenv`**, not the ambient `environment::` snapshot.

## Build dependency

Requires OpenSSL development headers and `libcrypto` (`libssl-dev` / `openssl-devel`). The srcdir is optional (`optionalChoiceDefault: true`).

## Design

See repository design doc `designs/secrets-and-afw-crypto.md` (issue #74).
