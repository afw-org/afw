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
//? description: Call http_head against the local HTTP stub (see config.py)
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const response = http_head(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/get"
);

return response.response_code;


//? test: http_head_callbacks
//? description: Call http_head with a headerFunction callback against the local HTTP stub
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

const response = http_head(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/get",
    undefined,
    options
);

assert(length(userData.headers) > 0);

return response.response_code;
