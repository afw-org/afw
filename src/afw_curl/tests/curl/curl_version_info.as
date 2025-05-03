#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: curl_version_info.as
//? customPurpose: Part of afw_curl tests
//? description: Test curl curl_version_info.
//? sourceType: script
//?
//? test: curl_version_info
//? description: Call curl_version_info with no parameters
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ver = curl_version_info();

// there may be lots of properties, but the version should exist
assert(ver.version !== undefined);

return 0;