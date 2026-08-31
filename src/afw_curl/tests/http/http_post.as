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


//? test: http_post_no_payload
//? description: payload is optional (#108) -- omitting it sends an empty body instead of raising a missing-parameter error.
//? expect: error:Could not resolve host: xyz
//? source: ...
#!/usr/bin/env afw

http_post("http://xyz");

//? test: http_post_bad_url
//? description: Call http_post with bad url
//? expect: error:Could not resolve host: xyz
//? source: ...
#!/usr/bin/env afw

http_post("http://xyz", "");

//? test: http_post_http_cleartext
//? description: Call http_post against the local HTTP stub (see config.py)
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const response = http_post(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/post",
    ""
);

return response.response_code;


//? test: http_post_google_secure
//? description: Call http_post with 200 rc against a real TLS endpoint
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// requires a real TLS endpoint; only run if configured to do so
if (environment::TEST_CURL_HTTPBIN === undefined) {
    return 200;
}

const response = http_post("https://www.httpbin.org/post",
    "xyz",
    undefined,
    {
        "sslVerifyPeer": true,
        "sslVerifyHost": true
    });

return response.response_code;


//? test: http_post_json
//? description: Call http_post with json data against the local HTTP stub
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const postData = {
    "foo": "bar"
};

const postHeaders = [
    "Content-Type: application/json",
    "Accept: application/json"
];


const response = http_post(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/post",
    string(postData),
    postHeaders
);

assert(length(response.headers) > 0);
assert(length(response.response) > 0);

const decoded = compile(json(decode_to_string(base64Binary(response.response))));

assert(decoded.headers.Accept === "application/json");
assert(decoded.json.foo === "bar");

return response.response_code;


//? test: http_post_callbacks
//? description: Call http_post with callbacks against the local HTTP stub
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// fixme: if I don't set the "payload" property first,
// then I get a valgrind/memory error; need to
// check memory pools
let userData = {
    "payload": "",
    "headers": []
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

function headers(header, userData) {
    const len = length(header);

    if (len > 0) {
        add_entries(userData.headers, header);
    }

    return len;
}

const postData = {
    "foo": "bar"
};

const postHeaders = [
    "Content-Type: application/json",
    "Accept: application/json"
];

const options = {
    "headerFunction": headers,
    "headerUserData": userData,
    "writeFunction": writer,
    "writeUserData": userData,
};

const response = http_post(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/post",
    string(postData),
    postHeaders,
    options
);

assert(length(userData.headers) > 0);
assert(length(userData.payload) > 0);

return response.response_code;


//? test: http_post_upload_stream
//? description: http_post() with payload omitted streams the body from options.readFunction (issue #108) via CURLOPT_UPLOAD instead of buffering it all in memory via CURLOPT_POSTFIELDS. readFunctionSize is supplied so libcurl sends a real Content-Length -- the stub reads raw bytes off Content-Length and doesn't decode chunked transfer encoding.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const bodyLen = 20000;
let body = "A";
while (length(body) < bodyLen) {
    body += body;
}
body = substring(body, 0, bodyLen);

let userData = {
    "body": body,
    "offset": 0,
    "calls": 0
};

function reader(userData, size, nitems) {
    if (userData.offset >= length(userData.body)) {
        return null;
    }

    // return chunks bigger than libcurl's offered capacity (size * nitems),
    // to exercise the pending_payload partial-drain path across calls
    const chunkSize = 8000;
    const remaining = length(userData.body) - userData.offset;
    const n = (remaining < chunkSize) ? remaining : chunkSize;
    const chunk = substring(userData.body, userData.offset, userData.offset + n);

    userData["offset"] = userData.offset + n;
    userData["calls"] = userData.calls + 1;

    return encode_as_hexBinary(chunk);
}

const response = http_post(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/post",
    undefined,
    undefined,
    {
        "readFunction": reader,
        "readUserData": userData,
        "readFunctionSize": bodyLen
    }
);

assert(userData.calls > 1);
assert(userData.offset === bodyLen);
assert(response.response_code === 200);

const decoded = compile(json(decode_to_string(base64Binary(response.response))));
assert(length(decoded.data) === bodyLen);

return 0;
