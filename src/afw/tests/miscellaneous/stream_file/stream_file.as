#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: stream_file.as
//? customPurpose: Part of stream category tests (#103)
//? description: Test open_file, path resolve, stream I/O, binary, and Unicode
//? sourceType: script
//?
//? test: open_file-write-read-roundtrip
//? description: open_file write then read back content
//? expect: 0
//? source: ...

const sn = open_file("t1", "data/roundtrip.txt", "w");
assert(sn >= 0);
write(sn, "hello-stream");
flush(sn);
close(sn);

const sn2 = open_file("t1", "data/roundtrip.txt", "r");
const s = read(sn2, 100);
close(sn2);
assert(s === "hello-stream");

return 0;


//? test: open_file-readln
//? description: readln reads a line without newline
//? expect: 0
//? source: ...

let sn = open_file("t2", "data/lines.txt", "w");
writeln(sn, "line-one");
writeln(sn, "line-two");
close(sn);

sn = open_file("t2", "data/lines.txt", "r");
assert(readln(sn) === "line-one");
assert(readln(sn) === "line-two");
close(sn);

return 0;


//? test: open_file-reopen-same-streamId
//? description: close clears slot so streamId can reopen
//? expect: 0
//? source: ...

let sn = open_file("reopen", "data/reopen.txt", "w");
write(sn, "a");
close(sn);
sn = open_file("reopen", "data/reopen.txt", "w");
write(sn, "b");
close(sn);

sn = open_file("reopen", "data/reopen.txt", "r");
assert(read(sn, 10) === "b");
close(sn);

return 0;


//? test: open_file-already-open
//? description: opening same streamId twice throws
//? expect: error
//? source: ...

const sn = open_file("dup", "data/dup.txt", "w");
open_file("dup", "data/dup2.txt", "w");


//? test: open_file-fails-missing-file
//? description: open_file mode r throws when file does not exist (fopen/errno)
//? expect: error
//? source: ...

open_file("open-fail-missing", "data/does_not_exist_open_fail.txt", "r");


//? test: open_file-fails-missing-file-message
//? description: open_file missing file populates _AdaptiveError_ message and errno rv
//? expect: 0
//? source: ...

let caught = false;
let msg = "";
let id = "";
let rv = 0;
let rvDecoded = "";
let rvSourceId = "";
try {
    open_file("open-fail-msg", "data/does_not_exist_open_fail_msg.txt", "r");
}
catch (e) {
    caught = true;
    msg = e.message;
    id = e.id;
    rv = e.rv;
    rvDecoded = e.rvDecoded;
    rvSourceId = e.rvSourceId;
}
assert(caught);
/* Host path is absolute; check stable message pieces (includes strerror(errno)). */
assert(starts_with<string>(msg, "streamId 'open-fail-msg' failed to open "));
assert(ends_with<string>(msg, "No such file or directory"));
assert(id === "not_found");
assert(rvSourceId === "errno");
/* ENOENT is typically 2 on Linux. */
assert(rv === 2);
assert(rvDecoded === "No such file or directory");
return 0;


//? test: open_file-no-matching-prefix
//? description: path without rootFilePaths prefix fails
//? expect: error:Failed to resolve file location 'no-such-root/file.txt'.
//? source: ...

open_file("x", "no-such-root/file.txt", "r");


//? test: open_file-path-traversal
//? description: .. in logical path is rejected
//? expect: error
//? source: ...

open_file("x", "data/../include_root/x.txt", "r");


//? test: open_file-longest-prefix
//? description: includes beats include for includes/...
//? expect: 0
//? source: ...

const sn = open_file("lp", "includes/x.txt", "r");
const s = read(sn, 100);
close(sn);
assert(s === "from-includes\n" || s === "from-includes");
return 0;


//? test: open_file-include-prefix
//? description: include/ maps to include_root
//? expect: 0
//? source: ...

const sn = open_file("ip", "include/x.txt", "r");
const s = read(sn, 100);
close(sn);
assert(s === "from-include\n" || s === "from-include");
return 0;


//? test: stream-resolve-custom-id
//? description: stream() returns streamNumber for open custom streamId
//? expect: 0
//? source: ...

const sn = open_file("sid", "data/sid.txt", "w");
assert(stream("sid") === sn);
close(sn);
return 0;


//? test: stream-resolve-stdout
//? description: stream() resolves standard stdout
//? expect: 0
//? source: ...

const sn = stream("stdout");
assert(sn >= 0);
return 0;


//? test: stream-not-open-throws
//? description: stream() throws when streamId is not open
//? expect: error:streamId 'not-an-open-stream' is not open
//? source: ...

stream("not-an-open-stream");


//? test: stream-after-close-throws
//? description: stream() throws after close of custom streamId
//? expect: error:streamId 'closed-sid' is not open
//? source: ...

const sn = open_file("closed-sid", "data/closed_sid.txt", "w");
close(sn);
stream("closed-sid");


//? test: stream-stderr
//? description: stream() resolves standard stderr
//? expect: 0
//? source: ...

const sn = stream("stderr");
assert(sn >= 0);
assert(sn !== stream("stdout"));
return 0;


//? test: stream-console
//? description: stream() resolves standard console
//? expect: 0
//? source: ...

const sn = stream("console");
assert(sn >= 0);
return 0;


//? test: stream-try-catch-recover
//? description: try/catch can recover from stream() not open
//? expect: 0
//? source: ...

