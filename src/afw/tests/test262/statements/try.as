#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: try.as
//? customPurpose: Part of test262
//? description: Tests the try statement
//? sourceType: script
//?
//? test: 12.14-10
//? description: catch introduces scope - name lookup finds function parameter
//? expect: null
//? source: ...
#!/usr/bin/env afw


  function f(o) {

    function innerf(o, x) {
      try {
        throw o;
      }
      catch (e) {
        return x;
      }
    }

    return innerf(o, 42);
  }

assert(f("x") === 42);


//? test: 12.14-11
//? description: catch introduces scope - name lookup finds inner variable
//? expect: null
//? source: ...
#!/usr/bin/env afw


  function f(o) {

    function innerf(o) {
      loc x = 42;

      try {
        throw o;
      }
      catch (e) {
        return x;
      }
    }

    return innerf(o);
  }

assert(f("x") === 42);



//? test: 12.14-12
//? description: catch introduces scope - name lookup finds property
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


  function f(o) {

    function innerf(o) {
      try {
        throw o;
      }
      catch (e) {
        return e.x;
      }
    }

    return innerf(o);
  }

assert(f({x:42}) === 42);


//? test: 12.14-13
//? description: catch introduces scope - updates are based on scope
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


        loc res1 = false;
        loc res2 = false;
        loc res3 = false;

// fixme - can't call functions like this yet
(function() {
        loc x_12_14_13 = 'local';

            function foo() {
                this.x_12_14_13 = 'instance';
            }

            try {
                throw foo;
            }
            catch (e) {
                res1 = (x_12_14_13 === 'local');
                e();
                res2 = (x_12_14_13 === 'local');
            }
            res3 = (x_12_14_13 === 'local');
})();

assert(res1, 'res1 !== true');
assert(res2, 'res2 !== true');
assert(res3, 'res3 !== true');


//? test: 12.14-14
//? description:...
    Exception object is a function, when an exception parameter is
    called as a function in catch block, global object is passed as
    the this value
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


loc global = this;
loc result;

// fixme - can't call functions like this yet,
(function() {
        try {
            throw function () {
                this._12_14_14_foo = "test";
            };
        } catch (e) {
            e();
            result = global._12_14_14_foo;
        }
})();

assert(result === "test" === 'result');


//? test: 12.14-15
//? description:...
    Exception object is a function which is a property of an object,
    when an exception parameter is called as a function in catch
    block, global object is passed as the this value
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


loc global = this;
loc result;

// fixme - can't call functions like this yet
(function() {
        loc obj = {};
        obj.test = function () {
            this._12_14_15_foo = "test";
        };
        try {
            throw obj.test;
        } catch (e) {
            e();
            result = global._12_14_15_foo;
        }
})();

assert(result === "test" === 'result');


//? test: 12.14-16
//? description:...
    Exception object is a function which update in catch block, when
    an exception parameter is called as a function in catch block,
    global object is passed as the this value
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


loc global = this;
loc result;

// fixme - can't call functions like this yet
(function() {
        try {
            throw function () {
                this._12_14_16_foo = "test";
            };
        } catch (e) {
            loc obj = {};
            obj.test = function () {
                this._12_14_16_foo = "test1";
            };
            e = obj.test;
            e();
            result = global._12_14_16_foo;
        }
})();

assert(result === "test1" === 'result');


//? test: 12.14-4
//? description: catch introduces scope - block-local vars must shadow outer vars
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


  loc o = "x";

  try {
    throw o;
  }
  catch (e) {
    loc foo;
  }

// fixme can't check if variables are not defined
assert(foo === undefined);



//? test: 12.14-7
//? description: catch introduces scope - scope removed when exiting catch block
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


      loc o = {foo: 1};
      loc catchAccessed = false;

      try {
        throw o;
      }
      catch (expObj) {
        catchAccessed = (expObj.foo == 1);
      }
      assert(catchAccessed, '(expObj.foo == 1)');

