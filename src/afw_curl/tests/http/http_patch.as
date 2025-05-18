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
//? description: Call http_patch with httpbin.org
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_patch("http://www.httpbin.org/patch", "xyz");

return response.response_code;


//? test: http_patch_200
//? description: Call http_patch with 200 rc
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
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
//? description: Call http_patch with 404 rc
//? expect: 404
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 404;
}

const response = http_patch("https://www.httpbin.org/status/404", "xyz");

return response.response_code;


//? test: http_patch_500
//? description: Call http_patch with 500 rc
//? expect: 500
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 500;
}

const response = http_patch("https://www.httpbin.org/status/500", "xyz");

return response.response_code;