let caught = false;
let msg = "";
try {
    stream("definitely-missing");
}
catch (e) {
    caught = true;
    msg = e.message;
}
assert(caught);
assert(msg === "streamId 'definitely-missing' is not open");
/* Script continues; standard streams still work. */
assert(stream("stdout") >= 0);
return 0;


//? test: stream-reopen-after-close
//? description: stream() works again after reopen same streamId
//? expect: 0
//? source: ...

let sn = open_file("reopen-stream", "data/reopen_stream.txt", "w");
const n1 = stream("reopen-stream");
assert(n1 === sn);
close(sn);
sn = open_file("reopen-stream", "data/reopen_stream.txt", "w");
const n2 = stream("reopen-stream");
assert(n2 === sn);
write(sn, "ok");
close(sn);
return 0;


//? test: stream-other-id-unaffected-by-close
//? description: closing one streamId does not break another open stream
//? expect: 0
//? source: ...

const a = open_file("keep-a", "data/keep_a.txt", "w");
const b = open_file("drop-b", "data/drop_b.txt", "w");
assert(stream("keep-a") === a);
assert(stream("drop-b") === b);
close(b);
assert(stream("keep-a") === a);
write(a, "still-open");
close(a);
return 0;


//? test: stream-use-number-from-stream-for-write
//? description: stream() number is usable with write/read
//? expect: 0
//? source: ...

open_file("via-stream", "data/via_stream.txt", "w");
const sn = stream("via-stream");
write(sn, "by-number");
flush(sn);
close(sn);
open_file("via-stream", "data/via_stream.txt", "r");
assert(read(stream("via-stream"), 20) === "by-number");
close(stream("via-stream"));
return 0;


//? test: stream-empty-id-after-close-throws
//? description: stream() throws for empty streamId after close
//? expect: error:streamId '' is not open
//? source: ...

const sn = open_file("", "data/empty_sid2.txt", "w");
close(sn);
stream("");


//? test: write-multiple-values
//? description: write concatenates multiple value arguments
//? expect: 0
//? source: ...

let sn = open_file("mv", "data/multi.txt", "w");
write(sn, "a", "b", "c");
close(sn);
sn = open_file("mv", "data/multi.txt", "r");
assert(read(sn, 20) === "abc");
close(sn);
return 0;


//? test: writeln-appends-newline
//? description: writeln ends with newline; next line starts after
//? expect: 0
//? source: ...

let sn = open_file("wn", "data/writeln.txt", "w");
writeln(sn, "one");
writeln(sn, "two");
close(sn);
sn = open_file("wn", "data/writeln.txt", "r");
assert(readln(sn) === "one");
assert(readln(sn) === "two");
assert(read(sn, 10) === "");
close(sn);
return 0;


//? test: flush-before-close
//? description: flush can be called explicitly with autoFlush false
//? expect: 0
//? source: ...

const sn = open_file("fl", "data/flush.txt", "w", false);
write(sn, "flushed");
flush(sn);
close(sn);
const sn2 = open_file("fl", "data/flush.txt", "r");
assert(read(sn2, 20) === "flushed");
close(sn2);
return 0;


//? test: open_file-autoFlush
//? description: autoFlush true still produces correct file
//? expect: 0
//? source: ...

const sn = open_file("af", "data/autoflush.txt", "w", true);
write(sn, "auto");
close(sn);
const sn2 = open_file("af", "data/autoflush.txt", "r");
assert(read(sn2, 20) === "auto");
close(sn2);
return 0;


//? test: open_file-append-mode
//? description: mode a appends to existing content
//? expect: 0
//? source: ...

let sn = open_file("ap", "data/append.txt", "w");
write(sn, "first");
close(sn);
sn = open_file("ap", "data/append.txt", "a");
write(sn, "second");
close(sn);
sn = open_file("ap", "data/append.txt", "r");
assert(read(sn, 50) === "firstsecond");
close(sn);
return 0;


//? test: read-zero-octets
//? description: read with n=0 returns empty string
//? expect: 0
//? source: ...

let sn = open_file("z0", "data/zero.txt", "w");
write(sn, "xyz");
close(sn);
sn = open_file("z0", "data/zero.txt", "r");
assert(read(sn, 0) === "");
assert(read(sn, 3) === "xyz");
close(sn);
return 0;


//? test: read-partial-and-eof
//? description: partial read then remaining; further read is empty
//? expect: 0
//? source: ...

let sn = open_file("pe", "data/partial.txt", "w");
write(sn, "abcdef");
close(sn);
sn = open_file("pe", "data/partial.txt", "r");
assert(read(sn, 2) === "ab");
assert(read(sn, 10) === "cdef");
assert(read(sn, 10) === "");
close(sn);
return 0;


//? test: readln-eof-empty
//? description: readln at EOF on empty file returns empty string
//? expect: 0
//? source: ...

let sn = open_file("ee", "data/empty_file.txt", "w");
close(sn);
sn = open_file("ee", "data/empty_file.txt", "r");
assert(readln(sn) === "");
close(sn);
return 0;


//? test: write_internal-string
//? description: write_internal with string
//? expect: 0
//? source: ...

let sn = open_file("wis", "data/wi_str.txt", "w");
write_internal(sn, "raw-string");
close(sn);
sn = open_file("wis", "data/wi_str.txt", "r");
assert(read(sn, 50) === "raw-string");
close(sn);
return 0;


