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
    throw '#1: Exception === 5. Actual:  Exception === '+ e.message  ;
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


//? test: S12.14_A10_T3
//? description: Try statement inside loop, where use break
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=0;
loc fin=0;
while(c1<2){
  try{
    c1+=1;
    break;
  }
  catch(er1){}
  finally{
    fin=1;
  }
  fin=-1;
  c1+=2;
}
if(fin!==1){
  throw '#1.1: "finally" block must be evaluated';
}
if(c1!==1){
  throw '#1.2: "try{break}catch finally" must work correctly';
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
    break;
  }
  finally{
    fin2=1;
  }
  c2+=2;
  fin2=-1;
}
if(fin2!==1){
  throw '#2.1: "finally" block must be evaluated';
}
if(c2!==1){
  throw '#2.2: "try catch{break} finally" must work correctly';
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
    break;
  }
  c3+=2;
  fin3=0;
}
if(fin3!==1){
  throw '#3.1: "finally" block must be evaluated';
}
if(c3!==1){
  throw '#3.2: "try catch finally{break}" must work correctly';
}

// CHECK#4
loc c4=0;
loc fin4=0;
while(c4<2){
  try{
    c4+=1;
    break;
  }
  finally{
    fin4=1;
  }
  fin4=-1;
  c4+=2;
}
if(fin4!==1){
  throw '#4.1: "finally" block must be evaluated';
}
if(c4!==1){
  throw '#4.2: "try{break} finally" must work correctly';
}

// CHECK#5
loc c5=0;
while(c5<2){
  try{
    throw "ex1";
  }
  catch(er1){
    break;
  }
}
if(c5!==0){
  throw '#5: "try catch{break}" must work correctly';
}

// CHECK#6
loc c6=0;
while(c6<2){
  try{
    c6+=1;
    break;
  }
  catch(er1){}
  c6+=2;
}
if(c6!==1){
  throw '#6: "try{break} catch" must work correctly';
}

// CHECK#7
loc c7=0;
loc fin7=0;
try{
  while(c7<2){
    try{
      c7+=1;
      throw "ex1";
    }
    finally{
      fin7=1;
      break;
    }
    fin7=-1;
    c7+=2;
  }
}
catch(ex1){
  c7=10;
}
if(fin7!==1){
  throw '#7.1: "finally" block must be evaluated';
}
if(c7!==1){
  throw '#7.2: "try finally{break}" must work correctly';
}



//? test: S12.14_A10_T4
//? description: Try statement inside loop, where combinate using break and continue
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=0;
loc fin=0;
while(c1<2){
  try{
    c1+=1;
    break;
  }
  catch(er1){}
  finally{
    fin=1;
    continue;
  }
  fin=-1;
  c1+=2;
}
if(fin!==1){
  throw '#1.1: "finally" block must be evaluated';
}
if(c1!==2){
  throw '#1.2: "try{break} catch finally{continue}" must work correctly';
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
    break;
  }
  finally{
    fin2=1;
    continue;
  }
  c2+=2;
  fin2=-1;
}
if(fin2!==1){
  throw '#2.1: "finally" block must be evaluated';
}
if(c2!==2){
  throw '#2.2: "try catch{break} finally{continue} must work correctly';
}


//? test: S12.14_A10_T5
//? description: Throw some exceptions from different place of loop body
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c=0;
loc i=0;
loc fin=0;
while(i<10){
  i+=1;
  try{
    if(c===0){
      throw "ex1";
      throw '#1.1: throw "ex1" lead to throwing exception';
    }
    c+=2;
    if(c===1){
      throw "ex2";
      throw '#1.2: throw "ex2" lead to throwing exception';
    }
  }
  catch(er1){
    c-=1;
    continue;
    throw '#1.3: "try catch{continue} finally" must work correctly';
  }
  finally{
    fin+=1;
  }
}
if(fin!==10){
  throw '#1.4: "finally" block must be evaluated';
}


//? test: S12.14_A11_T1
//? description: Loop inside try Block, where throw exception
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  for(loc i=0;i<10;i+=1){
    if(i===5) throw string(i);
  }
}
catch(e){
  if(e.message!=="5")throw '#1: Exception === 5. Actual:  Exception ==='+ e.message  ;
}


//? test: S12.14_A11_T2
//? description: Try statement inside loop, where use continue loop
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc fin=0;
for(loc i=0;i<5;i+=1){
  try{
    i+=1;
    continue;
  }
  catch(er1){}
  finally{
    fin=1;
  }
  fin=-1;
}
if(fin!==1){
  throw '#1: "finally" block must be evaluated at "try{continue} catch finally" construction';
}

// CHECK#2
loc c2=0;
loc fin2=0;
for(loc i=0;i<5;i+=1){
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
  throw '#2.1: "finally" block must be evaluated';
}
if(c2!==5){
  throw '#2.1: "try catch{continue} finally" must work correctly';
}

// CHECK#3
loc c3=0;
loc fin3=0;
for(loc i=0;i<5;i+=1){
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
  throw '#3.1: "finally" block must be evaluated';
}
if(c3!==5){
  throw '#3.2: "try catch finally{continue}" must work correctly';
}

// CHECK#4
fin=0;
for(loc i=0;i<5;i+=1){
  try{
    i+=1;
    continue;
  }
  finally{
    fin=1;
  }
  fin=-1;
};
if(fin!==1){
  throw '#4: "finally" block must be evaluated at "try{continue} finally" construction';
}

