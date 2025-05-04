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
//? description: Call http_get
//? expect: 200
//? source: ...
#!/usr/bin/env afw


// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_get("http://www.httpbin.org/get");

return response.response_code;


//? test: http_get_secure
//? description: Call http_get secure
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
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
//? description: Call http_get secure
//? expect: 400
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 400;
}

const response = http_get("https://www.httpbin.org/status/400");

return response.response_code;


//? test: http_get_writer_cb
//? description: Call http_get with writer callback
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

// fixme: if I don't set the "payload" property first,
// then I get a valgrind/memory error; need to 
// check memory pools
let userData = {
    "payload": ""
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

const response = http_get(
    "http://www.httpbin.org/get", 
    undefined,
    undefined,
    undefined,
    undefined,
    writer,
    userData
);

//trace(userData);

return response.response_code;