# Language Tests

These folders contain tests that were derived from the Official ECMAScript 
Conformance Test Suite, where you can find the source to here:  
https://github.com/tc39/test262/tree/main/test/language.

Each test has an id, whose prefix aligns with the chapter/section located in 
the ECMAScript Language Specification (5.1 Edition) found here:  
https://262.ecma-international.org/5.1/. If a folder contains several tests, 
we have changed that into a individual test file (.as) with multiple tests, 
in order to minimize the number of test files.

The functionality of ECMAScript and Adaptive Script have some overlap. This was 
intentional, in order to provide users with some comforts of a modern, 
well-received language. However, ECMAScript has some history and syntax that 
was designed for backwards compatibility, dating back to the beginning of the 
web browsers. Some if its rules for comparison and data type conversions 
reflect this.

The full Conformance Tests rely on constructs such as Objects, generators, 
async operations and prototypes. It also depends on a small number of 
primitive data types. Furthermore, not all of the syntax and semantics are the 
same between the two, and our EBNF and their grammar names do not always match 
up. Because of these differences, we used a subset of the Conformance tests and 
changed some of them, along with their descriptions, to meet our needs.

The script in this directory, _convert.py, was a helper script to attempt to 
automate some of this conversion.  There was still a lot of adjustments to 
be made after it was run.