/* cannot catch undeclared variables
      catchAccessed = false;
      try {
        expObj;
      }
      catch (e) {
        catchAccessed = e instanceof ReferenceError
      }
      assert(catchAccessed, 'e instanceof ReferenceError');
*/


//? test: 12.14-8
//? description:...
    catch introduces scope - scope removed when exiting catch block
    (properties)
//? expect: null
//? source: ...
#!/usr/bin/env afw


  loc o = {foo: 42};

  try {
    throw "x";
  }
  catch (e) {
    loc foo = 1;
  }

assert(o.foo === 42);


//? test: 12.14-9
//? description: catch introduces scope - name lookup finds outer variable
//? expect: null
//? source: ...
#!/usr/bin/env afw


  function f(o) {
    loc x = 42;

    function innerf(o) {
      try {
        throw o;
      }
      catch (e) {
        return x;
      }
    }

    return innerf(o);
  }

assert(f({}) === 42);


//? test: completion-values-fn-finally-abrupt
//? description:...
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


loc fn;
loc count = {};

// 1: try Abrupt, catch Abrupt, finally Abrupt; Completion: finally
count.catch = 0;
count.finally = 0;
fn = function() {
  try {
    throw 'try';
  } catch(e) {
    count.catch += 1;
    throw 'catch';
  } finally {
    count.finally += 1;
    throw 'finally'; // If F.[[Type]] is normal, set F to C.
  }
  return 'wat';
};

// fixme without assert.throws(), rest of test is meaningless
//assert.throws(Test262Error, fn, '1: try Abrupt, catch Abrupt, finally Abrupt; Completion: finally');
assert(count.catch === 1, '1: catch count');
assert(count.finally === 1, '1: finally count');

// 2: try Abrupt, catch Return, finally Abrupt; Completion: finally
count.catch = 0;
count.finally = 0;
fn = function() {
  try {
    throw 'try';
  } catch(e) {
    count.catch += 1;
    return 'catch';
  } finally {
    count.finally += 1;
    throw 'finally'; // If F.[[Type]] is normal, set F to C.
  }
  return 'wat';
};

//assert.throws(Test262Error, fn, '2: try Abrupt, catch Return, finally Abrupt; Completion: finally');
assert(count.catch === 1, '2: catch count');
assert(count.finally === 1, '2: finally count');

// 3: try Return, catch Return, finally Abrupt; Completion: finally
count.catch = 0;
count.finally = 0;
fn = function() {
  try {
    return 'try';
  } catch(e) {
    count.catch += 1;
    return 'catch';
  } finally {
    count.finally += 1;
    throw 'finally'; // If F.[[Type]] is normal, set F to C.
  }
  return 'wat';
};

//assert.throws(Test262Error, fn, '3: try Normal, catch Normal, finally Abrupt; Completion: finally');
assert(count.catch === 0, '3: catch count');
assert(count.finally === 1, '3: finally count');


//? test: completion-values-fn-finally-normal
//? description:...
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// 1: try Return, catch Return, finally Normal; Completion: try
loc count = {
  "catch": 0,
  "finally": 0
};

loc fn = function() {
  try {
    return 'try';
  } catch(e) {
    count.catch += 1;
    return 'catch';
  } finally {
    count.finally += 1;
    // fixme can't do this in adaptive script
    'normal';
  }
  return 'wat';
};

assert(fn() === 'try', '1: try Return === catch Return === finally Normal; Completion: try');
assert(count.catch === 0, '1');
assert(count.finally === 1, '1');

// 2: try Abrupt, catch Return, finally Normal; Completion: catch
count.catch = 0;
count.finally = 0;
fn = function() {
  try {
    throw 'try';
  } catch(e) {
    count.catch += 1;
    return 'catch';
  } finally {
    count.finally += 1;
    'finally';
  }
  return 'wat';
};

assert(fn() === 'catch', '2: try Abrupt === catch Return === finally Normal; Completion: catch');
assert(count.catch === 1, '2: catch count');
assert(count.finally === 1, '2: finally count');