//? test: write_internal-and-read_to_hexBinary
//? description: write_internal hexBinary and read_to_hexBinary round-trip
//? expect: 0
//? source: ...

const payload = hexBinary("48656c6c6f"); /* Hello */
let sn = open_file("wh", "data/hex.bin", "wb");
write_internal(sn, payload);
close(sn);

sn = open_file("wh", "data/hex.bin", "rb");
const got = read_to_hexBinary(sn, 100);
close(sn);
assert(eq<hexBinary>(got, hexBinary("48656c6c6f")));
return 0;


//? test: read_to_base64Binary-roundtrip
//? description: write bytes then read_to_base64Binary
//? expect: 0
//? source: ...

const payload = hexBinary("010203ff");
let sn = open_file("wb64", "data/b64.bin", "wb");
write_internal(sn, payload);
close(sn);

sn = open_file("wb64", "data/b64.bin", "rb");
const got = read_to_base64Binary(sn, 100);
close(sn);
/* base64Binary internal is same octets as written */
assert(eq<base64Binary>(got, base64Binary("AQID/w==")));
return 0;


//? test: write_internal-invalid-type
//? description: write_internal rejects non string/binary types
//? expect: error
//? source: ...

const sn = open_file("wibad", "data/wibad.txt", "w");
write_internal(sn, 42);


//? test: invalid-streamNumber-write
//? description: write with bad streamNumber throws
//? expect: error:Invalid streamNumber 99999
//? source: ...

write(99999, "nope");


//? test: invalid-streamNumber-read
//? description: read with bad streamNumber throws
//? expect: error:Invalid streamNumber 99999
//? source: ...

read(99999, 10);


//? test: invalid-streamNumber-flush
//? description: flush with bad streamNumber throws
//? expect: error:Invalid streamNumber 99999
//? source: ...

flush(99999);


//? test: invalid-streamNumber-close
//? description: close with bad streamNumber throws
//? expect: error:Invalid streamNumber 99999
//? source: ...

close(99999);


//? test: close-standard-stream-rejected
//? description: close of stdout via close() is rejected
//? expect: error:Cannot close a standard stream with close()
//? source: ...

close(stream("stdout"));






//? test: stream-stdout-print
//? description: print still works
//? expect: 0
//? expect-stdout: ok
//? source: ...

print("ok");
return 0;


//? test: stream-stdout-println
//? description: println still works
//? expect: 0
//? expect-stdout: ...
ok-line

//? source: ...

println("ok-line");
return 0;



//? test: unicode-write-read-chinese
//? description: Chinese UTF-8 round-trip via write/read
//? expect: 0
//? source: ...

const text = "你好世界";
let sn = open_file("uc1", "data/zh.txt", "w");
write(sn, text);
close(sn);
sn = open_file("uc1", "data/zh.txt", "r");
/* 你好世界 is 12 octets in UTF-8 */
assert(read(sn, 100) === text);
close(sn);
return 0;


//? test: unicode-write-read-japanese
//? description: Japanese UTF-8 round-trip
//? expect: 0
//? source: ...

const text = "こんにちは";
let sn = open_file("uc2", "data/ja.txt", "w");
write(sn, text);
close(sn);
sn = open_file("uc2", "data/ja.txt", "r");
assert(read(sn, 100) === text);
close(sn);
return 0;


//? test: unicode-write-read-arabic
//? description: Arabic UTF-8 round-trip (RTL script)
//? expect: 0
//? source: ...

const text = "مرحبا بالعالم";
let sn = open_file("uc3", "data/ar.txt", "w");
write(sn, text);
close(sn);
sn = open_file("uc3", "data/ar.txt", "r");
assert(read(sn, 200) === text);
close(sn);
return 0;


//? test: unicode-write-read-european
//? description: European accented characters round-trip
//? expect: 0
//? source: ...

const text = "café naïve Zürich — España";
let sn = open_file("uc4", "data/eu.txt", "w");
write(sn, text);
close(sn);
sn = open_file("uc4", "data/eu.txt", "r");
assert(read(sn, 200) === text);
close(sn);
return 0;


//? test: unicode-write-read-emoji
//? description: Emoji (4-byte UTF-8) round-trip
//? expect: 0
//? source: ...

const text = "Hello 🌍 🎉 👨‍👩‍👧‍👦";
let sn = open_file("uc5", "data/emoji.txt", "w");
write(sn, text);
close(sn);
sn = open_file("uc5", "data/emoji.txt", "r");
assert(read(sn, 200) === text);
close(sn);
return 0;


//? test: unicode-mixed-multilingual
//? description: Mixed multilingual paragraph round-trip
//? expect: 0
//? source: ...

const text =
    "EN: Hello. " +
    "ZH: 你好. " +
    "JA: こんにちは. " +
    "AR: مرحبا. " +
    "RU: Привет. " +
    "EL: Γειά σου. " +
    "HI: नमस्ते. " +
    "KO: 안녕하세요.";
let sn = open_file("uc6", "data/mixed.txt", "w");
write(sn, text);
flush(sn);
close(sn);
sn = open_file("uc6", "data/mixed.txt", "r");
assert(read(sn, 1000) === text);
close(sn);
return 0;


//? test: unicode-readln-lines
//? description: readln preserves Unicode lines without newline
//? expect: 0
//? source: ...

let sn = open_file("uc7", "data/ulines.txt", "w");
writeln(sn, "第一行");
writeln(sn, "segunda línea");
writeln(sn, "سطر ثالث");
close(sn);

