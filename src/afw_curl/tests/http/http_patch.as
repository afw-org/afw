#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: http_patch.as
//? customPurpose: Part of afw_curl tests
//? description: Test curl http_patch.
//? sourceType: script
//?
//? test: http_patch_no_params
//? description: Call http_patch with no parameters
//? expect: error:'http_patch' expects 1 required parameters
//? source: ...
#!/usr/bin/env afw

http_patch();


//? test: http_patch_bad_url
//? description: Call http_patch with bad url
//? expect: error:Could not resolve host: xyz
//? source: ...
#!/usr/bin/env afw

http_patch("http://xyz", "");


//? test: http_patch_http_cleartext
//? description: Call http_patch against the local HTTP stub (see config.py)
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const response = http_patch(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/patch",
    "xyz"
);

return response.response_code;


//? test: http_patch_200
//? description: Call http_patch with 200 rc against a real TLS endpoint
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// requires a real TLS endpoint; only run if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_patch("https://www.httpbin.org/patch","xyz",,
    {
        "sslVerifyPeer": true,
        "sslVerifyHost": true
    });

return response.response_code;

//? test: http_patch_404
//? description: Call http_patch against the local HTTP stub, forcing a 404
//? expect: 404
//? source: ...
#!/usr/bin/env afw

const response = http_patch(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/status/404",
    "xyz"
);

return response.response_code;


//? test: http_patch_500
//? description: Call http_patch against the local HTTP stub, forcing a 500
//? expect: 500
//? source: ...
#!/usr/bin/env afw

const response = http_patch(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/status/500",
    "xyz"
);

return response.response_code;


//? test: http_patch_callbacks
//? description: Call http_patch with callbacks against the local HTTP stub
//? expect: 200
//? source: ...
#!/usr/bin/env afw

let userData = {
    "payload": "",
    "headers": []
};

function writer(buffer, userData) {
    const str = decode_to_string(buffer);
    const len = length(str);

    if (userData.payload !== undefined)
        userData["payload"] += str;
    else
        userData["payload"] = str;

    return len;
}

function headers(header, userData) {
    const len = length(header);

    if (len > 0) {
        add_entries(userData.headers, header);
    }

    return len;
}

const options = {
    "headerFunction": headers,
    "headerUserData": userData,
    "writeFunction": writer,
    "writeUserData": userData,
};

const response = http_patch(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/patch",
    "xyz",
    undefined,
    options
);

assert(length(userData.headers) > 0);
assert(length(userData.payload) > 0);

return response.response_code;
