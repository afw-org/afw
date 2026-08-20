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