sn = open_file("uc7", "data/ulines.txt", "r");
assert(readln(sn) === "第一行");
assert(readln(sn) === "segunda línea");
assert(readln(sn) === "سطر ثالث");
assert(readln(sn) === "");
close(sn);
return 0;


//? test: unicode-write-multiple-args
//? description: write multiple Unicode string args concatenates
//? expect: 0
//? source: ...

let sn = open_file("uc8", "data/umulti.txt", "w");
write(sn, "αβγ", " ", "δεζ");
close(sn);
sn = open_file("uc8", "data/umulti.txt", "r");
assert(read(sn, 50) === "αβγ δεζ");
close(sn);
return 0;


//? test: unicode-write_internal-string
//? description: write_internal preserves Unicode string octets
//? expect: 0
//? source: ...

const text = "東京・パリ・São Paulo";
let sn = open_file("uc9", "data/uwi.txt", "w");
write_internal(sn, text);
close(sn);
sn = open_file("uc9", "data/uwi.txt", "r");
assert(read(sn, 200) === text);
close(sn);
return 0;


//? test: unicode-partial-codepoint-rejected
//? description: read that splits multi-byte UTF-8 codepoint throws
//? expect: error
//? source: ...

/* 你 is U+4F60 = E4 BD A0 (3 octets). Reading 1 octet is invalid UTF-8. */
let sn = open_file("uc10", "data/usplit.txt", "w");
write(sn, "你好");
close(sn);
sn = open_file("uc10", "data/usplit.txt", "r");
read(sn, 1);


//? test: unicode-octet-count-read
//? description: read n is octets not characters; exact multi-byte length
//? expect: 0
//? source: ...

/* 你好 = 6 octets total (3+3). Read 3 octets => first character only. */
let sn = open_file("uc11", "data/uoct.txt", "w");
write(sn, "你好");
close(sn);
sn = open_file("uc11", "data/uoct.txt", "r");
assert(read(sn, 3) === "你");
assert(read(sn, 3) === "好");
assert(read(sn, 3) === "");
close(sn);
return 0;


//? test: unicode-binary-preserves-utf8-octets
//? description: UTF-8 text via write_internal hex and read_to_hexBinary
//? expect: 0
//? source: ...

/* UTF-8 for "€" is E2 82 AC */
const euro = hexBinary("e282ac");
let sn = open_file("uc12", "data/euro.bin", "wb");
write_internal(sn, euro);
close(sn);
sn = open_file("uc12", "data/euro.bin", "rb");
const got = read_to_hexBinary(sn, 10);
close(sn);
assert(eq<hexBinary>(got, hexBinary("e282ac")));
/* Also as text stream */
sn = open_file("uc12b", "data/euro.txt", "w");
write(sn, "€");
close(sn);
sn = open_file("uc12b", "data/euro.txt", "r");
assert(read(sn, 10) === "€");
close(sn);
return 0;


//? test: unicode-append-mode
//? description: append mode with Unicode
//? expect: 0
//? source: ...

let sn = open_file("uc13", "data/uappend.txt", "w");
write(sn, "α");
close(sn);
sn = open_file("uc13", "data/uappend.txt", "a");
write(sn, "β");
close(sn);
sn = open_file("uc13", "data/uappend.txt", "r");
assert(read(sn, 20) === "αβ");
close(sn);
return 0;


//? test: mode-r-read-existing
//? description: mode r opens existing file for read
//? expect: 0
//? source: ...

let sn = open_file("mr0", "data/mode_r.txt", "w");
write(sn, "readable");
close(sn);
sn = open_file("mr0", "data/mode_r.txt", "r");
assert(read(sn, 20) === "readable");
close(sn);
return 0;


//? test: mode-r-missing-file
//? description: mode r fails when file does not exist
//? expect: error
//? source: ...

open_file("mr1", "data/mode_r_missing.txt", "r");


//? test: mode-r-write-rejected
//? description: mode r is not open for write
//? expect: error:Stream is not open for write
//? source: ...

let sn = open_file("mr2", "data/mode_r_nowrite.txt", "w");
write(sn, "x");
close(sn);
sn = open_file("mr2", "data/mode_r_nowrite.txt", "r");
write(sn, "nope");


//? test: mode-w-creates-and-truncates
//? description: mode w creates file and truncates existing content
//? expect: 0
//? source: ...

let sn = open_file("mw0", "data/mode_w.txt", "w");
write(sn, "first");
close(sn);
sn = open_file("mw0", "data/mode_w.txt", "w");
write(sn, "second");
close(sn);
sn = open_file("mw0", "data/mode_w.txt", "r");
assert(read(sn, 20) === "second");
close(sn);
return 0;


//? test: mode-w-read-rejected
//? description: mode w is not open for read
//? expect: error:Stream is not open for read
//? source: ...

const sn = open_file("mw1", "data/mode_w_noread.txt", "w");
read(sn, 10);


//? test: mode-a-append
//? description: mode a appends without truncating
//? expect: 0
//? source: ...

let sn = open_file("ma0", "data/mode_a.txt", "w");
write(sn, "A");
close(sn);
sn = open_file("ma0", "data/mode_a.txt", "a");
write(sn, "B");
close(sn);
sn = open_file("ma0", "data/mode_a.txt", "r");
assert(read(sn, 10) === "AB");
close(sn);
return 0;


