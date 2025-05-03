#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: curl_easy_options.as
//? customPurpose: Part of afw_curl tests
//? description: Test curl curl_easy_options.
//? sourceType: script
//?
//? test: curl_easy_options
//? description: Call curl_easy_options with no parameters
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const options = curl_easy_options();

// make sure some options get returned
assert(length(options) > 0);

return 0;