// CHECK#5
loc c5=0;
for(c5=0;c5<10;){
  try{
    throw "ex1";
  }
  catch(er1){
    c5+=1;
    continue;
  }
  c5+=12;
};
if(c5!==10){
  throw '#5: "try catch{continue} must work correctly"';
}

// CHECK#6
loc c6=0;
loc fin6=0;
for(c6=0;c6<10;){
  try{
    c6+=1;
    throw "ex1";
  }
  finally{
    fin6=1;
    continue;
  }
  fin6=-1;
};
if(fin6!==1){
  throw '#6.1: "finally" block must be evaluated';
}
if(c6!==10){
  throw '#6.2: "try finally{continue}" must work correctly';
}


//? test: S12.14_A11_T3
//? description: Try statement inside loop, where use break
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=0;
loc fin=0;
for(loc i=0;i<5;i+=1){
  try{
    c1+=1;
    break;
  }
  catch(er1){}
  finally{
    fin=1;
  }
  fin=-1;
  c1+=2;
};
if(fin!==1){
  throw '#1.1: "finally" block must be evaluated';
}
if(c1!==1){
  throw '#1.2: "try{break}catch finally" must work correctly';
}

// CHECK#2
loc c2=0;
loc fin2=0;
for(loc i=0;i<5;i+=1){
  try{
    throw "ex1";
  }
  catch(er1){
    c2+=1;
    break;
  }
  finally{
    fin2=1;
  }
  c2+=2;
  fin2=-1;
};
if(fin2!==1){
  throw '#2.1: "finally" block must be evaluated';
}
if(c2!==1){
  throw '#2.2: "try catch{break} finally" must work correctly';
}

// CHECK#3
loc c3=0;
loc fin3=0;
for(loc i=0;i<5;i+=1){
  try{
    throw "ex1";
  }
  catch(er1){
    c3+=1;
  }
  finally{
    fin3=1;
    break;
  }
  c3+=2;
  fin3=0;
};
if(fin3!==1){
  throw '#3.1: "finally" block must be evaluated';
}
if(c3!==1){
  throw '#3.2: "try catch finally{break}" must work correctly';
}

// CHECK#4
loc c4=0;
loc fin4=0;
for(loc i=0;i<5;i+=1){
  try{
    c4+=1;
    break;
  }
  finally{
    fin4=1;
  }
  fin4=-1;
  c4+=2;
};
if(fin4!==1){
  throw '#4.1: "finally" block must be evaluated';
}
if(c4!==1){
  throw '#4.2: "try{break} finally" must work correctly';
}

// CHECK#5
// fixme
loc i;
for(i=0;i<5;i+=1){
  try{
    throw "ex1";
  }
  catch(er1){
    break;
  }
};
if(i!==0){
  throw '#5: "try catch{break}" must work correctly';
}

// CHECK#6
loc c6=0;
for(c6=0;c6<5;){
  try{
    c6+=1;
    break;
  }
  catch(er1){}
  c6+=2;
};
if(c6!==1){
  throw '#6: "try{break} catch" must work correctly';
}

// CHECK#7
loc c7=0;
loc fin7=0;
try{
  for(c7=0;c7<5;){
    try{
      c7+=1;
      throw "ex1";
    }
    finally{
      fin7=1;
      break;
    }
    fin7=-1;
    c7+=2;
  }
}
catch(ex1){
  c7=10;
}
if(fin7!==1){
  throw '#7.1: "finally" block must be evaluated';
}
if(c7!==1){
  throw '#7.2: "try finally{break}" must work correctly';
}


//? test: S12.14_A11_T4
//? description: Try statement inside loop, where combinate using break and continue
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=0;
loc fin=0;
for(loc i=0;i<5;i+=1){
  try{
    c1+=1;
    break;
  }
  catch(er1){}
  finally{
    fin=1;
    continue;
  }
  fin=-1;
  c1+=2;
}
if(fin!==1){
  throw '#1.1: "finally" block must be evaluated';
}
if(c1!==5){
  throw '#1.2: "try{break} catch finally{continue}" must work correctly';
}

// CHECK#2
loc c2=0;
loc fin2=0;
for(loc i=0;i<5;i+=1){
  try{
    throw "ex1";
  }
  catch(er1){
    c2+=1;
    break;
  }
  finally{
    fin2=1;
    continue;
  }
  c2+=2;
  fin2=-1;
}
if(fin2!==1){
  throw '#2.1: "finally" block must be evaluated';
}
if(c2!==5){
  throw '#2.2: "try catch{break} finally{continue}" must work correctly';
}


//? test: S12.14_A12_T1
//? description: Loop inside try Block, where throw exception
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc mycars = [
    "Saab",
    "Volvo",
    "BMW"
];

// CHECK#1
try{
  for (loc x = 0; x <3; x += 1) {
    if (mycars[x]==="BMW") throw "ex";
  }
}
catch(e){
  if(e.message!=="ex")
    throw '#1: Exception ==="ex". Actual:  Exception ==='+ e.message  ;
}


//? test: S12.14_A12_T2
//? description: Try statement inside loop, where use continue loop
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


loc mycars = [
    "Saab",
    "Volvo",
    "BMW"
];

