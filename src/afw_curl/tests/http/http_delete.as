#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: http_delete.as
//? customPurpose: Part of afw_curl tests
//? description: Test curl http_delete.
//? sourceType: script
//?
//? test: http_delete_no_params
//? description: Call http_delete with no parameters
//? expect: error:'http_delete' expects 1 required parameters
//? source: ...
#!/usr/bin/env afw

http_delete();


//? test: http_delete_bad_url
//? description: Call http_delete with bad url
//? expect: error:Could not resolve host: xyz
//? source: ...
#!/usr/bin/env afw

http_delete("http://xyz");


//? test: http_delete_http_cleartext
//? description: Call http_delete against the local HTTP stub (see config.py)
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const response = http_delete(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/delete"
);

return response.response_code;


//? test: http_delete_200
//? description: Call http_delete with 200 rc against a real TLS endpoint
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// requires a real TLS endpoint; only run if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_delete("https://www.httpbin.org/delete",,
    {
        "sslVerifyPeer": true,
        "sslVerifyHost": true
    });

return response.response_code;

//? test: http_delete_404
//? description: Call http_delete against the local HTTP stub, forcing a 404
//? expect: 404
//? source: ...
#!/usr/bin/env afw

const response = http_delete(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/status/404"
);

return response.response_code;


//? test: http_delete_500
//? description: Call http_delete against the local HTTP stub, forcing a 500
//? expect: 500
//? source: ...
#!/usr/bin/env afw

const response = http_delete(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/status/500"
);

return response.response_code;


//? test: http_delete_callbacks
//? description: Call http_delete with callbacks against the local HTTP stub
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

const response = http_delete(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/delete",
    undefined,
    options
);

assert(length(userData.headers) > 0);
assert(length(userData.payload) > 0);

return response.response_code;
