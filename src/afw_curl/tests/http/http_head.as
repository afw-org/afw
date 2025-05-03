#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: http_head.as
//? customPurpose: Part of afw_curl tests
//? description: Test curl http_head.
//? sourceType: script
//?
//? test: http_head_no_params
//? description: Call http_head with no parameters
//? expect: error:'http_head' expects 1 required parameters
//? source: ...
#!/usr/bin/env afw

http_head();


//? test: http_head_bad_url
//? description: Call http_head with bad url
//? expect: error:Could not resolve host: xyz
//? source: ...
#!/usr/bin/env afw

http_head("http://xyz");


//? test: http_head
//? description: Call http_head with https://api.reqbin.com/
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_head("http://wwww.httpbin.org/get");

return response.response_code;