#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: keywords.as
//? customPurpose: Part of test262
//? description: keywords
//? sourceType: script
//?
//? test: ident-ref-break
//? description: Checking if execution of "break=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc break = 1;


//? test: ident-ref-case
//? description: Checking if execution of "case=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc case = 1;


//? test: ident-ref-catch
//? description: Checking if execution of "catch=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc catch = 1;


//? test: ident-ref-continue
//? description: Checking if execution of "contunue=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc continue = 1;


//? test: ident-ref-default
//? description: Checking if execution of "default=1" fails
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

loc default = 1;



//? test: ident-ref-do
//? description: Checking if execution of "do=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc do = 1;


//? test: ident-ref-else
//? description: Checking if execution of "else=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc else = 1;


//? test: ident-ref-finally
//? description: Checking if execution of "finally=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc finally = 1;


//? test: ident-ref-for
//? description: Checking if execution of "for=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc for = 1;


//? test: ident-ref-function
//? description: Checking if execution of "function=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc function = 1;


//? test: ident-ref-if
//? description: Checking if execution of "if=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc if = 1;


//? test: ident-ref-in
//? description: Checking if execution of "in=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc in = 1;


//? test: ident-ref-instanceof
//? description: Checking if execution of "instanceof=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc instanceof = 1;



//? test: ident-ref-return
//? description: Checking if execution of "return=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc return = 1;


//? test: ident-ref-switch
//? description: Checking if execution of "switch=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc switch = 1;


//? test: ident-ref-throw
//? description: Checking if execution of "throw=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc throw = 1;


//? test: ident-ref-try
//? description: Checking if execution of "try=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc try = 1;


//? test: ident-ref-typeof
//? description: Checking if execution of "typeof=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc typeof = 1;


//? test: ident-ref-var
//? description: Checking if execution of "var=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc loc = 1;


//? test: ident-ref-while
//? description: Checking if execution of "while=1" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc while = 1;
