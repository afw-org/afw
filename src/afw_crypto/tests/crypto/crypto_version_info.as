#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: crypto_version_info.as
//? customPurpose: Part of afw_crypto tests
//? description: Test crypto_version_info.
//? sourceType: script
//?
//? test: crypto_version_info
//? description: Call crypto_version_info and check basic properties
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ver = crypto_version_info();
assert(ver.opensslVersion !== undefined);
assert(ver.extensionVersion !== undefined);
assert(length(ver.algorithms) >= 1);

return 0;
