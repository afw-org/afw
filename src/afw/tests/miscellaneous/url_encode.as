#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: url_encode.as
//? customPurpose: Part of miscellaneous tests
//? description: url_encode encodes as ECMAScript encodeURI (#543)
//? sourceType: script
//?
//? test: equals-kept
//? description: = is kept like & (it was encoded, so a query string was half encoded)
//? expect: "a=b&c%20d"
//? source: ...

url_encode("a=b&c d"anyURI)

//? test: string
//? description: a string is encoded too (url_encode<string> threw, wanting an anyURI)
//? expect: "a=b&c%20d"
//? source: ...

"a=b&c d"->url_encode()

//? test: kept-set
//? description: every character encodeURI keeps is kept
//? expect: "AZaz09;,/?:@&=+$-_.!~*'()#"
//? source: ...

url_encode("AZaz09;,/?:@&=+$-_.!~*'()#")

//? test: encoded-set
//? description: space, % and other characters are percent-encoded as UTF-8
//? expect: "%20%25%22%3C%3E%5B%5D%7B%7D%7C%5C%5E%60%C3%A9"
//? source: ...

url_encode(" %\"<>[]{}|\\^`é")

//? test: url
//? description: a whole URL keeps its structure
//? expect: "https://example.com/a%20b?x=1&y=caf%C3%A9#top"
//? source: ...

url_encode("https://example.com/a b?x=1&y=café#top"anyURI)