//? test: mode-a-creates-if-missing
//? description: mode a creates file when missing
//? expect: 0
//? source: ...

/* Unique path so re-runs are not polluted by prior appends. */
const path = "data/mode_a_create_" + generate_uuid() + ".txt";
let sn = open_file("ma1", path, "a");
write(sn, "new");
close(sn);
sn = open_file("ma1", path, "r");
assert(read(sn, 10) === "new");
close(sn);
return 0;


//? test: mode-a-read-rejected
//? description: mode a is not open for read
//? expect: error:Stream is not open for read
//? source: ...

const sn = open_file("ma2", "data/mode_a_noread.txt", "a");
read(sn, 10);


//? test: mode-rplus-read-and-write
//? description: mode r+ reads and writes existing file
//? expect: 0
//? source: ...

let sn = open_file("mrp0", "data/mode_rplus.txt", "w");
write(sn, "abcd");
close(sn);
sn = open_file("mrp0", "data/mode_rplus.txt", "r+");
assert(read(sn, 2) === "ab");
write(sn, "XY");
close(sn);
sn = open_file("mrp0", "data/mode_rplus.txt", "r");
assert(read(sn, 10) === "abXY");
close(sn);
return 0;


//? test: mode-rplus-missing-file
//? description: mode r+ fails when file does not exist
//? expect: error
//? source: ...

open_file("mrp1", "data/mode_rplus_missing.txt", "r+");


//? test: mode-wplus-create-read-write
//? description: mode w+ creates, truncates, allows read and write
//? expect: 0
//? source: ...

let sn = open_file("mwp0", "data/mode_wplus.txt", "w");
write(sn, "old");
close(sn);
sn = open_file("mwp0", "data/mode_wplus.txt", "w+");
write(sn, "new");
flush(sn);
/* reopen to read from start; same handle position is after write */
close(sn);
sn = open_file("mwp0", "data/mode_wplus.txt", "r");
assert(read(sn, 10) === "new");
close(sn);
/* create path with w+ and read after reopen */
sn = open_file("mwp1", "data/mode_wplus_new.txt", "w+");
write(sn, "created");
close(sn);
sn = open_file("mwp1", "data/mode_wplus_new.txt", "r+");
assert(read(sn, 20) === "created");
close(sn);
return 0;


//? test: mode-aplus-read-start-write-append
//? description: mode a+ reads from start; writes append
//? expect: 0
//? source: ...

let sn = open_file("map0", "data/mode_aplus.txt", "w");
write(sn, "base");
close(sn);
sn = open_file("map0", "data/mode_aplus.txt", "a+");
assert(read(sn, 10) === "base");
write(sn, "tail");
close(sn);
sn = open_file("map0", "data/mode_aplus.txt", "r");
assert(read(sn, 20) === "basetail");
close(sn);
return 0;


//? test: mode-rb-wb-roundtrip
//? description: binary modes wb and rb
//? expect: 0
//? source: ...

let sn = open_file("mb0", "data/mode_rb_wb.bin", "wb");
write_internal(sn, hexBinary("deadbeef"));
close(sn);
sn = open_file("mb0", "data/mode_rb_wb.bin", "rb");
const got0 = read_to_hexBinary(sn, 100);
close(sn);
assert(eq<hexBinary>(got0, hexBinary("deadbeef")));
return 0;


//? test: mode-ab-append-binary
//? description: binary mode ab appends octets
//? expect: 0
//? source: ...

let sn = open_file("mb1", "data/mode_ab.bin", "wb");
write_internal(sn, hexBinary("01"));
close(sn);
sn = open_file("mb1", "data/mode_ab.bin", "ab");
write_internal(sn, hexBinary("02"));
close(sn);
sn = open_file("mb1", "data/mode_ab.bin", "rb");
const got1 = read_to_hexBinary(sn, 100);
close(sn);
assert(eq<hexBinary>(got1, hexBinary("0102")));
return 0;


//? test: mode-rbplus
//? description: binary mode rb+ read and write existing
//? expect: 0
//? source: ...

let sn = open_file("mb2", "data/mode_rbplus.bin", "wb");
write_internal(sn, hexBinary("aabbccdd"));
close(sn);
sn = open_file("mb2", "data/mode_rbplus.bin", "rb+");
const head2 = read_to_hexBinary(sn, 2);
assert(eq<hexBinary>(head2, hexBinary("aabb")));
write_internal(sn, hexBinary("eeff"));
close(sn);
sn = open_file("mb2", "data/mode_rbplus.bin", "rb");
const all2 = read_to_hexBinary(sn, 100);
close(sn);
assert(eq<hexBinary>(all2, hexBinary("aabbeeff")));
return 0;


//? test: mode-r-plus-b
//? description: binary mode r+b same as rb+
//? expect: 0
//? source: ...

let sn = open_file("mb3", "data/mode_rplusb.bin", "wb");
write_internal(sn, hexBinary("11223344"));
close(sn);
sn = open_file("mb3", "data/mode_rplusb.bin", "r+b");
const head3 = read_to_hexBinary(sn, 2);
assert(eq<hexBinary>(head3, hexBinary("1122")));
write_internal(sn, hexBinary("5566"));
close(sn);
sn = open_file("mb3", "data/mode_rplusb.bin", "rb");
const all3 = read_to_hexBinary(sn, 100);
close(sn);
assert(eq<hexBinary>(all3, hexBinary("11225566")));
return 0;


