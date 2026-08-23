#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: smtp_send_upload.as
//? customPurpose: Part of afw_curl tests
//? description: Regression for C1 -- curl upload READFUNCTION ignored the buffer size libcurl offered, overflowing its buffer. Runs against a local SMTP stub (see config.py) that verifies the exact byte count received.
//? sourceType: script
//?
//? test: smtp_upload_small
//? description: Baseline sanity -- a payload well under any read buffer, single READFUNCTION call. Confirms the stub/harness itself works.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const bodyLen = 50;
let body = "A";
while (length(body) < bodyLen) {
    body += body;
}
body = substring(body, 0, bodyLen);

smtp_send(
    "smtp://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_SMTP_PORT)),
    "stub-len-" + string(bodyLen) + "@example.com",
    ["recipient@example.com"],
    body
);

return 0;


//? test: smtp_upload_read_callback
//? description: smtp_send() with a custom readFunction (see afw_curl_internal_request_cb), streaming the body from script-controlled chunks instead of the internal single-shot buffer copy. Chunks are returned larger than libcurl's offered read-buffer capacity, exercising the pending_payload partial-drain path.
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

    // return a chunk far bigger than libcurl's offered capacity (size * nitems),
    // to exercise the pending_payload partial-drain path across calls
    const chunkSize = 8000;
    const remaining = length(userData.body) - userData.offset;
    const n = (remaining < chunkSize) ? remaining : chunkSize;
    const chunk = substring(userData.body, userData.offset, userData.offset + n);

    userData["offset"] = userData.offset + n;
    userData["calls"] = userData.calls + 1;

    return encode_as_hexBinary(chunk);
}

smtp_send(
    "smtp://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_SMTP_PORT)),
    "stub-len-" + string(bodyLen) + "@example.com",
    ["recipient@example.com"],
    "",
    {
        "readFunction": reader,
        "readUserData": userData
    }
);

assert(userData.calls > 1);
assert(userData.offset === bodyLen);

return 0;


//? test: smtp_upload_multi_buffer
//? description: Payload far larger than libcurl's internal upload buffer, forcing multiple READFUNCTION invocations. This is the case that crashed before the C1 fix.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const bodyLen = 500000;
let body = "A";
while (length(body) < bodyLen) {
    body += body;
}
body = substring(body, 0, bodyLen);

smtp_send(
    "smtp://127.0.0.1:" + string(integer(environment::AFW_CURL_TEST_SMTP_PORT)),
    "stub-len-" + string(bodyLen) + "@example.com",
    ["recipient@example.com"],
    body
);

return 0;
