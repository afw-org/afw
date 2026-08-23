#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: http_get.as
//? customPurpose: Part of afw_curl tests
//? description: Test curl http_get.
//? sourceType: script
//?
//? test: http_get_no_params
//? description: Call http_get with no parameters
//? expect: error:'http_get' expects 1 required parameters
//? source: ...
#!/usr/bin/env afw

http_get();


//? test: http_get_bad_url
//? description: Call http_get with bad url
//? expect: error:Could not resolve host: xyz
//? source: ...
#!/usr/bin/env afw

http_get("http://xyz");

//? test: http_get
//? description: Call http_get against the local HTTP stub (see config.py)
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const response = http_get(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/get"
);

/* make sure we get at least the 200 OK header back */
assert(includes(response.headers, "HTTP/1.1 200 OK\r\n"));

return response.response_code;


//? test: http_get_secure
//? description: Call http_get secure against a real TLS endpoint
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// requires a real TLS endpoint; only run if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_get("https://www.httpbin.org/get",
    undefined,
    {
        "sslVerifyPeer": true,
        "sslVerifyHost": true
    });

return response.response_code;


//? test: http_get_400
//? description: Call http_get against the local HTTP stub, forcing a 400
//? expect: 400
//? source: ...
#!/usr/bin/env afw

const response = http_get(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/status/400"
);

return response.response_code;


//? test: http_get_callbacks
//? description: Call http_get with callbacks against the local HTTP stub
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// fixme: if I don't set the "payload" property first,
// then I get a valgrind/memory error; need to
// check memory pools
let userData = {
    "payload": "",
    "headers": []
};

function writer(buffer, userData) {
    const str = decode_to_string(buffer);
    const len = length(str);

    // here, we could read the string and do something
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

const response = http_get(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/get",
    undefined,
    options
);

assert(length(userData.headers) > 0);
assert(length(userData.payload) > 0);

return response.response_code;
