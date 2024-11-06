#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: http_put.as
//? customPurpose: Part of afw_curl tests
//? description: Test curl http_put.
//? sourceType: script
//?
//? test: http_put_no_params
//? description: Call http_put with no parameters
//? expect: error:'http_put' expects 1 required parameters
//? source: ...
#!/usr/bin/env afw

http_put();


//? test: http_put_bad_url
//? description: Call http_put with bad url
//? expect: error:Could not resolve host: xyz
//? source: ...
#!/usr/bin/env afw

http_put("http://xyz", "");


//? test: http_put_http_cleartext
//? description: Call http_put with httpbin.org
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_put("http://www.httpbin.org/put", "xyz");

return response.response_code;


//? test: http_put_200
//? description: Call http_put with 200 rc
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_put("https://www.httpbin.org/put","xyz",,
    { 
        "sslVerifyPeer": true, 
        "sslVerifyHost": true
    });

return response.response_code;

//? test: http_put_404
//? description: Call http_put with 404 rc
//? expect: 404
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 404;
}

const response = http_put("https://www.httpbin.org/status/404", "xyz");

return response.response_code;


//? test: http_put_500
//? description: Call http_put with 500 rc
//? expect: 500
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 500;
}

const response = http_put("https://www.httpbin.org/status/500", "xyz");

return response.response_code;