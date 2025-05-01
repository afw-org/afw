#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: http_options.as
//? customPurpose: Part of afw_curl tests
//? description: Test curl http_options.
//? sourceType: script
//?
//? test: http_options_no_params
//? description: Call http_options with no parameters
//? expect: error:'http_options' expects 1 required parameters
//? source: ...
#!/usr/bin/env afw

http_options();


//? test: http_options_bad_url
//? description: Call http_options with bad url
//? expect: error:Could not resolve host: xyz
//? source: ...
#!/usr/bin/env afw

http_options("http://xyz");


//? test: http_options
//? description: Call http_options with https://api.reqbin.com/
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// only do live HTTP requests, if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_options("http://wwww.httpbin.org/get");

return response.response_code;