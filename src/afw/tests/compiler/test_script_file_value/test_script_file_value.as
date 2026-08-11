#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: test_script_file_value.as
//? customPurpose: Part of compiler category tests
//? description: Exercise //? key file form (triple less-than path) for various keys
//? sourceType: script
//?
//? test: expect-from-file-integer
//? description: File form supplies expect Adaptive source for integer 0
//? expect: <<< expect_zero.txt
//? source: ...

return 0;

//?
//? test: expect-from-file-string
//? description: File form supplies quoted string expect
//? expect: <<< expect_string.txt
//? source: ...

return "from-file";

//?
//? test: expect-from-file-bag
//? description: File form multi-line Adaptive expect (bag)
//? expect: <<< expect_bag.txt
//? source: ...

return bag<integer>(1, 2, 3);

//?
//? test: source-from-file
//? description: File form supplies entire source body
//? expect: 7
//? source: <<< source_return_7.txt
//?
//? test: description-from-file
//? description: <<< description.txt
//? expect: 1
//? source: ...

return 1;

//?
//? test: mixed-inline-and-file
//? description: inline expect with file source
//? expect: "mixed"
//? source: <<< source_return_mixed.txt