//? test: mode-wbplus
//? description: binary mode wb+ create/truncate read-write
//? expect: 0
//? source: ...

let sn = open_file("mb4", "data/mode_wbplus.bin", "wb");
write_internal(sn, hexBinary("ffff"));
close(sn);
sn = open_file("mb4", "data/mode_wbplus.bin", "wb+");
write_internal(sn, hexBinary("0102"));
close(sn);
sn = open_file("mb4", "data/mode_wbplus.bin", "rb");
const got4 = read_to_hexBinary(sn, 100);
close(sn);
assert(eq<hexBinary>(got4, hexBinary("0102")));
return 0;


//? test: mode-w-plus-b
//? description: binary mode w+b same as wb+
//? expect: 0
//? source: ...

let sn = open_file("mb5", "data/mode_wplusb.bin", "w+b");
write_internal(sn, hexBinary("99"));
close(sn);
sn = open_file("mb5", "data/mode_wplusb.bin", "rb");
const got5 = read_to_hexBinary(sn, 100);
close(sn);
assert(eq<hexBinary>(got5, hexBinary("99")));
return 0;


//? test: mode-abplus
//? description: binary mode ab+ append write with read
//? expect: 0
//? source: ...

let sn = open_file("mb6", "data/mode_abplus.bin", "wb");
write_internal(sn, hexBinary("aa"));
close(sn);
sn = open_file("mb6", "data/mode_abplus.bin", "ab+");
const head6 = read_to_hexBinary(sn, 100);
assert(eq<hexBinary>(head6, hexBinary("aa")));
write_internal(sn, hexBinary("bb"));
close(sn);
sn = open_file("mb6", "data/mode_abplus.bin", "rb");
const all6 = read_to_hexBinary(sn, 100);
close(sn);
assert(eq<hexBinary>(all6, hexBinary("aabb")));
return 0;


//? test: mode-a-plus-b
//? description: binary mode a+b same as ab+
//? expect: 0
//? source: ...

let sn = open_file("mb7", "data/mode_aplusb.bin", "wb");
write_internal(sn, hexBinary("cc"));
close(sn);
sn = open_file("mb7", "data/mode_aplusb.bin", "a+b");
const head7 = read_to_hexBinary(sn, 100);
assert(eq<hexBinary>(head7, hexBinary("cc")));
write_internal(sn, hexBinary("dd"));
close(sn);
sn = open_file("mb7", "data/mode_aplusb.bin", "rb");
const all7 = read_to_hexBinary(sn, 100);
close(sn);
assert(eq<hexBinary>(all7, hexBinary("ccdd")));
return 0;


//? test: mode-rb-write-rejected
//? description: mode rb is not open for write
//? expect: error:Stream is not open for write
//? source: ...

let sn = open_file("mb8", "data/mode_rb_nowrite.bin", "wb");
write_internal(sn, hexBinary("00"));
close(sn);
sn = open_file("mb8", "data/mode_rb_nowrite.bin", "rb");
write_internal(sn, hexBinary("01"));


//? test: mode-wb-read-rejected
//? description: mode wb is not open for read
//? expect: error:Stream is not open for read
//? source: ...

const sn = open_file("mb9", "data/mode_wb_noread.bin", "wb");
read_to_hexBinary(sn, 10);


//? test: robust-invalid-mode
//? description: unknown fopen mode fails to open
//? expect: error
//? source: ...

open_file("badmode", "data/badmode.txt", "qq");


//? test: robust-empty-mode
//? description: empty mode string fails
//? expect: error
//? source: ...

open_file("emptymode", "data/emptymode.txt", "");


//? test: robust-empty-path
//? description: empty logical path fails resolve
//? expect: error
//? source: ...

open_file("emptypath", "", "r");


//? test: robust-empty-streamId
//? description: empty streamId can open once and is look-upable
//? expect: 0
//? source: ...

/* Empty streamId is allowed; still one slot, still closeable. */
const sn = open_file("", "data/empty_sid.txt", "w");
assert(sn >= 0);
assert(stream("") === sn);
write(sn, "ok");
close(sn);
return 0;


//? test: robust-streamId-stdout-conflict
//? description: cannot open_file with streamId of standard stdout
//? expect: error
//? source: ...

/* stdout is always a standard streamId */
open_file("stdout", "data/hijack_stdout.txt", "w");


//? test: robust-streamId-stderr-conflict
//? description: cannot open_file with streamId stderr
//? expect: error
//? source: ...

open_file("stderr", "data/hijack_stderr.txt", "w");


//? test: robust-use-after-close-write
//? description: write after close throws invalid streamNumber
//? expect: error
//? source: ...

const sn = open_file("uacw", "data/uacw.txt", "w");
close(sn);
write(sn, "after-close");


//? test: robust-use-after-close-read
//? description: read after close throws invalid streamNumber
//? expect: error
//? source: ...

let sn = open_file("uacr", "data/uacr.txt", "w");
write(sn, "x");
close(sn);
read(sn, 10);


//? test: robust-double-close
//? description: second close of same streamNumber throws
//? expect: error
//? source: ...

const sn = open_file("dblc", "data/dblc.txt", "w");
close(sn);
close(sn);


//? test: robust-flush-after-close
//? description: flush after close throws
//? expect: error
//? source: ...

const sn = open_file("flc", "data/flc.txt", "w");
close(sn);
flush(sn);


//? test: robust-read-negative-n
//? description: read with negative n throws
//? expect: error
//? source: ...