// CHECK#1
loc fin=0;
loc i=0;
foreach loc x of mycars {
  try{
    i+=1;
    continue;
  }
  catch(er1){}
  finally{
    fin=1;
  }
  fin=-1;
}
if(fin!==1){
  throw '#1.1: "finally" block must be evaluated';
}
if(i!==3){
  throw '#1.2:  "try{continue} catch finally" must work correctly';
}

// CHECK#2
loc c2=0;
loc fin2=0;
foreach loc x of mycars{
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
  throw '#2.1: "finally" block must be evaluated';
}
if(c2!==3){
  throw '#2.1: "try catch{continue} finally" must work correctly';
}

// CHECK#3
loc c3=0;
loc fin3=0;
foreach loc x of mycars{
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
if(c3!==3){
  throw '#3.1: "finally" block must be evaluated';
}
if(fin3!==1){
  throw '#3.2: "try catch finally{continue}" must work correctly';
}

// CHECK#4
loc fin=0;
foreach loc x of mycars{
  try{
    continue;
  }
  finally{
    fin=1;
  }
  fin=-1;
}
if(fin!==1){
  throw '#4: "finally" block must be evaluated at "try{continue} finally" construction';
}

// CHECK#5
loc c5=0;
// fixme this gives an error that x is already defined
foreach loc x of mycars{
  try{
    throw "ex1";
  }
  catch(er1){
    c5+=1;
    continue;
  }
  c5+=12;
}
if(c5!==3){
  throw '#5: "try catch{continue}" must work correctly';
}

// CHECK#6
loc c6=0;
loc fin6=0;
foreach loc x of mycars{
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
if(c6!==3){
  throw '#6.2: "try finally{continue}" must work correctly';
}



//? test: S12.14_A12_T3
//? description: Try statement inside loop, where use break
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


loc mycars = [
    "Saab",
    "Volvo",
    "BMW"
];

// CHECK#1
loc c1=0;
loc fin=0;
foreach loc x of mycars{
  try{
    c1+=1;
    break;
  }
  catch(er1){
    c1+=1;
  }
  finally{
    fin=1;
  }
  fin=-1;
  c1+=2;
};
if(fin!==1){
  throw '#1.1: "finally" block must be evaluated';
}
if(c1!==1){
  throw '#1.2: "try{break}catch finally" must work correctly';
}

// CHECK#2
loc c2=0;
loc fin2=0;
foreach loc x of mycars {
  try{
    throw "ex1";
  }
  catch(er1){
    c2+=1;
    break;
  }
  finally{
    fin2=1;
  }
  c2+=2;
  fin2=-1;
}
if(fin2!==1){
  throw '#2.1: "finally" block must be evaluated';
}
if(c2!==1){
  throw '#2.2: "try catch{break} finally" must work correctly';
}

// CHECK#3
loc c3=0;
loc fin3=0;
foreach loc x of mycars{
  try{
    throw "ex1";
  }
  catch(er1){
    c3+=1;
  }
  finally{
    fin3=1;
    break;
  }
  c3+=2;
  fin3=0;
}
if(fin3!==1){
  throw '#3.1: "finally" block must be evaluated';
}
if(c3!==1){
  throw '#3.2: "try catch finally{break}" must work correctly';
}

// CHECK#4
loc c4=0;
loc fin4=0;
foreach loc x of mycars {
  try{
    c4+=1;
    break;
  }
  finally{
    fin4=1;
  }
  fin4=-1;
  c4+=2;
}
if(fin4!==1){
  throw '#4.1: "finally" block must be evaluated';
}
if(c4!==1){
  throw '#4.2: "try{break} finally" must work correctly';
}

// CHECK#5
loc c5=0;
// fixme this gives an error that x is already defined
foreach loc x of mycars {
  try{
    throw "ex1";
    c5++;
  }
  catch(er1){
    break;
    c5++;
  }
  c5++;
}
if(c5!==0){
  throw '#5: "try catch{break}" must work correctly';
}

// CHECK#6
loc c6=0;
foreach loc x of mycars{
  try{
    c6+=1;
    break;
  }
  catch(er1){}
  c6+=2;
}
if(c6!==1){
  throw '#6: "try{break} catch" must work correctly';
}

// CHECK#7
loc c7=0;
loc fin7=0;
try{
  foreach loc x of mycars{
    try{
      c7+=1;
      throw "ex1";
    }
    finally{
      fin7=1;
      break;
    }
    fin7=-1;
    c7+=2;
  }
}
catch(ex1){
  c7=10;
}
if(fin7!==1){
  throw '#7.1: "finally" block must be evaluated';
}
if(c7!==1){
  throw '#7.2: "try finally{break}" must work correctly';
}


//? test: S12.14_A12_T4
//? description: Try statement inside loop, where combinate using break and continue
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


loc x;
loc mycars = [
    "Saab",
    "Volvo",
    "BMW"
];

// CHECK#1
loc c1=0;
loc fin=0;
// fixme this gives an error that x is already defined
foreach loc x in mycars {
  try{
    c1+=1;
    break;
  }
  catch(er1){}
  finally{
    fin=1;
    continue;
  }
  fin=-1;
  c1+=2;
}
if(fin!==1){
  throw '#1.1: "finally" block must be evaluated';
}
if(c1!==3){
  throw '#1.2: "try{break} catch finally{continue}" must work correctly';
}

// CHECK#2
loc c2=0;
loc fin2=0;
foreach loc x in mycars{
  try{
    throw "ex1";
  }
  catch(er1){
    c2+=1;
    break;
  }
  finally{
  	fin2=1;
  	continue;
  }
  c2+=2;
  fin2=-1;
}
if(fin2!==1){
  throw '#2.1: "finally" block must be evaluated';
}
if(c2!==3){
  throw '#2.2: "try catch{break} finally{continue}" must work correctly';
}


//? test: S12.14_A13_T1
//? description: Using try/catch syntax construction
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// CHECK#1
function myFunction1(){
  try{
    return 1;
  }
  catch(err){
  	throw '#1.1: "return 1" inside function does not lead to throwing exception';
    return 0;
  }
  return 2;
}
loc x1=myFunction1();
if(x1!==1){
  throw '#1.2: x1===1. Actual: x1==='+x1;
}

// CHECK#2
function myFunction2(){
  try{
    throw "exc";
    return 1;
  }catch(err){
    return 2;
  }
  return 3;
}
loc x2=myFunction2();
if (x2!==2){
  throw '#2: x2===2. Actual: x2==='+x2;
}

// CHECK#3
function myFunction3(){
  try{
    // fixme this gives an error that someValue is not defined
    return someValue;
  }catch(err){
    return 1;
  }
  return 2;
}
loc x3=myFunction3();
if (x3!==1){
  throw '#3: x3===1. Actual: x3==='+x3;
}

// CHECK#4
function myFunction4(){
  try{
    throw "ex1";
    return 1;
  }catch(err){
    throw "ex2"
    return 0;
  }
  return 2;
}
try{
  loc x4=myFunction4();
  throw '#4.1: Throwing exception inside function lead to throwing exception outside this function';
}
catch(e){
  if(e==="ex1"){
    throw '#4.2: Exception !=="ex1". Actual: catch previous exception';
  }
  if(e!=="ex2"){
    throw '#4.3: Exception ==="ex2". Actual:  Exception ==='+ e  ;
  }
}


//? test: S12.14_A13_T2
//? description: Using try/finally syntax construction
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=0;
function myFunction1(){
  try{
    return 1;
  }finally{
    c1=1;
  }
  return 2;
}
loc x1=myFunction1();
if(x1!==1){
  throw '#1.1: x1===1. Actual: x1==='+x1;
}
if (c1!==1){
  throw '#1.2: "finally" block must be evaluated';
}

// CHECK#2
loc c2=0;
function myFunction2(){
  try{
    throw "exc";
    return 1;
  }finally{
    c2=1;
  }
  return 2;
}
try{
  loc x2=myFunction2();
  throw '#2.1: Throwing exception inside function lead to throwing exception outside this function';
}
catch(e){
  if (c2!==1){
    throw '#2.2: "finally" block must be evaluated';
  }
}

// CHECK#3
loc c3=0;
function myFunction3(){
  try{
    // fixme this gives an error that someValue is not defined
    return someValue;
  }finally{
    c3=1;
  }
  return 2;
}
try{
  loc x3=myFunction3();
  throw '#3.1: Throwing exception inside function lead to throwing exception outside this function';
}
catch(e){
  if (c3!==1){
    throw '#3.2: "finally" block must be evaluated';
  }
}

// CHECK#4
loc c4=0;
function myFunction4(){
  try{
    return 1;
  }finally{
    c4=1;
    throw "exc";
    return 0;
  }
  return 2;
}
try{
  loc x4=myFunction4();
  throw '#4.2: Throwing exception inside function lead to throwing exception outside this function';
}
catch(e){
  if (c4!==1){
    throw '#4.3: "finally" block must be evaluated';
  }
}

// CHECK#5
loc c5=0;
function myFunction5(){
  try{
    return 1;
  }finally{
    c5=1;
    return someValue;
    return 0;
  }
  return 2;
}
try{
  loc x5=myFunction5();
  throw '#5.2: Throwing exception inside function lead to throwing exception outside this function';
}
catch(e){
  if (c5!==1){
    throw '#5.3: "finally" block must be evaluated';
  }
}

// CHECK#6
loc c6=0;
function myFunction6(){
  try{
    throw "ex1";
    return 1;
  }finally{
    c6=1;
    throw "ex2";
    return 2;
  }
  return 3;
}
try{
  loc x6=myFunction6();
  throw '#6.1: Throwing exception inside function lead to throwing exception outside this function';
}
catch(e){
  if(e==="ex1"){
    throw '#6.2: Exception !=="ex1". Actual: catch previous exception';
  }
  if(e!=="ex2"){
    throw '#6.3: Exception !=="ex1". Actual: '+e;
  }
  if (c6!==1){
    throw '#6.4: "finally" block must be evaluated';
  }
}

// CHECK#7
loc c7=0;
function myFunction7(){
  try{
    return 1;
  }finally{
    c7=1;
    return 2;
  }
  return 3;
}
loc x7=myFunction7();
if(x7!==2){
  throw '#7.1: "catch" block must be evaluated';
}
if (c7!==1){
  throw '#7.2: "finally" block must be evaluated';
}

// CHECK#8
loc c8=0;
function myFunction8(){
  try{
    throw "ex1";
  }finally{
    c8=1;
    return 2;
  }
  return 3;
}
try{
  loc x8=myFunction8();
}
catch(ex1){
  c8=10;
}
if (c8!==1){
  throw '#8: "finally" block must be evaluated';
}

//? test: S12.14_A13_T3
//? description: Using try/catch/finally syntax construction
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=0;
function myFunction1(){
  try{
    return 1;
  }catch(err){
    throw '#1.1: "return 1" inside function does not lead to throwing exception';
    return 0;
  }finally{
    c1=1;
  }
  return 2;
}
loc x1=myFunction1();
if(x1!==1){
  throw '#1.3: x1===1. Actual: x1==='+x1;
}
if (c1!==1){
  throw '#1.4: "finally" block must be evaluated';
}

// CHECK#2
loc c2=0;
function myFunction2(){
  try{
    throw "exc";
    return 1;
  }catch(err){
    return 0;
  }finally{
    c2=1;
  }
  return 2;
}
loc x2=myFunction2();
if (c2!==1){
  throw '#2.1: "finally" block must be evaluated';
}
if (x2!==0){
  throw '#2.2: x2===0. Actual: x2==='+x2;
}

// CHECK#3
loc c3=0;
function myFunction3(){
  try{
    // fixme this gives an error that someValue is not defined
    return someValue;
  }catch(err){
    return 1;
  }finally{
    c3=1;
  }
  return 2;
}
loc x3=myFunction3();
if (c3!==1){
  throw '#3.1: "finally" block must be evaluated';
}
if (x3!==1){
  throw '#3.2: x3===1. Actual: x3==='+x3;
}

// CHECK#4
loc c4=0;
function myFunction4(){
  try{
    throw "ex1";
    return 1;
  }catch(err){
    throw "ex2"
    return 0;
  }finally{
    c4=1;
  }
  return 2;
}
try{
  loc x4=myFunction4();
  throw '#4.1: Throwing exception inside function lead to throwing exception outside this function';
}
catch(e){
  if(e==="ex1"){
    throw '#4.2: Exception !== "ex1". Actual: catch previous exception';
  }
  if(e!=="ex2"){
    throw '#4.3: Exception === "ex2". Actual:  Exception ==='+ e  ;
  }
  if (c4!==1){
    throw '#4.4: "finally" block must be evaluated';
  }
}

// CHECK#5
loc c5=0;
function myFunction5(){
  try{
    throw "ex1";
    return 1;
  }catch(err){
    return 0;
  }finally{
    c5=1;
    throw "ex2";
  }
  return 2;
}
try{
  loc x5=myFunction5();
  throw '#5.1: Throwing exception inside function lead to throwing exception outside this function';
}
catch(e){
  if(e==="ex1"){
    throw '#5.2: Exception !== "ex1". Actual: catch previous exception';
  }
  if(e!=="ex2"){
    throw '#5.3: Exception === "ex2". Actual:  Exception ==='+ e  ;
  }
  if (c5!==1){
    throw '#5.4: "finally" block must be evaluated';
  }
}

// CHECK#6
loc c6=0;
function myFunction6(){
  try{
    throw "ex1";
    return 1;
  }catch(err){
    throw "ex2";
    return 0;
  }finally{
    c6=1;
    throw "ex3";
  }
  return 2;
}
try{
  loc x6=myFunction6();
  throw '#6.1: Throwing exception inside function lead to throwing exception outside this function';
}
catch(e){
  if(e==="ex1"){
    throw '#6.2: Exception !== "ex1". Actual: catch previous exception';
  }
  if(e==="ex2"){
    throw '#6.3: Exception !== "ex2". Actual: catch previous exception';
  }
  if(e!=="ex3"){
    throw '#6.4: Exception === "ex3". Actual:  Exception ==='+ e  ;
  }
  if(c6!==1) throw '#6.5: "finally" block must be evaluated';
}

// CHECK#7
loc c7=0;
function myFunction7(){
  try{
    throw "ex1";
    return 1;
  }catch(err){
    throw "ex2";
    return 0;
  }finally{
    c7=1;
    return 2;
  }
  return 3;
}
try{
  loc x7=myFunction7();
  if(x7!==2) throw '#7.1: x7===2. Actual: x7==='+x7;
}
catch(e){}
if(c7!==1) throw '#7.2: "finally" block must be evaluated';


//? test: S12.14_A14
//? description: Using try/catch/finally in With and With in try/catch/finally
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc myObj = {p1: 'a',
             p2: 'b',
             p3: 'c',
             value: 'myObj_value',
             valueOf : function(){return 'obj_valueOf';},
             parseInt : function(){return 'obj_parseInt';},
             NaN : 'obj_NaN',
             Infinity : 'obj_Infinity',
             eval     : function(){return 'obj_eval';},
             parseFloat : function(){return 'obj_parseFloat';},
             isNaN      : function(){return 'obj_isNaN';},
             isFinite   : function(){return 'obj_isFinite';}
}

// CHECK#1
try{
  with(myObj){
    throw "ex";
  }
}
catch(e){
  if (e!=="ex") throw '#1: Exception ==="ex". Actual:  Exception ==='+ e  ;
}

// CHECK#2
with(myObj){
  try{
    throw p1;
  }
  catch(e){
    if (e!=="a") throw '#2.1: Exception ==="a". Actual:  Exception ==='+ e  ;
    p1='pass';
  }
}
if(myObj.p1!=='pass') throw '#2.2: "throw p1" lead to throwing exception';

// CHECK#3
with(myObj){
  try{
    p1='fail';
    throw p2;
  }
  catch(e){
    if (e!=="b") throw '#3.1: Exception ==="b". Actual:  Exception ==='+ e  ;
    p1='pass';
  }
  finally{
    p2='pass';
  }
}
if(myObj.p1!=='pass') throw '#3.2: "throw p2" lead to throwing exception';
if(myObj.p2!=='pass') throw '#3.3: "finally" block must be evaluated';

// CHECK#4
myObj.p1='fail';
try{
  with(myObj){
    try{
      throw p3;
    }
    finally{
      p1='pass';
    }
  }
}
catch(e){}
if(myObj.p1!=='pass') throw '#4: "finally" block must be evaluated';


//? test: S12.14_A15
//? description: Insert try/catch/finally to switch statement
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// CHECK#1
function SwitchTest1(value){
  loc result = 0;
  try{
    // fixme need switch first
    switch(value) {
      case 1:
        result += 4;
        throw result;
        break;
      default:
        result += 32;
        break;
      case 4:
        result += 64;
        throw "ex";
    }
    return result;
  }
  catch(e){
    if ((value===1)&&(e!==4)) throw '#1.1: Exception ===4. Actual:  Exception ==='+ e  ;
    if ((value===4)&&(e!=="ex")) throw '#1.2: Exception ==="ex". Actual:  Exception ==='+ e  ;
  }
  finally{
    return result;
  }
}
if (SwitchTest1(1)!==4) throw '#1.3: SwitchTest1(1)===4. Actual:  SwitchTest1(1)==='+ SwitchTest1(1) ;
if (SwitchTest1(4)!==64) throw '#1.4: SwitchTest1(4)===64. Actual:  SwitchTest1(4)==='+ SwitchTest1(4) ;

// CHECK#2
loc c2=0;
function SwitchTest2(value){
  loc result = 0;
  switch(value) {
    case 0:
      try{
        result += 2;
        break;
      }
      finally{
        c2=1;
      }
    case 1:
      result += 4;
      break;
    default:
      result += 32;
      break;
  }
  return result;
}
if (SwitchTest2(1)!==4) throw '#2.1: SwitchTest1(1)===4. Actual:  SwitchTest1(1)==='+ SwitchTest1(1) ;
if (c2===1) throw '#2.2: Evaluate finally block';
if (SwitchTest2(0)!==2) throw '#2.3: SwitchTest1(0)===2. Actual:  SwitchTest1(0)==='+ SwitchTest1(0) ;
if (c2!==1) throw '#2.4: "finally" block must be evaluated';

// CHECK#3
function SwitchTest3(value){
  loc result = 0;
  switch(value) {
    case 0:
      try{
        result += 2;
        throw "ex";
      }
      finally{
        break;
      }
    default:
      result += 32;
      break;
  }
  return result;
}
try{
  loc x3=SwitchTest3(0);
  if (x3!==2) throw '#3.1: x3===2. Actual: x3==='+x3;
}
catch(e){
  throw '#3.2: Catching exception inside function does not lead to throwing exception outside this function';
}


//? test: S12.14_A16_T10
//? description: "Catch: \"catch (Identifier ) Block\""
//? expect: error
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{}
catch(){}
finally{}


//? test: S12.14_A16_T11
//? description:...
    Catch and Finally are placed into the Block of "try" (whitle
    expected outside)
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
try{
  {
  }
  catch(e){}
  finally{}
}


//? test: S12.14_A16_T12
//? description: Embedded "try" statements followed by two "catch" statements
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
try
{
  try
  {
  }
}
catch(e1){}
catch(e2){}



//? test: S12.14_A16_T13
//? description:...
    Catch: "catch (Identifier ) Block". Checking if execution of "22"
    passes at the place of Identifier of "catch"
//? expect: error
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try
{
}
catch("22")
{
}


//? test: S12.14_A16_T14
//? description: Checking if passing argument to "try" statement fails
//? expect: error
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try(e1){
}
catch(e){}


//? test: S12.14_A16_T15
//? description:...
    Finally: "finally Block". Checking if passing argument to "try"
    statement fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
try{
}
finally(e){}


//? test: S12.14_A16_T1
//? description: Checking if pure "try" syntax construction passes
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
try


//? test: S12.14_A16_T2
//? description: Checking if execution of "catch" with no "try" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
catch


//? test: S12.14_A16_T3
//? description: Checking if execution of "finally" with no "try" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
finally


//? test: S12.14_A16_T5
//? description:...
    Catch: "catch (Identifier ) Block". Checking if execution of
    "catch" with no Block fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
try{}
catch()


//? test: S12.14_A16_T6
//? description:...
    Block: "{ StatementList }". Checking if execution of "try{
    catch{}{}" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
try{
catch(){}


//? test: S12.14_A16_T7
//? description:...
    Block: "{ StatementList }". Checking if execution of "try{}
    catch(){" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
try{}
catch(){


//? test: S12.14_A16_T8
//? description:...
    Block: "{ StatementList }". Catch: "catch (Identifier ) Block".
    Checking if execution of "try{} catch(){finally{}" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
try{}
catch(){
finally{}


//? test: S12.14_A16_T9
//? description: Checking if execution of "catch(){} finally{}" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




// CHECK#1
catch(){}
finally{}



//? test: S12.14_A17
//? description: Creating exceptions within constructor
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc i=1;
function Integer( value, exception ) {
  try{
    this.value = checkValue( value );
    if(exception) throw '#'+i+'.1: Must be exception';
  }
  catch(e){
    this.value = e.toString();
    if(!exception) throw '#'+i+'.2: Don`t must be exception';
  }
  i++;
}

function checkValue(value){
  if(Math.floor(value)!=value||isNaN(value)){
    throw (INVALID_INTEGER_VALUE +": " + value);
  }
  else{
    return value;
  }
}

// CHECK#1
new Integer(13, false);
// CHECK#2
new Integer(NaN, true);
// CHECK#3
new Integer(0, false);
// CHECK#4
new Integer(Infinity, false);
// CHECK#5
new Integer(-1.23, true);
// CHECK#6
new Integer(Math.LN2, true);


//? test: S12.14_A18_T1
//? description: Catching undefined
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  // we cannot do this with our throw
  throw undefined;
}
catch(e){
  if (e.message!==undefined) throw '#1: Exception === undefined. Actual: '+e.message;
}


//? test: S12.14_A18_T2
//? description: Catching null
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  // we can't do this with our throw
  throw null;
}
catch(e){
  if (e!==null) throw '#1: Exception ===null. Actual: '+e;
}


//? test: S12.14_A18_T3
//? description: Catching boolean
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  throw "" true;
}
catch(e){
  if (e.data!==true) throw '#1: Exception ===true. Actual:  Exception ==='+ e.data  ;
}

// CHECK#2
try{
  throw "" false;
}
catch(e){
  if (e.data!==false) throw '#2: Exception ===false. Actual:  Exception ==='+ e.data  ;
}

// CHECK#3
loc b=false;
try{
  throw "" b;
}
catch(e){
  if (e.data!==false) throw '#3: Exception ===false. Actual:  Exception ==='+ e.data  ;
}

// CHECK#4
b=true;
try{
  throw "" b;
}
catch(e){
  if (e.data!==true) throw '#4: Exception ===true. Actual:  Exception ==='+ e.data  ;
}

// CHECK#5
b=true;
try{
  throw "" b&&false;
}
catch(e){
  if (e.data!==false) throw '#5: Exception ===false. Actual:  Exception ==='+ e.data  ;
}

// CHECK#5
b=true;
try{
  throw "" b||false;
}
catch(e){
  if (e.data!==true) throw '#6: Exception ===true. Actual:  Exception ==='+ e.data  ;
}



//? test: S12.14_A18_T4
//? description: Catching string
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  throw "exception #1";
}
catch(e){
  if (e.message!=="exception #1") throw '#1: Exception ==="exception #1". Actual:  Exception ==='+ e.message  ;
}

// CHECK#2
try{
  throw "exception"+" #1";
}
catch(e){
  if (e.message!=="exception #1") throw '#2: Exception ==="exception #1". Actual:  Exception ==='+ e.message  ;
}

// CHECK#3
loc b="exception #1";
try{
  throw b;
}
catch(e){
  if (e.message!=="exception #1") throw '#3: Exception ==="exception #1". Actual:  Exception ==='+ e.message  ;
}

// CHECK#4
loc a="exception";
b=" #1";
try{
  throw a+b;
}
catch(e){
  if (e.message!=="exception #1") throw '#4: Exception ==="exception #1". Actual:  Exception ==='+ e.message  ;
}


//? test: S12.14_A18_T5
//? description: Catching Number
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  throw "" 13;
}
catch(e){
  if (e.data!==13) throw '#1: Exception ===13. Actual:  Exception ==='+ e.data  ;
}

// CHECK#2
try{
  throw "" 10+3;
}
catch(e){
  if (e.data!==13) throw '#2: Exception ===13. Actual:  Exception ==='+ e.data  ;
}

// CHECK#3
loc b=13;
try{
  throw "" b;
}
catch(e){
  if (e.data!==13) throw '#3: Exception ===13. Actual:  Exception ==='+ e.data  ;
}

// CHECK#4
loc a=3;
loc b=10;
try{
  throw "" a+b;
}
catch(e){
  if (e.data!==13) throw '#4: Exception ===13. Actual:  Exception ==='+ e.data  ;
}

// CHECK#5
try{
  throw "" 2.13;
}
catch(e){
  if (e.data!==2.13) throw '#5: Exception ===2.13. Actual:  Exception ==='+ e.data  ;
}

// CHECK#6
loc ex=2/3;
try{
  throw "" 2/3;
}
catch(e){
  if (e.data!==ex) throw '#6: Exception ===2/3. Actual:  Exception ==='+ e.data  ;
}

// CHECK#7
try{
  throw "" NaN;
}
catch(e){
  assert(isNaN(e.data), "e is NaN");
}

// CHECK#8
try{
  throw  "" +Infinity;
}
catch(e){
  // fixme maybe this should work?
  if (e.data!==+Infinity) throw '#8: Exception ===+Infinity. Actual:  Exception ==='+ string(e.data);
}

// CHECK#9
try{
  throw "" -Infinity;
}
catch(e){
  if (e.data!==-Infinity) throw '#9: Exception ===-Infinity. Actual:  Exception ==='+ e.data  ;
}

// CHECK#10
try{
  throw "" +0;
}
catch(e){
  if (e.data!==+0) throw '#10: Exception ===+0. Actual:  Exception ==='+ e.data  ;
}

// CHECK#11
try{
  throw "" -0;
}
catch(e){
  assert(e.data === -0);
}


//? test: S12.14_A18_T6
//? description: Catching Object
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc myObj = {p1: 'a',
             p2: 'b',
             p3: 'c',
             value: 'myObj_value',
             valueOf : function(){return 'obj_valueOf';},
             parseInt : function(){return 'obj_parseInt';},
             NaN : 'obj_NaN',
             Infinity : 'obj_Infinity',
             eval     : function(){return 'obj_eval';},
             parseFloat : function(){return 'obj_parseFloat';},
             isNaN      : function(){return 'obj_isNaN';},
             isFinite   : function(){return 'obj_isFinite';},
             i:7,
}

try{
  throw myObj;
}
catch(e){
// CHECK#1
  if (e.p1!=="a") throw '#1: e.p1==="a". Actual:  e.p1==='+ e.p1 ;
// CHECK#2
  if (e.value!=='myObj_value') throw '#2: e.value===\'myObj_value\'. Actual:  e.value==='+ e.value ;
// CHECK#3
  if (e.eval()!=='obj_eval') throw '#3: e.eval()===\'obj_eval\'. Actual:  e.eval()==='+ e.eval() ;
}

// CHECK#4
myObj.i=6;
try{
  throw myObj;
}
catch(e){}
if (myObj.i!==6) throw '#4: Handling of catch must be correct';

// CHECK#5
myObj.i=6;
try{
  throw myObj;
}
catch(e){
  e.i=10;
}
if (myObj.i!==10) throw '#5: Handling of catch must be correct';


//? test: S12.14_A18_T7
//? description: Catching Array
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw


loc mycars = [
    "Saab",
    "Volvo",
    "BMW"
];

loc mycars2 = [
    "Mercedes",
    "Jeep",
    "Suzuki"
];

// CHECK#1
try{
  throw "" mycars;
}
catch(e){
  for (loc i=0;i<3;i+=1){
    if (e[i]!==mycars[i]) throw '#1.'+i+': Exception['+i+']===mycars['+i+']. Actual:  Exception['+i+']==='+ e[i] ;
  }
}

// CHECK#2
try{
  throw mycars.concat(mycars2);
}
catch(e){
  for (loc i=0;i<3;i++){
    if (e[i]!==mycars[i]) throw '#2.'+i+': Exception['+i+']===mycars['+i+']. Actual:  Exception['+i+']==='+ e[i] ;
  }
  for (loc i=3;i<6;i++){
    if (e[i]!==mycars2[i-3]) throw '#2.'+i+': Exception['+i+']===mycars2['+i+']. Actual:  Exception['+i+']==='+ e[i] ;
  }
}

// CHECK#3
try{
  throw new Array("Mercedes","Jeep","Suzuki");
}
catch(e){
  for (loc i=0;i<3;i++){
    if (e[i]!==mycars2[i]) throw '#3.'+i+': Exception['+i+']===mycars2['+i+']. Actual:  Exception['+i+']==='+ e[i];
  }
}

// CHECK#4
try{
  throw mycars.concat(new Array("Mercedes","Jeep","Suzuki"));
}
catch(e){
  for (loc i=0;i<3;i++){
    if (e[i]!==mycars[i]) throw '#4.'+i+': Exception['+i+']===mycars['+i+']. Actual:  Exception['+i+']==='+ e[i] ;
  }
  for (loc i=3;i<6;i++){
    if (e[i]!==mycars2[i-3]) throw '#4.'+i+': Exception['+i+']===mycars2['+(i-3)+']. Actual:  Exception['+i+']==='+ e[i];
  }
}

//? test: S12.14_A19_T1
//? description: Testing try/catch syntax construction
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// CHECK#1
try{
  throw (Error("hello"));
}
catch(e){
  if (e.toString()!=="Error: hello") throw '#1: Exception.toString()==="Error: hello". Actual: Exception is '+e;
}

// CHECK#2
try{
  throw (new Error("hello"));
}
catch(e){
  if (e.toString()!=="Error: hello") throw '#2: Exception.toString()==="Error: hello". Actual: Exception is '+e;
}

// CHECK#3
loc c3=0;
try{
  throw EvalError(1);
}
catch(e){
  if (e.toString()!=="EvalError: 1") throw '#3: Exception.toString()==="EvalError: 1". Actual: Exception is '+e;
}

// CHECK#4
try{
  throw RangeError(1);
}
catch(e){
  if (e.toString()!=="RangeError: 1") throw '#4: Exception.toString()==="RangeError: 1". Actual: Exception is '+e;
}

// CHECK#5
try{
  throw ReferenceError(1);
}
catch(e){
  if (e.toString()!=="ReferenceError: 1") throw '#5: Exception.toString()==="ReferenceError: 1". Actual: Exception is '+e;
}

// CHECK#6
loc c6=0;
try{
  throw TypeError(1);
}
catch(e){
  if (e.toString()!=="TypeError: 1") throw '#6: Exception.toString()==="TypeError: 1". Actual: Exception is '+e;
}

// CHECK#7
try{
  throw URIError("message", "fileName", "1");
}
catch(e){
  if (e.toString()!=="URIError: message") throw '#7: Exception.toString()==="URIError: message". Actual: Exception is '+e;
}
