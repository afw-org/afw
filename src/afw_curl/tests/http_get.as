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

//? test: http_get_google
//? description: Call http_get with google.com
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const test_curl_http_get = environment::TEST_CURL_HTTP_GET;

// only test if environment variable is defined
if (test_curl_http_get === undefined) {
    return 200;
}

const response = http_get("http://www.google.com");

return response.response_code;


//? test: http_get_google_secure
//? description: Call http_get with https://google.com
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const test_curl_http_get = environment::TEST_CURL_HTTP_GET;

// only test if environment variable is defined
if (test_curl_http_get === undefined) {
    return 200;
}

const response = http_get("https://www.google.com", 
    undefined,
    { 
        "sslVerifyPeer": true, 
        "sslVerifyHost": true
    });

return response.response_code;