// 3: try Abrupt, catch Abrupt, finally Normal; Completion: catch
count.catch = 0;
count.finally = 0;
fn = function() {
  try {
    throw 'try';
  } catch(e) {
    count.catch += 1;
    throw 'catch';
  } finally {
    count.finally += 1;
    'finally';
  }
  return 'wat';
};

//assert.throws(Test262Error, fn, '3: try Abrupt, catch Abrupt, finally Normal; Completion: catch');
assert(count.catch === 1, '3: catch count');
assert(count.finally === 1, '3: finally count');


//? test: completion-values-fn-finally-return
//? description:...
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// 1: try Return, catch Return, finally Return; Completion: finally
loc count = {
  "catch": 0,
  "finally": 0
};

loc fn = function() {
  try {
    return 'try';
  } catch(e) {
    count.catch += 1;
    return 'catch';
  } finally {
    count.finally += 1;
    return 'finally';
  }
  return 'wat';
};

assert(fn() === 'finally', '1: try Return === catch Return === finally Return; Completion: finally');
assert(count.catch === 0, '1');
assert(count.finally === 1, '1');

// 2: try Abrupt, catch Return, finally Return; Completion: finally
count.catch = 0;
count.finally = 0;
fn = function() {
  try {
    throw 'try';
  } catch(e) {
    count.catch += 1;
    return 'catch';
  } finally {
    count.finally += 1;
    return 'finally';
  }
  return 'wat';
};

assert(fn() === 'finally', '2: try Abrupt === catch Return === finally Return; Completion: finally');
assert(count.catch === 1, '2: catch count');
assert(count.finally === 1, '2: finally count');

// 3: try Abrupt, catch Abrupt, finally Normal; Completion: finally
count.catch = 0;
count.finally = 0;
fn = function() {
  try {
    throw 'try';
  } catch(e) {
    count.catch += 1;
    // fixme finally should have last say here, and not throw
    throw 'catch';
  } finally {
    count.finally += 1;
    return 'finally';
  }
  return 'wat';
};

assert(fn() === 'finally', '3: try Abrupt === catch Abrupt === finally Normal; Completion: finally');
assert(count.catch === 1, '3: catch count');
assert(count.finally === 1, '3: finally count');


//? test: completion-values
//? description:...
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


assert(
  eval('99; do { -99; try { 39 } catch (e) { -1 } finally { 42; break; -2 }; } while (false);'),
  42
);
assert(
  eval('99; do { -99; try { [].x.x } catch (e) { -1; } finally { 42; break; -3 }; } while (false);'),
  42
);
assert(
  eval('99; do { -99; try { 39 } catch (e) { -1 } finally { break; -2 }; } while (false);'),
  undefined
);
assert(
  eval('99; do { -99; try { [].x.x } catch (e) { -1; } finally { break; -3 }; } while (false);'),
  undefined
);
assert(
  eval('99; do { -99; try { 39 } catch (e) { -1 } finally { 42; break; -3 }; -77 } while (false);'),
  42
);
assert(
  eval('99; do { -99; try { [].x.x } catch (e) { -1; } finally { 42; break; -3 }; -77 } while (false);'),
  42
);
assert(
  eval('99; do { -99; try { 39 } catch (e) { -1 } finally { break; -3 }; -77 } while (false);'),
  undefined
);
assert(
  eval('99; do { -99; try { [].x.x } catch (e) { -1; } finally { break; -3 }; -77 } while (false);'),
  undefined
);
assert(
  eval('99; do { -99; try { 39 } catch (e) { -1 } finally { 42; continue; -3 }; } while (false);'),
  42
);
assert(
  eval('99; do { -99; try { [].x.x } catch (e) { -1; } finally { 42; continue; -3 }; } while (false);'),
  42
);
assert(
  eval('99; do { -99; try { 39 } catch (e) { -1 } finally { 42; continue; -3 }; -77 } while (false);'),
  42
);
assert(
  eval('99; do { -99; try { [].x.x } catch (e) { -1 } finally { 42; continue; -3 }; -77 } while (false);'),
  42
);




