#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: if.as
//? customPurpose: Part of test262
//? description: Tests if statement
//? sourceType: script
//?
//? test: if-1
//? description: testing falsy values
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if (false)
    throw "if false should be false";

return 0;


//? test: if-2
//? description: test if/else
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let c: integer = 1;
if (false)
    throw "false in expression should be false";
else
    c = c + 1;

assert(c === 2, "false else branch didn't execute");

return 0;

//? test: if-3
//? description: Using embedded "if/else" into "if/else" constructions
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if (true)
    if (false)
        throw "if false in if should be false";
    else
        ;
else
    if (true)
        throw "if true in else should be false";
    else
        throw "else in else should be false";

if (true)
    if (true)
        ;
    else
        throw "else in if should be false";
else
    if (true)
        throw "if true in else should be false";
    else
        throw "else in else should be false";

if (false)
    if (true)
        throw "if true in if should be false";
    else
        throw "else in if should be false";
else
    if (true)
        ;
    else
        throw "else in else should be false";

if (false)
    if (true)
        throw "if true in if should be false";
    else
        throw "else in if should be false";
else
    if (false)
        throw "if false in else should be false";
    else
        ;

return 0;


//? test: if-4
//? description: Using embedded "if/else" into "if/else" block constructions
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if(true){
  if (false)
    throw "if false in if should be false";
}
else{ 
  if (true)
    throw "if true in else should be false";
}

if(true){
  if (true)
    ;
}
else{ 
  if (true)
    throw "else in if should be false";
}

if(false){
  if (true)
    throw "if true in if should be false";
}
else{ 
  if (true)
    ;
}

if(false){
  if (true)
    throw "if true in if should be false";
}
else{ 
  if (false)
    throw "else in else should be false";
}

return 0;


//? test: if-5
//? description: Using embedded "if/else" into "if" constructions
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if(true)
  if (false)
    throw "if false in if should be false";
  else
    ;

if(true)
  if (true)
    ;
  else
    throw "else in if should be false";

if(false)
  if (true)
    throw "if true in if should be false";
  else
    throw "else in if should be false";

if(false)
  if (true)
    throw "if true in if should be false";
  else
    throw "else in if should be false";

return 0;

//? test: if-6
//? description: Using embedded "if" into "if" constructions
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if(true)
  if (false)
    throw "if false in if should be false";

let c: integer=0;
if(true)
  if (true)
    c=2;
if (c!==2)
    throw "if true in if should be true";

if(false)
  if (true)
    throw "if true in if should be false";

if(false)
  if (true)
    throw "if true in if should be false";

return 0;


//? test: if-7
//? description: Using "if" without "else" construction
//? expect: 0
//? source: ...
#!/usr/bin/env afw


if (!(true))
    throw "if !(true) should be false";

return 0;


//? test: if-8
//? description: Using "if/else" construction
//? expect: 0
//? source: ...
#!/usr/bin/env afw

let c: integer = 1;
if(!(true))
	throw "if !(true) should be false";
else
  c = c + 1;
if (c!=2) throw "if !(true) else branch didn't execute";

return 0;

//? test: if-9
//? description: Checking if execution of "if()" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw

if ();

//? test: if-10
//? description: In the "if" statement, empty statement is allowed.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if (true);

return 0;

//? test: if-11
//? description: In the "if" statement, the statement can be broken up by newlines
//? expect: 0
//? source: ...
#!/usr/bin/env afw

if (true) let x
: integer;

if (false) let 
y: integer = 1;

return 0;