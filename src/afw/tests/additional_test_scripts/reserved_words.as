#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: reserved_words.as
//? customPurpose: Part of custom tests
//? description: Check to make sure reserved words are enforced correctly.
//? sourceType: script
//?
//? test: reserved_word_1
//? description: Reserved word after "loc" is an error.
//? expect: error:Parse error at offset 24 around line 3 column 5: Variable name can not be a reserved word
//? source: ...
#!/usr/bin/env afw

loc if: integer = 1;
return if;

//? test: reserved_word_2
//? description: Reserved word as property name is okay
//? expect: 1
//? source: ...
#!/usr/bin/env afw

loc obj: object = {};
obj.if = 1;

return obj.if;

//? test: reserved_word_3
//? description: Reserved word as property name is okay, even special literals
//? expect: 1
//? source: ...
#!/usr/bin/env afw

loc obj: object = {};
obj.NaN = 1;

return obj.NaN;

//? test: reserved_word_4
//? description: Calling function as statement with reserved name requires "fn::" qualifier
//? expect: error:Parse error at offset 27 around line 3 column 8: Expecting ')'
//? source: ...
#!/usr/bin/env afw

if(true, print("true"), print("false"));

return 1;

//? test: reserved_word_5
//? description: Calling function as statement with reserved name requires "fn::" qualifier
//? expect: 1
//? source: ...
#!/usr/bin/env afw

fn::if(true, print("true"), print("false"));

return 1;

//? test: reserved_word_6
//? description: Name function name can not be reserved word
//? expect: error:Parse error at offset 29 around line 3 column 10: Function name can not be a reserved word
//? source: ...
#!/usr/bin/env afw

function if() print("hello");

if();

return 1;
