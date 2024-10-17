#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: http_post.as
//? customPurpose: Part of afw_curl tests
//? description: Test curl http_post.
//? sourceType: script
//?
//? test: http_post_no_params
//? description: Call http_post with no parameters
//? expect: error:'http_post' expects 1 required parameters
//? source: ...
#!/usr/bin/env afw

http_post();


//? test: http_post_bad_param
//? description: Call http_post with missing parameter
//? expect: error:Parameter 2 is undefined value
//? source: ...
#!/usr/bin/env afw

http_post("http://xyz");

//? test: http_post_bad_url
//? description: Call http_post with bad url
//? expect: error:Could not resolve host: xyz
//? source: ...
#!/usr/bin/env afw

http_post("http://xyz", "");

//? test: http_post_google
//? description: Call http_post with google.com
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const test_curl_http_post = environment::TEST_CURL_HTTP_POST;

// only test if environment variable is defined
if (test_curl_http_post === undefined) {
    return 200;
}

const response = http_post("http://www.google.com", "");

return response.response_code;


//? test: http_post_google_secure
//? description: Call http_post with https://google.com
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const test_curl_http_post = environment::TEST_CURL_HTTP_POST;

// only test if environment variable is defined
if (test_curl_http_post === undefined) {
    return 200;
}

const response = http_post("https://www.google.com", 
    "",
    undefined,
    { 
        "sslVerifyPeer": true, 
        "sslVerifyHost": true
    });

return response.response_code;