#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: test_script_expect_streams.as
//? customPurpose: Part of compiler category tests
//? description: expect-stdout / expect-stderr side-channel capture
//? sourceType: script
//?
//? test: print-expect-stdout
//? description: print text matches expect-stdout exactly
//? expect: 0
//? expect-stdout: ok
//? source: ...

print("ok");
return 0;


//?
//? test: println-expect-stdout
//? description: println includes trailing newline
//? expect: 0
//? expect-stdout: ...
ok-line

//? source: ...

println("ok-line");
return 0;


//?
//? test: expect-stdout-empty
//? description: no print when expect-stdout is empty string
//? expect: 0
//? expect-stdout: ...
//? source: ...

return 0;


//?
//? test: write-stdout-stream
//? description: write to stream("stdout") is captured
//? expect: 0
//? expect-stdout: via-stream
//? source: ...

write(stream("stdout"), "via-stream");
return 0;


//?
//? test: expect-stderr-write
//? description: write to stream("stderr") matches expect-stderr
//? expect: 0
//? expect-stderr: err-text
//? source: ...

write(stream("stderr"), "err-text");
return 0;


//?
//? test: both-streams
//? description: stdout and stderr expects together
//? expect: 0
//? expect-stdout: out
//? expect-stderr: err
//? source: ...

print("out");
write(stream("stderr"), "err");
return 0;


//?
//? test: return-and-stdout
//? description: return value and stdout both checked
//? expect: "done"
//? expect-stdout: hello
//? source: ...

print("hello");
return "done";