//? test: cptn-catch-empty-break
//? description: Abrupt completion from catch block calls UpdatEmpty()
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = eval("for (loc i = 0; i < 2; ++i) { if (i) { try { throw null; } catch (e) { break; } } 'bad completion'; }");
assert(completion === undefined);


//? test: cptn-catch-empty-continue
//? description: Abrupt completion from catch block calls UpdatEmpty()
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = eval("for (loc i = 0; i < 2; ++i) { if (i) { try { throw null; } catch (e) { continue; } } 'bad completion'; }");
assert(completion === undefined);


//? test: cptn-catch-finally-empty-break
//? description: Abrupt completion from finally block calls UpdatEmpty()
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = eval("for (loc i = 0; i < 2; ++i) { if (i) { try { throw null; } catch (e) {} finally { break; } } 'bad completion'; }");
assert(completion === undefined);


//? test: cptn-catch-finally-empty-continue
//? description: Abrupt completion from finally block calls UpdatEmpty()
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = eval("for (loc i = 0; i < 2; ++i) { if (i) { try { throw null; } catch (e) {} finally { continue; } } 'bad completion'; }");
assert(completion === undefined);


//? test: cptn-catch
//? description: Completion value from `catch` clause of a try..catch statement
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


assert(eval('1; try { throw null; } catch (err) { }') === undefined);
assert(eval('2; try { throw null; } catch (err) { 3; }') === 3);


//? test: cptn-finally-empty-break
//? description: Abrupt completion from finally block calls UpdatEmpty()
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = eval("for (loc i = 0; i < 2; ++i) { if (i) { try {} finally { break; } } 'bad completion'; }");
assert(completion === undefined);


//? test: cptn-finally-empty-continue
//? description: Abrupt completion from finally block calls UpdatEmpty()
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = eval("for (loc i = 0; i < 2; ++i) { if (i) { try {} finally { continue; } } 'bad completion'; }");
assert(completion === undefined);


//? test: cptn-finally-from-catch
//? description:...
    Completion value from `finally` clause of a try..catch..finally statement
    (following execution of `catch` block)
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


assert(
  eval('1; try { throw null; } catch (err) { } finally { }'), undefined
);
assert(
  eval('2; try { throw null; } catch (err) { 3; } finally { }'), 3
);
assert(
  eval('4; try { throw null; } catch (err) { } finally { 5; }'), undefined
);
assert(
  eval('6; try { throw null; } catch (err) { 7; } finally { 8; }'), 7
);


//? test: cptn-finally-skip-catch
//? description:...
    Completion value from `finally` clause of a try..catch..finally statement
    (when `catch` block is not executed)
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


assert(eval('1; try { } catch (err) { } finally { }') === undefined);
assert(eval('2; try { } catch (err) { 3; } finally { }') === undefined);
assert(eval('4; try { } catch (err) { } finally { 5; }') === undefined);
assert(eval('6; try { } catch (err) { 7; } finally { 8; }') === undefined);
assert(eval('9; try { 10; } catch (err) { } finally { }') === 10);
assert(eval('11; try { 12; } catch (err) { 13; } finally { }') === 12);
assert(eval('14; try { 15; } catch (err) { } finally { 16; }') === 15);
assert(eval('17; try { 18; } catch (err) { 19; } finally { 20; }') === 18);


//? test: cptn-finally-wo-catch
//? description: Completion value from `finally` clause of a try..finally statement
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw



assert(eval('1; try { } finally { }') === undefined);
assert(eval('2; try { 3; } finally { }') === 3);
assert(eval('4; try { } finally { 5; }') === undefined);
assert(eval('6; try { 7; } finally { 8; }') === 7);


//? test: cptn-try
//? description: Completion value from `try` clause of a try..catch statement
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


assert(eval('1; try { } catch (err) { }') === undefined);
assert(eval('2; try { 3; } catch (err) { }') === 3);
assert(eval('4; try { } catch (err) { 5; }') === undefined);
assert(eval('6; try { 7; } catch (err) { 8; }') === 7);

//? test: early-catch-duplicates
//? description:...
    It is a Syntax Error if BoundNames of CatchParameter contains any duplicate
    elements.
