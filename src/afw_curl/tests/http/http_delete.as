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
//? description: Call http_delete with httpbin.org
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_delete("http://www.httpbin.org/delete");

return response.response_code;


//? test: http_delete_200
//? description: Call http_delete with 200 rc
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
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
//? description: Call http_delete with 404 rc
//? expect: 404
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 404;
}

const response = http_delete("https://www.httpbin.org/status/404");

return response.response_code;


//? test: http_delete_500
//? description: Call http_delete with 500 rc
//? expect: 500
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 500;
}

const response = http_delete("https://www.httpbin.org/status/500");

return response.response_code;