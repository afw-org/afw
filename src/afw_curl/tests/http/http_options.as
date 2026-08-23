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
//? description: Call http_options against the local HTTP stub (see config.py)
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const response = http_options(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/get"
);

return response.response_code;


//? test: http_options_callbacks
//? description: Call http_options with a headerFunction callback against the local HTTP stub
//? expect: 200
//? source: ...
#!/usr/bin/env afw

let userData = {
    "headers": []
};

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
};

const response = http_options(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/get",
    undefined,
    options
);

assert(length(userData.headers) > 0);

return response.response_code;