//? expect: error
//? source: ...
#!/usr/bin/env afw


try { } catch ([x, x]) {}


//? test: early-catch-function
//? description:...
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

function f() {
    try {
    } catch (e) {
        // fixme this should cause an error
        function e(){}
    }
}

return null;



//? test: early-catch-lex
//? description:...
    It is a Syntax Error if any element of the BoundNames of CatchParameter
    also occurs in the LexicallyDeclaredNames of Block.
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

// fixme this should throw an error
try { } catch (x) { loc x; }



//? test: optional-catch-binding-finally
//? description: try/catch/finally syntax with omission of the catch binding
//? expect: null
//? source: ...
#!/usr/bin/env afw


try {} catch {} finally {}


//? test: optional-catch-binding
//? description: try/catch syntax with omission of the catch binding
//? expect: null
//? source: ...
#!/usr/bin/env afw


try {} catch {}



//? test: optional-catch-binding-lexical
//? description: lexical environment runtime semantics for optional catch binding
//? expect: error:Parse error at offset 235 around line 20 column 5: Unknown built-in function y
//? source: ...
#!/usr/bin/env afw


loc x = 1;
loc ranCatch = false;

try {
    x = 2;
    throw "x";
} catch {
    loc x = 3;
    loc y = true;
    ranCatch = true;
}

assert(ranCatch, 'executed `catch` block');
assert(x === 2);

function f() {
    y = false;
}

return null;



//? test: optional-catch-binding-parens
//? description:...
//? expect: error
//? source: ...
#!/usr/bin/env afw


try {} catch () {}



//? test: S12.14_A10_T1
//? description:...
    Throwing exception while executing iteration statement placed into
    try Block
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc i=0;
try{
while(i<10){
  if(i===5) throw string(i);
  i+=1;
}
}
catch(e){
  if(e.message!=="5")
    throw '#1: Exception === 5. Actual:  Exception ==='+ e.message  ;
}


//? test: S12.14_A10_T2
//? description: Try statement inside loop, where use continue loop
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=0;
loc fin=0;
while(c1<2){
  try{
    c1+=1;
    continue;
  }
  catch(er1){}
  finally{
    fin=1;
  }
  fin=-1;
};
if(fin!==1){
  throw '#1: "finally" block must be evaluated at "try{continue} catch finally" construction';
}

// CHECK#2
loc c2=0;
loc fin2=0;
while(c2<2){
  try{
    throw "ex1";
  }
  catch(er1){
    c2+=1;
    continue;
  }
  finally{
    fin2=1;
  }
  fin2=-1;
}
if(fin2!==1){
  throw '#2: "finally" block must be evaluated at "try catch{continue} finally" construction';
}

// CHECK#3
loc c3=0;
loc fin3=0;
while(c3<2){
  try{
    throw "ex1";
  }
  catch(er1){
    c3+=1;
  }
  finally{
    fin3=1;
    continue;
  }
  fin3=0;
}
if(fin3!==1){
  throw '#3: "finally" block must be evaluated at "try catch finally{continue}" construction';
}

// CHECK#4
loc c4=0;
loc fin4=0;
while(c4<2){
  try{
    c4+=1;
    continue;
  }
  finally{
    fin4=1;
  }
  fin4=-1;
};
if(fin4!==1){
  throw '#4: "finally" block must be evaluated at "try{continue} finally" construction';
}

// CHECK#5
loc c5=0;
while(c5<2){
  try{
    throw "ex1";
  }
  catch(er1){
    c5+=1;
    continue;
  }
}
if(c5!==2){
  throw '#5: "try catch{continue}" must work correctly';
}

// CHECK#6
loc c6=0;
loc fin6=0;
while(c6<2){
  try{
    c6+=1;
    throw "ex1";
  }
  finally{
    fin6=1;
    continue;
  }
  fin6=-1;
}
if(fin6!==1){
  throw '#6.1: "finally" block must be evaluated';
}
if(c6!==2){
  throw '#6.2: "try finally{continue}" must work correctly';
}