let sn = open_file("negn", "data/negn.txt", "w");
write(sn, "x");
close(sn);
sn = open_file("negn", "data/negn.txt", "r");
read(sn, -1);


//? test: robust-read_to_hexBinary-negative-n
//? description: read_to_hexBinary with negative n throws
//? expect: error
//? source: ...

let sn = open_file("negh", "data/negh.bin", "wb");
write_internal(sn, hexBinary("00"));
close(sn);
sn = open_file("negh", "data/negh.bin", "rb");
read_to_hexBinary(sn, -5);


//? test: robust-read-n-at-max
//? description: read n at the 1,000,000 octet cap still returns available bytes
//? expect: 0
//? source: ...

let sn = open_file("maxn", "data/maxn.txt", "w");
write(sn, "ok");
close(sn);
sn = open_file("maxn", "data/maxn.txt", "r");
assert(read(sn, 1000000) === "ok");
close(sn);
return 0;


//? test: robust-read-n-over-max
//? description: read n above 1,000,000 throws
//? expect: error
//? source: ...

let sn = open_file("overn", "data/overn.txt", "w");
write(sn, "x");
close(sn);
sn = open_file("overn", "data/overn.txt", "r");
read(sn, 1000001);


//? test: robust-read_to_hexBinary-n-over-max
//? description: read_to_hexBinary n above 1,000,000 throws
//? expect: error
//? source: ...

let sn = open_file("overh", "data/overh.bin", "wb");
write_internal(sn, hexBinary("00"));
close(sn);
sn = open_file("overh", "data/overh.bin", "rb");
read_to_hexBinary(sn, 1000001);


//? test: robust-read_to_base64Binary-n-over-max
//? description: read_to_base64Binary n above 1,000,000 throws
//? expect: error
//? source: ...

let sn = open_file("overb", "data/overb.bin", "wb");
write_internal(sn, hexBinary("00"));
close(sn);
sn = open_file("overb", "data/overb.bin", "rb");
read_to_base64Binary(sn, 1000001);


//? test: robust-multiple-streams-same-file
//? description: two streamIds can open same path (read) independently
//? expect: 0
//? source: ...

let sn = open_file("ms1", "data/multi_open.txt", "w");
write(sn, "shared");
close(sn);
const a = open_file("msA", "data/multi_open.txt", "r");
const b = open_file("msB", "data/multi_open.txt", "r");
assert(a !== b);
assert(read(a, 10) === "shared");
assert(read(b, 10) === "shared");
close(a);
close(b);
return 0;


//? test: robust-many-open-close-cycle
//? description: open/close many times reuses slots without leak of ids
//? expect: 0
//? source: ...

let i = 0;
while (i < 25) {
    const sn = open_file("cycle", "data/cycle.txt", "w");
    write(sn, "x");
    close(sn);
    i = i + 1;
}
/* After last close, streamId must not resolve (throws). */
return 0;


//? test: robust-writeln-no-args
//? description: writeln with only streamNumber writes a newline
//? expect: 0
//? source: ...

let sn = open_file("wln0", "data/writeln0.txt", "w");
writeln(sn);
close(sn);
sn = open_file("wln0", "data/writeln0.txt", "r");
assert(read(sn, 10) === "\n");
close(sn);
return 0;


//? test: robust-write-undefined-arg
//? description: write of undefined renders as the word undefined
//? expect: 0
//? source: ...

let sn = open_file("wund", "data/write_undef.txt", "w");
write(sn, undefined);
close(sn);
sn = open_file("wund", "data/write_undef.txt", "r");
assert(read(sn, 20) === "undefined");
close(sn);
return 0;


//? test: robust-readln-empty-lines
//? description: readln handles consecutive newlines
//? expect: 0
//? source: ...

let sn = open_file("eln", "data/empty_lines.txt", "w");
write(sn, "a\n\nb\n");
close(sn);
sn = open_file("eln", "data/empty_lines.txt", "r");
assert(readln(sn) === "a");
assert(readln(sn) === "");
assert(readln(sn) === "b");
assert(readln(sn) === "");
close(sn);
return 0;


//? test: robust-readln-no-final-newline
//? description: last line without newline is still returned
//? expect: 0
//? source: ...

let sn = open_file("nfn", "data/no_final_nl.txt", "w");
write(sn, "no-nl");
close(sn);
sn = open_file("nfn", "data/no_final_nl.txt", "r");
assert(readln(sn) === "no-nl");
assert(readln(sn) === "");
close(sn);
return 0;


//? test: robust-path-exact-root-key
//? description: logical path equal to root key is host directory; fopen as file fails
//? expect: error
//? source: ...

/* Opening the directory itself as a file should fail at fopen. */
open_file("rootkey", "data", "w");


//? test: robust-path-double-slash
//? description: double slash under prefix still resolves under root
//? expect: 0
//? source: ...

let sn = open_file("dsl", "data//double_slash.txt", "w");
write(sn, "ds");
close(sn);
sn = open_file("dsl", "data/double_slash.txt", "r");
assert(read(sn, 10) === "ds");
close(sn);
return 0;


//? test: robust-write_internal-base64Binary
//? description: write_internal and read_to_base64Binary with base64Binary value
//? expect: 0
//? source: ...

