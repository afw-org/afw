#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/url_decode.as
//? customPurpose: Part of core function tests
//? description: url_decode decodes as ECMAScript decodeURI, the inverse of url_encode (#513)
//? sourceType: script
//?
//? test: url_decode-utf8
//? description: escapes are decoded as UTF-8, either case of hex digit
//? expect: "a b%é€"
//? source: ...

url_decode("a%20b%25%C3%A9%e2%82%ac")

//?
//? test: url_decode-kept
//? description: escapes of characters encodeURI keeps stay as they are
//? expect: "%3B%2F%3F%3A%40%26%3D%2B%24%2C%23%2f"
//? source: ...

url_decode("%3B%2F%3F%3A%40%26%3D%2B%24%2C%23%2f")

//?
//? test: url_decode-unreserved
//? description: escapes of other ASCII characters are decoded
//? expect: "A-_.!~*'()"
//? source: ...

url_decode("%41%2D%5F%2E%21%7E%2A%27%28%29")

//?
//? test: url_decode-no-escapes
//? description: a string with no escapes is returned as is
//? expect: "http://x/a?b=c#d"
//? source: ...

url_decode("http://x/a?b=c#d")

//?
//? test: url_decode-round-trip
//? description: url_decode undoes url_encode
//? expect: "http://x/a b?q=é&r=€#f"
//? source: ...

url_decode(url_encode("http://x/a b?q=é&r=€#f"))

//?
//? test: url_decode-method
//? description: url_decode is a string method
//? expect: "a b"
//? source: ...

"a%20b"->url_decode()

//?
//? test: url_decode-error-short
//? description: a % without two hex digits is an error
//? expect: error:Invalid percent-encoding in URI
//? source: ...

url_decode("a%2")

//?
//? test: url_decode-error-not-hex
//? description: a % followed by non-hex is an error
//? expect: error:Invalid percent-encoding in URI
//? source: ...

url_decode("%zz")

//?
//? test: url_decode-error-truncated-utf8
//? description: a truncated UTF-8 sequence is an error
//? expect: error:Percent-encoding in URI is not valid UTF-8
//? source: ...

url_decode("%C3")

//?
//? test: url_decode-error-kept-continuation
//? description: a kept character is not a UTF-8 continuation, as in decodeURI("%C3%23")
//? expect: error:Percent-encoding in URI is not valid UTF-8
//? source: ...

url_decode("%C3%23")

//?
//? test: url_decode-error-overlong
//? description: an overlong encoding is an error
//? expect: error:Percent-encoding in URI is not valid UTF-8
//? source: ...

url_decode("%C0%AF")

//?
//? test: url_decode-error-surrogate
//? description: an encoded surrogate is an error
//? expect: error:Percent-encoding in URI is not valid UTF-8
//? source: ...

url_decode("%ED%A0%80")
