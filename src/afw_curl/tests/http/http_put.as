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
//? description: Call http_put against the local HTTP stub (see config.py)
//? expect: 200
//? source: ...
#!/usr/bin/env afw

const response = http_put(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/put",
    "xyz"
);

return response.response_code;


//? test: http_put_200
//? description: Call http_put with 200 rc against a real TLS endpoint
//? expect: 200
//? source: ...
#!/usr/bin/env afw

// requires a real TLS endpoint; only run if configured to do so
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
//? description: Call http_put against the local HTTP stub, forcing a 404
//? expect: 404
//? source: ...
#!/usr/bin/env afw

const response = http_put(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/status/404",
    "xyz"
);

return response.response_code;


//? test: http_put_500
//? description: Call http_put against the local HTTP stub, forcing a 500
//? expect: 500
//? source: ...
#!/usr/bin/env afw

const response = http_put(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/status/500",
    "xyz"
);

return response.response_code;


//? test: http_put_callbacks
//? description: Call http_put with callbacks against the local HTTP stub
//? expect: 200
//? source: ...
#!/usr/bin/env afw

let userData = {
    "payload": "",
    "headers": []
};

function writer(buffer, userData) {
    const str = decode_to_string(buffer);
    const len = length(str);

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

const options = {
    "headerFunction": headers,
    "headerUserData": userData,
    "writeFunction": writer,
    "writeUserData": userData,
};

const response = http_put(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/put",
    "xyz",
    undefined,
    options
);

assert(length(userData.headers) > 0);
assert(length(userData.payload) > 0);

return response.response_code;


//? test: http_put_upload_stream
//? description: http_put() with payload omitted streams the body from options.readFunction (issue #108) via CURLOPT_UPLOAD instead of buffering it all in memory via CURLOPT_POSTFIELDS. readFunctionSize is supplied so libcurl sends a real Content-Length -- the stub reads raw bytes off Content-Length and doesn't decode chunked transfer encoding.
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

const response = http_put(
    "http://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_HTTP_PORT)) + "/put",
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