const payload = base64Binary("SGVsbG8="); /* Hello */
let sn = open_file("b64wi", "data/b64wi.bin", "wb");
write_internal(sn, payload);
close(sn);
sn = open_file("b64wi", "data/b64wi.bin", "rb");
const got = read_to_base64Binary(sn, 100);
close(sn);
assert(eq<base64Binary>(got, payload));
return 0;


//? test: robust-read-oversized-n
//? description: read n larger than file returns only available octets
//? expect: 0
//? source: ...

let sn = open_file("bigN", "data/bigN.txt", "w");
write(sn, "short");
close(sn);
sn = open_file("bigN", "data/bigN.txt", "r");
assert(read(sn, 1000000) === "short");
close(sn);
return 0;


//? test: robust-binary-then-text-utf8
//? description: binary write of UTF-8 octets readable as text
//? expect: 0
//? source: ...

/* 你好 = e4bda0e5a5bd */
let sn = open_file("btu", "data/bin_text_utf8.bin", "wb");
write_internal(sn, hexBinary("e4bda0e5a5bd"));
close(sn);
sn = open_file("btu", "data/bin_text_utf8.bin", "r");
assert(read(sn, 100) === "你好");
close(sn);
return 0;


//? test: robust-two-files-interleaved
//? description: interleaved write to two open streams
//? expect: 0
//? source: ...

const a = open_file("ia", "data/inter_a.txt", "w");
const b = open_file("ib", "data/inter_b.txt", "w");
write(a, "A1");
write(b, "B1");
write(a, "A2");
write(b, "B2");
close(a);
close(b);
const ra = open_file("ia", "data/inter_a.txt", "r");
const rb = open_file("ib", "data/inter_b.txt", "r");
assert(read(ra, 20) === "A1A2");
assert(read(rb, 20) === "B1B2");
close(ra);
close(rb);
return 0;


//? test: robust-flush-idempotent
//? description: multiple flush calls succeed
//? expect: 0
//? source: ...

const sn = open_file("fli", "data/flush_idem.txt", "w");
write(sn, "x");
flush(sn);
flush(sn);
close(sn);
return 0;


//? test: robust-read_to_hexBinary-zero
//? description: read_to_hexBinary n=0 returns empty hexBinary
//? expect: 0
//? source: ...

let sn = open_file("hz0", "data/hex_zero.bin", "wb");
write_internal(sn, hexBinary("ff"));
close(sn);
sn = open_file("hz0", "data/hex_zero.bin", "rb");
const got = read_to_hexBinary(sn, 0);
close(sn);
assert(eq<hexBinary>(got, hexBinary("")));
return 0;


//? test: robust-absolute-host-escape-via-logical
//? description: double-slash remainder stays under rootFilePaths host root
//? expect: 0
//? source: ...

/*
 * Remainder after prefix may contain //; path must stay under the mapped
 * host directory (not rebind to host filesystem root).
 */
let sn = open_file("esc", "data//under_root_safe.txt", "w");
write(sn, "safe");
close(sn);
sn = open_file("esc", "data/under_root_safe.txt", "r");
assert(read(sn, 20) === "safe");
close(sn);
return 0;



//? test: robust-unicode-filename-roundtrip
//? description: open_file path with Unicode filename component
//? expect: 0
//? source: ...

const path = "data/файл-名前.txt";
let sn = open_file("ufn", path, "w");
write(sn, "имя");
close(sn);
sn = open_file("ufn", path, "r");
assert(read(sn, 50) === "имя");
close(sn);
return 0;


//? test: robust-rplus-overwrite-middle
//? description: r+ can overwrite middle of file after partial read
//? expect: 0
//? source: ...

let sn = open_file("rpm", "data/rplus_mid.txt", "w");
write(sn, "0123456789");
close(sn);
sn = open_file("rpm", "data/rplus_mid.txt", "r+");
assert(read(sn, 4) === "0123");
write(sn, "ABCD");
close(sn);
sn = open_file("rpm", "data/rplus_mid.txt", "r");
assert(read(sn, 20) === "0123ABCD89");
close(sn);
return 0;


//? test: error-open-file-streamId-already-open
//? description: open_file throws when streamId is already open
//? expect: error:streamId 'dup_sid' is already open
//? source: ...

open_file("dup_sid", "data/dup1.txt", "w");
return open_file("dup_sid", "data/dup2.txt", "w");


//? test: error-write-internal-wrong-type
//? description: write_internal throws for a value that is not string, hexBinary, or base64Binary
//? expect: error:write_internal() requires string, hexBinary, or base64Binary
//? source: ...

let sn = open_file("wi_probe", "data/wi_probe.txt", "w");
return write_internal(sn, integer(5));


//? test: error-read-invalid-utf8
//? description: read throws when the raw bytes read are not valid UTF-8
//? expect: error:read() result is not valid UTF-8
//? source: ...

let sn = open_file("bad_utf8", "data/bad_utf8.bin", "w");
write_internal(sn, hexBinary("FF"));
close(sn);
sn = open_file("bad_utf8", "data/bad_utf8.bin", "r");
return read(sn, 10);


//? test: error-readln-invalid-utf8
//? description: readln throws when the raw bytes read are not valid UTF-8
//? expect: error:readln() result is not valid UTF-8
//? source: ...

let sn = open_file("bad_utf8_ln", "data/bad_utf8_ln.bin", "w");
write_internal(sn, hexBinary("FF0A"));
close(sn);
sn = open_file("bad_utf8_ln", "data/bad_utf8_ln.bin", "r");
return readln(sn);
