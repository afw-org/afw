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
//? source: ...
#!/usr/bin/env afw

function f(o) {

    function innerf(o) {
      try {
        throw '' o;
      }
      catch (e) {
        return e.data.x;
      }
    }

    return innerf(o);
}

assert(f({x:42}) === 42);


//? test: 12.14-13
//? description: catch introduces scope - updates are based on scope
//? expect: error:Parse error at offset 194 around line 12 column 13: Unknown built-in function this
//? source: ...
#!/usr/bin/env afw

loc res1 = false;
loc res2 = false;
loc res3 = false;

// fixme: we do not have "this".
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
//? expect: error:Parse error at offset 33 around line 3 column 14: Unknown built-in function this
//? source: ...
#!/usr/bin/env afw

loc global = this;
loc result;

// fixme - we don't have "this".
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
//? expect: error:Parse error at offset 33 around line 3 column 14: Unknown built-in function this
//? source: ...
#!/usr/bin/env afw

loc global = this;
loc result;

// fixme - we don't have "this"
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
//? expect: error:Parse error at offset 33 around line 3 column 14: Unknown built-in function this
//? source: ...
#!/usr/bin/env afw

loc global = this;
loc result;

// fixme - we don't have "this".
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
assert(variable_exists("foo") === false);


//? test: 12.14-7
//? description: catch introduces scope - scope removed when exiting catch block
//? expect:error:Parse error at offset 282 around line 18 column 5: Unknown built-in function expObj
//? source: ...
#!/usr/bin/env afw

loc o = {foo: 1};
loc catchAccessed = false;

try {
    throw 'x' o;
}
catch (expObj) {
    catchAccessed = (expObj.data.foo == 1);
}

assert(catchAccessed, '(expObj.foo == 1)');

// can't catch ReferenceError in adaptive script
catchAccessed = false;
try {
    expObj;
}
catch (e) {
    catchAccessed = e instanceof ReferenceError
}
assert(catchAccessed, 'e instanceof ReferenceError');


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

loc err = false;
try {
    fn();
} catch (e) {
    err = true;
    assert(count.catch === 1, '1: catch count');
    assert(count.finally === 1, '1: finally count');
}
// fixme doesn't work yet
assert(err, '1: try Abrupt, catch Abrupt, finally Abrupt; Completion: finally');


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

err = false;
try {
    fn();
} catch (e) {
    err = true;
    assert(count.catch === 1, '2: catch count');
    assert(count.finally === 1, '2: finally count');
}
assert(err, '2: try Abrupt, catch Return, finally Abrupt; Completion: finally');


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

err = false;
try {
    fn();
} catch (e) {
    err = true;
    assert(count.catch === 0, '3: catch count');
    assert(count.finally === 1, '3: finally count');
}
assert(err, '3: try Normal, catch Normal, finally Abrupt; Completion: finally');



//? test: completion-values-fn-finally-normal
//? description:...
//? expect: null
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
    loc x = 'normal';
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
    loc x = 'finally';
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
    loc x = 'finally';
  }
  return 'wat';
};

loc caught = false;
try {
    fn();
} catch (e) {
    caught = true;
}
assert(caught, '3: try Abrupt, catch Abrupt, finally Normal; Completion: catch');
assert(count.catch === 1, '3: catch count');
assert(count.finally === 1, '3: finally count');


//? test: completion-values-fn-finally-return
//? description:...
//? expect: null
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
//? expect: error:Parse error at offset 2 around line 1 column 3: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('99; do { -99; try { 39 } catch (e) { -1 } finally { 42; break; -2 }; } while (false);')) === 42
);
assert(
  evaluate(script('99; do { -99; try { [].x.x } catch (e) { -1; } finally { 42; break; -3 }; } while (false);')) === 42
);
assert(
  evaluate(script('99; do { -99; try { 39 } catch (e) { -1 } finally { break; -2 }; } while (false);')) === undefined
);
assert(
  evaluate(script('99; do { -99; try { [].x.x } catch (e) { -1; } finally { break; -3 }; } while (false);')) === undefined
);
assert(
  evaluate(script('99; do { -99; try { 39 } catch (e) { -1 } finally { 42; break; -3 }; -77 } while (false);')) === 42
);
assert(
  evaluate(script('99; do { -99; try { [].x.x } catch (e) { -1; } finally { 42; break; -3 }; -77 } while (false);')) ===
  42
);
assert(
  evaluate(script('99; do { -99; try { 39 } catch (e) { -1 } finally { break; -3 }; -77 } while (false);')) ===
  undefined
);
assert(
  evaluate(script('99; do { -99; try { [].x.x } catch (e) { -1; } finally { break; -3 }; -77 } while (false);')) ===
  undefined
);
assert(
  evaluate(script('99; do { -99; try { 39 } catch (e) { -1 } finally { 42; continue; -3 }; } while (false);')) ===
  42
);
assert(
  evaluate(script('99; do { -99; try { [].x.x } catch (e) { -1; } finally { 42; continue; -3 }; } while (false);')) ===
  42
);
assert(
  evaluate(script('99; do { -99; try { 39 } catch (e) { -1 } finally { 42; continue; -3 }; -77 } while (false);')) ===
  42
);
assert(
  evaluate(script('99; do { -99; try { [].x.x } catch (e) { -1 } finally { 42; continue; -3 }; -77 } while (false);')) ===
  42
);




//? test: cptn-catch-empty-break
//? description: Abrupt completion from catch block calls UpdatEmpty()
//? expect:error:Parse error at offset 99 around line 1 column 100: Invalid statement
//? source: ...
#!/usr/bin/env afw

// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = evaluate(script("for (loc i = 0; i < 2; i+=1) { if (i) { try { throw null; } catch (e) { break; } } 'bad completion'; }"));
assert(completion === undefined);


//? test: cptn-catch-empty-continue
//? description: Abrupt completion from catch block calls UpdatEmpty()
//? expect: error:Parse error at offset 102 around line 1 column 103: Invalid statement
//? source: ...
#!/usr/bin/env afw

// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = evaluate(script("for (loc i = 0; i < 2; i+=1) { if (i) { try { throw null; } catch (e) { continue; } } 'bad completion'; }"));
assert(completion === undefined);


//? test: cptn-catch-finally-empty-break
//? description: Abrupt completion from finally block calls UpdatEmpty()
//? expect: error:Parse error at offset 110 around line 1 column 111: Invalid statement
//? source: ...
#!/usr/bin/env afw

// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = evaluate(script("for (loc i = 0; i < 2; i+=1) { if (i) { try { throw null; } catch (e) {} finally { break; } } 'bad completion'; }"));
assert(completion === undefined);


//? test: cptn-catch-finally-empty-continue
//? description: Abrupt completion from finally block calls UpdatEmpty()
//? expect: error:Parse error at offset 113 around line 1 column 114: Invalid statement
//? source: ...
#!/usr/bin/env afw

// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = evaluate(script("for (loc i = 0; i < 2; i+=1) { if (i) { try { throw null; } catch (e) {} finally { continue; } } 'bad completion'; }"));
assert(completion === undefined);


//? test: cptn-catch
//? description: Completion value from `catch` clause of a try..catch statement
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('1; try { throw null; } catch (err) { }') === undefined);
assert(evaluate(script('2; try { throw null; } catch (err) { 3; }') === 3);


//? test: cptn-finally-empty-break
//? description: Abrupt completion from finally block calls UpdatEmpty()
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = evaluate(script("for (loc i = 0; i < 2; ++i) { if (i) { try {} finally { break; } } 'bad completion'; }");
assert(completion === undefined);


//? test: cptn-finally-empty-continue
//? description: Abrupt completion from finally block calls UpdatEmpty()
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// Ensure the completion value from the first iteration ('bad completion') is not returned.
loc completion = evaluate(script("for (loc i = 0; i < 2; ++i) { if (i) { try {} finally { continue; } } 'bad completion'; }");
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
  evaluate(script('1; try { throw null; } catch (err) { } finally { }'), undefined
);
assert(
  evaluate(script('2; try { throw null; } catch (err) { 3; } finally { }'), 3
);
assert(
  evaluate(script('4; try { throw null; } catch (err) { } finally { 5; }'), undefined
);
assert(
  evaluate(script('6; try { throw null; } catch (err) { 7; } finally { 8; }'), 7
);


//? test: cptn-finally-skip-catch
//? description:...
    Completion value from `finally` clause of a try..catch..finally statement
    (when `catch` block is not executed)
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('1; try { } catch (err) { } finally { }') === undefined);
assert(evaluate(script('2; try { } catch (err) { 3; } finally { }') === undefined);
assert(evaluate(script('4; try { } catch (err) { } finally { 5; }') === undefined);
assert(evaluate(script('6; try { } catch (err) { 7; } finally { 8; }') === undefined);
assert(evaluate(script('9; try { 10; } catch (err) { } finally { }') === 10);
assert(evaluate(script('11; try { 12; } catch (err) { 13; } finally { }') === 12);
assert(evaluate(script('14; try { 15; } catch (err) { } finally { 16; }') === 15);
assert(evaluate(script('17; try { 18; } catch (err) { 19; } finally { 20; }') === 18);


//? test: cptn-finally-wo-catch
//? description: Completion value from `finally` clause of a try..finally statement
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('1; try { } finally { }') === undefined);
assert(evaluate(script('2; try { 3; } finally { }') === 3);
assert(evaluate(script('4; try { } finally { 5; }') === undefined);
assert(evaluate(script('6; try { 7; } finally { 8; }') === 7);


//? test: cptn-try
//? description: Completion value from `try` clause of a try..catch statement
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('1; try { } catch (err) { }') === undefined);
assert(evaluate(script('2; try { 3; } catch (err) { }') === 3);
assert(evaluate(script('4; try { } catch (err) { 5; }') === undefined);
assert(evaluate(script('6; try { 7; } catch (err) { 8; }') === 7);

//? test: early-catch-duplicates
//? description:...
    It is a Syntax Error if BoundNames of CatchParameter contains any duplicate
    elements.
//? expect: error:Parse error at offset 35 around line 3 column 16: Expecting identifier
//? source: ...
#!/usr/bin/env afw

try { } catch ([x, x]) {}


//? test: early-catch-function
//? description:...
//? expect: error
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
foreach loc x of mycars {
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
foreach loc x of mycars{
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
//? expect: error:Parse error at offset 537 around line 37 column 12: Unknown built-in function someValue
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
//? expect: error:Parse error at offset 674 around line 46 column 12: Unknown built-in function someValue
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
//? expect: error:Parse error at offset 743 around line 49 column 12: Unknown built-in function someValue
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
//? expect: error:Parse error at offset 596 around line 20 column 3: Unknown built-in function with
//? source: ...
#!/usr/bin/env afw


loc myObj = {"p1": 'a',
             "p2": 'b',
             "p3": 'c',
             "value": 'myObj_value',
             "valueOf" : function(){return 'obj_valueOf';},
             "parseInt" : function(){return 'obj_parseInt';},
             "NaN" : 'obj_NaN',
             "Infinity" : 'obj_Infinity',
             "eval"     : function(){return 'obj_eval';},
             "parseFloat" : function(){return 'obj_parseFloat';},
             "isNaN"      : function(){return 'obj_isNaN';},
             "isFinite"   : function(){return 'obj_isFinite';}
};

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
//? expect: error:Parse error at offset 79 around line 6 column 5: Unknown built-in function this
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
//? expect: error:#1: Exception === undefined. Actual: Parameter 1 of function throw can not be undefined
//? source: ...
#!/usr/bin/env afw

// CHECK#1
try{
  throw undefined;
}
catch(e){
  if (e.message!==undefined) throw '#1: Exception === undefined. Actual: '+e.message;
}


//? test: S12.14_A18_T2
//? description: Catching null
//? expect: error:#1: Exception ===null. Actual: Parameter 1 of function throw must evaluate to data type string but evaluated to be null
//? source: ...
#!/usr/bin/env afw

// CHECK#1
try{
  throw null;
}
catch(e){
  if (e.message!==null) throw '#1: Exception ===null. Actual: '+e.message;
}


//? test: S12.14_A18_T3
//? description: Catching boolean
//? expect: error:#1: Exception ===true. Actual:  Exception ===Parameter 1 of function throw must evaluate to data type string but evaluated to be boolean
//? source: ...
#!/usr/bin/env afw

// CHECK#1
try{
  throw true;
}
catch(e){
  if (e.message!==true) throw '#1: Exception ===true. Actual:  Exception ==='+ e.message  ;
}

// CHECK#2
try{
  throw false;
}
catch(e){
  if (e.message!==false) throw '#2: Exception ===false. Actual:  Exception ==='+ e.message  ;
}

// CHECK#3
loc b=false;
try{
  throw b;
}
catch(e){
  if (e.message!==false) throw '#3: Exception ===false. Actual:  Exception ==='+ e.message  ;
}

// CHECK#4
b=true;
try{
  throw b;
}
catch(e){
  if (e.message!==true) throw '#4: Exception ===true. Actual:  Exception ==='+ e.message  ;
}

// CHECK#5
b=true;
try{
  throw b&&false;
}
catch(e){
  if (e.message!==false) throw '#5: Exception ===false. Actual:  Exception ==='+ e.messsage  ;
}

// CHECK#5
b=true;
try{
  throw b||false;
}
catch(e){
  if (e.message!==true) throw '#6: Exception ===true. Actual:  Exception ==='+ e.message  ;
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
//? expect: error:#1: Exception ===13. Actual:  Exception ===Parameter 1 of function throw must evaluate to data type string but evaluated to be integer
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  throw 13;
}
catch(e){
  if (e.message!==13) throw '#1: Exception ===13. Actual:  Exception ==='+ e.message  ;
}

// CHECK#2
try{
  throw 10+3;
}
catch(e){
  if (e.message!==13) throw '#2: Exception ===13. Actual:  Exception ==='+ e.message  ;
}

// CHECK#3
loc b=13;
try{
  throw b;
}
catch(e){
  if (e.message!==13) throw '#3: Exception ===13. Actual:  Exception ==='+ e.message  ;
}

// CHECK#4
loc a=3;
b=10;
try{
  throw a+b;
}
catch(e){
  if (e.message!==13) throw '#4: Exception ===13. Actual:  Exception ==='+ e.message  ;
}

// CHECK#5
try{
  throw 2.13;
}
catch(e){
  if (e.message!==2.13) throw '#5: Exception ===2.13. Actual:  Exception ==='+ e.message  ;
}

// CHECK#6
loc ex=2/3;
try{
  throw 2/3;
}
catch(e){
  if (e.message!==ex) throw '#6: Exception ===2/3. Actual:  Exception ==='+ e.message  ;
}

// CHECK#7
try{
  throw NaN;
}
catch(e){
  assert(isNaN(e.data), "e is NaN");
}

// CHECK#8
try{
  throw +Infinity;
}
catch(e){
  if (e.message!==+Infinity) throw '#8: Exception ===+Infinity. Actual:  Exception ==='+ e.message;
}

// CHECK#9
try{
  throw -Infinity;
}
catch(e){
  if (e.message!==-Infinity) throw '#9: Exception ===-Infinity. Actual:  Exception ==='+ e.message  ;
}

// CHECK#10
try{
  throw +0;
}
catch(e){
  if (e.message!==+0) throw '#10: Exception ===+0. Actual:  Exception ==='+ e.message  ;
}

// CHECK#11
try{
  throw -0;
}
catch(e){
  assert(e.message === -0);
}


//? test: S12.14_A18_T6
//? description: Catching Object
//? expect: error:Parameter 2 of function add<string> can not be undefined
//? source: ...
#!/usr/bin/env afw

loc myObj = {"p1": 'a',
             "p2": 'b',
             "p3": 'c',
             "value": 'myObj_value',
             "valueOf" : function(){return 'obj_valueOf';},
             "parseInt" : function(){return 'obj_parseInt';},
             "NaN" : 'obj_NaN',
             "Infinity" : 'obj_Infinity',
             "eval"     : function(){return 'obj_eval';},
             "parseFloat" : function(){return 'obj_parseFloat';},
             "isNaN"      : function(){return 'obj_isNaN';},
             "isFinite"   : function(){return 'obj_isFinite';},
             "i":7,
};

try{
  throw myObj;
}
catch(e){
// CHECK#1
  if (e.p1!=="a") throw '#1: e.p1==="a". Actual:  e.p1==='+ e.p1 ;
// CHECK#2
  if (e.value!=='myObj_value') throw '#2: e.value===\'myObj_value\'. Actual:  e.value==='+ e.value ;
// CHECK#3
  if (e.evaluate(script()!=='obj_eval')) throw '#3: e.evaluate(script()===\'obj_eval\'. Actual:  e.evaluate(script())==='+ e.evaluate(script()) ;
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
//? expect: error:Parse error at offset 708 around line 40 column 9: Unknown built-in function new
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
  throw mycars;
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
  for (loc i=0;i<3;i+=1){
    if (e[i]!==mycars[i]) throw '#2.'+i+': Exception['+i+']===mycars['+i+']. Actual:  Exception['+i+']==='+ e[i] ;
  }
  for (loc i=3;i<6;i+=1){
    if (e[i]!==mycars2[i-3]) throw '#2.'+i+': Exception['+i+']===mycars2['+i+']. Actual:  Exception['+i+']==='+ e[i] ;
  }
}

// CHECK#3
try{
  throw new Array("Mercedes","Jeep","Suzuki");
}
catch(e){
  for (loc i=0;i<3;i+=1){
    if (e[i]!==mycars2[i]) throw '#3.'+i+': Exception['+i+']===mycars2['+i+']. Actual:  Exception['+i+']==='+ e[i];
  }
}

// CHECK#4
try{
  throw mycars.concat(new Array("Mercedes","Jeep","Suzuki"));
}
catch(e){
  for (loc i=0;i<3;i+=1){
    if (e[i]!==mycars[i]) throw '#4.'+i+': Exception['+i+']===mycars['+i+']. Actual:  Exception['+i+']==='+ e[i] ;
  }
  for (loc i=3;i<6;i+=1){
    if (e[i]!==mycars2[i-3]) throw '#4.'+i+': Exception['+i+']===mycars2['+(i-3)+']. Actual:  Exception['+i+']==='+ e[i];
  }
}

//? test: S12.14_A19_T1
//? description: Testing try/catch syntax construction
//? expect: error:Parse error at offset 45 around line 5 column 10: Unknown built-in function Error
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


//? test: S12.14_A19_T2
//? description: Testing try/catch/finally syntax construction
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc fin=0;
// CHECK#1
try{
  throw "Error: hello";
}
catch(e){
  if (e.message!=="Error: hello") throw '#1.1: Exception.toString()==="Error: hello". Actual: Exception is '+e.message;
}
finally{
  fin=1;
}
if (fin!==1) throw '#1.2: "finally" block must be evaluated';

// CHECK#2
fin=0;
try{
  throw "Error: hello";
}
catch(e){
  if (e.message!=="Error: hello") throw '#2.1: Exception.toString()==="Error: hello". Actual: Exception is '+e.message;
}
finally{
  fin=1;
}
if (fin!==1) throw '#2.2: "finally" block must be evaluated';

// CHECK#3
fin=0;
loc c3=0;
try{
  throw "EvalError: 1";
}
catch(e){
  if (e.message!=="EvalError: 1") throw '#3.1: Exception.toString()==="EvalError: 1". Actual: Exception is '+e.message;
}
finally{
  fin=1;
}
if (fin!==1) throw '#3.2: "finally" block must be evaluated';

// CHECK#4
fin=0;
try{
  throw "RangeError: 1";
}
catch(e){
  if (e.message!=="RangeError: 1") throw '#4.1: Exception.toString()==="RangeError: 1". Actual: Exception is '+e.message;
}
finally{
  fin=1;
}
if (fin!==1) throw '#4.2: "finally" block must be evaluated';

// CHECK#5
fin=0;
try{
  throw "ReferenceError: 1";
}
catch(e){
  if (e.message!=="ReferenceError: 1") throw '#5.1: Exception.toString()==="ReferenceError: 1". Actual: Exception is '+e.message;
}
finally{
  fin=1;
}
if (fin!==1) throw '#5.2: "finally" block must be evaluated';

// CHECK#6
fin=0;
try{
  throw "TypeError: 1";
}
catch(e){
  if (e.message!=="TypeError: 1") throw '#6.1: Exception.toString()==="TypeError: 1". Actual: Exception is '+e.message;
}
finally{
  fin=1;
}
if (fin!==1) throw '#6.2: "finally" block must be evaluated';

// CHECK#7
fin=0;
try{
  throw "URIError: message";
}
catch(e){
  if (e.message!=="URIError: message") throw '#7.1: Exception.toString()==="URIError: message". Actual: Exception is '+e.message;
}
finally{
  fin=1;
}
if (fin!==1) throw '#7.2: "finally" block must be evaluated';


//? test: S12.14_A1
//? description:...
    Executing TryStatement : try Block Catch. The statements doesn't
    cause actual exceptions
//? expect: null
//? source: ...
#!/usr/bin/env afw

// CHECK#1
try {
  loc x=0;
}
catch (e) {
  throw '#1: If Result(1).type is not throw, return Result(1). Actual: 4 Return(Result(3))';
}

// CHECK#2
loc c1=0;
loc x1;
try{
  x1=1;
}
finally
{
  c1=1;
}
if(x1!==1){
  throw '#2.1: "try" block must be evaluated. Actual: try Block has not been evaluated';
}
if (c1!==1){
  throw '#2.2: "finally" block must be evaluated. Actual: finally Block has not been evaluated';
}

// CHECK#3
loc c2=0;
loc x2;
try{
  x2=1;
}
catch(e){
  throw '#3.1: If Result(1).type is not throw, return Result(1). Actual: 4 Return(Result(3))';
}
finally{
  c2=1;
}
if(x2!==1){
  throw '#3.2: "try" block must be evaluated. Actual: try Block has not been evaluated';
}
if (c2!==1){
  throw '#3.3: "finally" block must be evaluated. Actual: finally Block has not been evaluated';
}


//? test: S12.14_A2
//? description:...
    Checking if execution of "catch" catches an exception thrown with
    "throw"
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try {
  throw "catchme";
  throw '#1: throw "catchme" lead to throwing exception';
}
catch(e){}

// CHECK#2
loc c2=0;
try{
  try{
    throw "exc";
    throw '#2.1: throw "exc" lead to throwing exception';
  }finally{
    c2=1;
  }
}
catch(e){
  if (c2!==1){
    throw '#2.2: "finally" block must be evaluated';
  }
}

// CHECK#3
loc c3=0;
loc x3;
try{
  throw "exc";
  throw '#3.1: throw "exc" lead to throwing exception';
}
catch(err){
  x3=1;
}
finally{
  c3=1;
}
if (x3!==1){
  throw '#3.2: "catch" block must be evaluated';
}
if (c3!==1){
  throw '#3.3: "finally" block must be evaluated';
}


//? test: S12.14_A3
//? description: Checking if execution of "catch" catches system exceptions
//? expect: error:Parse error at offset 373 around line 31 column 6: Unknown built-in function someValue
//? source: ...
#!/usr/bin/env afw

// CHECK#1
try{
  loc y;
  throw '#1: "y" lead to throwing exception';
}
catch(e){}

// CHECK#2
loc c2=0;
try{
  try{
    loc someValue;
    throw '#3.1: "someValues" lead to throwing exception';
  }
  finally{
    c2=1;
  }
}
catch(e){
  if (c2!==1){
    throw '#3.2: "finally" block must be evaluated';
  }
}

// CHECK#3
loc c3=0;
loc x3=0;
try{
  x3=someValue;
  throw '#3.1: "x3=someValues" lead to throwing exception';
}
catch(err){
  x3=1;
}
finally{
  c3=1;
}
if (x3!==1){
  throw '#3.2: "catch" block must be evaluated';
}
if (c3!==1){
  throw '#3.3: "finally" block must be evaluated';
}


//? test: S12.14_A4
//? description: Checking if deleting an exception fails
//? expect: error:Parse error at offset 136 around line 9 column 7: Unknown built-in function delete
//? source: ...
#!/usr/bin/env afw

// CHECK#1
try {
  throw "catchme";
  throw '#1.1: throw "catchme" lead to throwing exception';
}
catch (e) {
  if (delete e){
    throw '#1.2: Exception has DontDelete property';
  }
  if (e!=="catchme") {
    throw '#1.3: Exception === "catchme". Actual:  Exception ==='+ e  ;
  }
}

// CHECK#2
try {
  throw "catchme";
  throw '#2.1: throw "catchme" lead to throwing exception';
}
catch(e){}
try{
  e;
  throw '#2.2: Deleting catching exception after ending "catch" block';
}
catch(err){}


//? test: S12.14_A5
//? description: Checking "catch" catches the Identifier in appropriate way
//? expect: null
//? source: ...
#!/usr/bin/env afw

// CHECK#1
try {
  throw "catchme";
  throw "dontcatchme";
  throw '#1.1: throw "catchme" lead to throwing exception';
}
catch (e) {
  if(e.message==="dontcatchme"){
    throw '#1.2: Exception !== "dontcatchme"';
  }
  if (e.message!=="catchme") {
    throw '#1.3: Exception === "catchme". Actual:  Exception ==='+ e  ;
  }
}

// CHECK#2
function SwitchTest1(value){
  loc result = 0;
  try{
    // fixme no switch
    switch(value) {
      case 1:
        result += 4;
        throw result;
        break;
      case 4:
        result += 64;
        throw "ex";
    }
  return result;
  }
  catch(e){
    if ((value===1)&&(e!==4)) throw '#2.1: Exception === 4. Actual: '+e;
    if ((value===4)&&(e!=="ex"))throw '#2.2: Exception === "ex". Actual: '+e;
  }
  finally{
    return result;
  }
}
if (SwitchTest1(1)!==4) throw '#2.3: "finally" block must be evaluated';
if (SwitchTest1(4)!==64)throw '#2.4: "finally" block must be evaluated';



//? test: S12.14_A6
//? description:...
    Executing sequence of "try" statements, using counters with
    varying values within
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// CHECK#1
loc c1=0;
try {
  c1+=1;
  y;
  throw '#1.1: "y" lead to throwing exception';
}
catch (e) {
  c1*=2;
}
if (c1!==2){
  throw '#1.2: Sequence evaluation of commands try/catch is 1. try, 2. catch';
}

// CHECK#2
loc c2=0;
try{
  c2+=1;
}
finally{
  c2*=2;
}
if (c2!==2){
  throw '#2: Sequence evaluation of commands try/finally is 1. try, 2. finally';
}

// CHECK#3
loc c3=0;
try{
  c3=1;
  z;
}
catch(err){
  c3*=2;
}
finally{
  c3+=1;
}
if (c3!==3){
  throw '#3: Sequence evaluation of commands try/catch/finally(with exception) is 1. try, 2. catch, 3. finally';
}

// CHECK#4
loc c4=0;
try{
  c4=1;
}
catch(err){
  c4*=3;
}
finally{
  c4+=1;
}
if (c4!==2){
  throw '#4: Sequence evaluation of commands try/catch/finally(without exception) is 1. try, 2. finally';
}


//? test: S12.14_A7_T1
//? description:...
    Checking if the production of nested TryStatement statements
    evaluates correct
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  try{
    throw "ex2";
  }
  catch(er2){
    if (er2.message!=="ex2")
      throw '#1.1: Exception === "ex2". Actual:  Exception ==='+ er2.message  ;
      throw "ex1";
    }
  }
  catch(er1){
    if (er1.message!=="ex1") throw '#1.2: Exception === "ex1". Actual: '+er1.message;
    if (er1.message==="ex2") throw '#1.3: Exception !== "ex2". Actual: catch previous embedded exception';
}

// CHECK#2
try{
  throw "ex1";
}
catch(er1){
  try{
    throw "ex2";
  }
  catch(er1){
    if (er1.message==="ex1") throw '#2.1: Exception !== "ex1". Actual: catch previous catching exception';
    if (er1.message!=="ex2") throw '#2.2: Exception === "ex2". Actual:  Exception ==='+ er1.message  ;
  }
  if (er1.message!=="ex1") throw '#2.3: Exception === "ex1". Actual:  Exception ==='+ er1.message  ;
  if (er1.message==="ex2") throw '#2.4: Exception !== "ex2". Actual: catch previous catching exception';
}

// CHECK#3
try{
  throw "ex1";
}
catch(er1){
  if (er1.message!=="ex1") throw '#3.1: Exception ==="ex1". Actual:  Exception ==='+ er1.message  ;
}
finally{
  try{
    throw "ex2";
  }
  catch(er1){
    if (er1.message==="ex1") throw '#3.2: Exception !=="ex1". Actual: catch previous embedded exception';
    if (er1.message!=="ex2") throw '#3.3: Exception ==="ex2". Actual:  Exception ==='+ er1.message  ;
  }
}

// CHECK#4
loc c4=0;
try{
  throw "ex1";
}
catch(er1){
  try{
    throw "ex2";
  }
  catch(er1){
    if (er1.message==="ex1") throw '#4.1: Exception !=="ex1". Actual: catch previous catching exception';
    if (er1.message!=="ex2") throw '#4.2: Exception ==="ex2". Actual:  Exception ==='+ er1.message  ;
  }
  if (er1.message!=="ex1") throw '#4.3: Exception ==="ex1". Actual:  Exception ==='+ er1.message  ;
  if (er1.message==="ex2") throw '#4.4: Exception !=="ex2". Actual: Catch previous embedded exception';
}
finally{
  c4=1;
}
if (c4!==1) throw '#4.5: "finally" block must be evaluated';

// CHECK#5
loc c5=0;
try{
  try{
    throw "ex2";
  }
  catch(er1){
    if (er1.message!=="ex2") throw '#5.1: Exception ==="ex2". Actual:  Exception ==='+ er1.message  ;
  }
  throw "ex1";
}
catch(er1){
  if (er1.message!=="ex1") throw '#5.2: Exception ==="ex1". Actual:  Exception ==='+ er1.message  ;
  if (er1.message==="ex2") throw '#5.3: Exception !=="ex2". Actual: catch previous embedded exception';
}
finally{
  c5=1;
}
if (c5!==1) throw '#5.4: "finally" block must be evaluated';

// CHECK#6
loc c6=0;
try{
  try{
    throw "ex1";
  }
  catch(er1){
    if (er1.message!=="ex1") throw '#6.1: Exception ==="ex1". Actual:  Exception ==='+ er1.message  ;
  }
}
finally{
  c6=1;
}
if (c6!==1) throw '#6.2: "finally" block must be evaluated';

// CHECK#7
loc c7=0;
try{
  try{
    throw "ex1";
  }
  finally{
    try{
      c7=1;
      throw "ex2";
    }
    catch(er1){
      if (er1.message!=="ex2") throw '#7.1: Exception ==="ex2". Actual:  Exception ==='+ er1.message  ;
      if (er1.message==="ex1") throw '#7.2: Exception !=="ex1". Actual: catch previous embedded exception';
      c7+=1;
    }
  }
}
catch(er1){
  if (er1.message!=="ex1") throw '#7.3: Exception ==="ex1". Actual:  Exception ==='+ er1.message  ;
}
if (c7!==2) throw '#7.4: "finally" block must be evaluated';


//? test: S12.14_A7_T2
//? description:...
    Checking if the production of nested TryStatement statements
    evaluates correct
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  try{
    throw "ex2";
  }
  finally{
    throw "ex1";
  }
}
catch(er1){
  if (er1.message!=="ex1") throw '#1.2: Exception === "ex1". Actual:  Exception ==='+er1.message ;
  if (er1.message==="ex2") throw '#1.3: Exception !== "ex2". Actual: catch previous embedded exception';
}

// CHECK#2
try{
  try{
    throw "ex1";
  }
  catch(er1){
    if (er1.message!=="ex1") throw '#2.1: Exception === "ex1". Actual:  Exception ==='+er1.message ;
    try{
      throw "ex2";
    }
    finally{
      throw "ex3";
    }
    throw '#2.2: throw "ex1" lead to throwing exception';
  }
}
catch(er1){
  if (er1.message!=="ex3") throw '#2.3: Exception === "ex3". Actual:  Exception ==='+er1.message ;
}

// CHECK#3
try{
  try{
    throw "ex1";
  }
  catch(er1){
    if (er1.message!=="ex1") throw '#3.1: Exception === "ex1". Actual:  Exception ==='+er1.message ;
  }
  finally{
    try{
      throw "ex2";
    }
    finally{
      throw "ex3";
    }
  }
}
catch(er1){
  if (er1.message!=="ex3") throw '#3.2: Exception === "ex3". Actual:  Exception ==='+er1.message ;
}

// CHECK#4
loc c4=0;
try{
  try{
    throw "ex1";
  }
  catch(er1){
    if (er1.message!=="ex1") throw '#4.1: Exception === "ex1". Actual:  Exception ==='+er1.message ;
    try{
      throw "ex2";
    }
    finally{
      throw "ex3";
    }
  }
  finally{
    c4=1;
  }
}
catch(er1){
  if (er1.message!=="ex3") throw '#4.2: Exception === "ex3". Actual:  Exception ==='+er1.message ;
}
if (c4!==1) throw '#4.3: "finally" block must be evaluated';

// CHECK#5
loc c5=0;
try{
  try{
    throw "ex2";
  }
  finally{
    throw "ex3";
  }
  throw "ex1";
}
catch(er1){
  if (er1.message!=="ex3") throw '#5.1: Exception === "ex3". Actual:  Exception ==='+er1.message ;
  if (er1.message==="ex2") throw '#5.2: Exception !== "ex2". Actual: catch previous embedded exception';
  if (er1.message==="ex1") throw '#5.3: Exception !=="ex1". Actual: catch previous embedded exception';
}
finally{
  c5=1;
}
if (c5!==1) throw '#5.4: "finally" block must be evaluated';

// CHECK#6
loc c6=0;
try{
  try{
    try{
      throw "ex1";
    }
    finally{
      throw "ex2";
    }
  }
  finally{
    c6=1;
  }
}
catch(er1){
  if (er1.message!=="ex2") throw '#6.1: Exception === "ex2". Actual:  Exception ==='+er1.message ;
}
if (c6!==1) throw '#6.2: "finally" block must be evaluated';

// CHECK#7
loc c7=0;
try{
  try{
    throw "ex1";
  }
  finally{
    try{
      c7=1;
      throw "ex2";
    }
    finally{
      c7+=1;
      throw "ex3";
    }
  }
}
catch(er1){
  if (er1.message!=="ex3") throw '#7.1: Exception === "ex3". Actual:  Exception ==='+er1.message ;
}
if (c7!==2) throw '#7.2: Embedded "try/finally" blocks must be evaluated';


//? test: S12.14_A7_T3
//? description:...
    Checking if the production of nested TryStatement statements
    evaluates correct
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  try{
    throw "ex2";
  }
  catch(er2){
    if (er2.message!=="ex2") throw '#1.1: Exception === "ex2". Actual:  Exception ==='+er2.message;
    throw "ex1";
  }
  finally{
    throw "ex3";
  }
}
catch(er1){
  if (er1.message!=="ex3") throw '#1.2: Exception === "ex3". Actual:  Exception ==='+er1.message;
  if (er1.message==="ex2") throw '#1.3: Exception !=="ex2". Actual: catch previous catched exception';
  if (er1.message==="ex1") throw '#1.4: Exception !=="ex1". Actual: catch previous embedded exception';
}

// CHECK#2
loc c2=0;
try{
  throw "ex1";
}
catch(er1){
  try{
    throw "ex2";
  }
  catch(er1){
    if (er1.message==="ex1") throw '#2.1: Exception !=="ex1". Actual: catch previous catched exception';
    if (er1.message!=="ex2") throw '#2.2: Exception === "ex2". Actual:  Exception ==='+er1.message;
  }
  finally{
    c2=1;
  }
  if (er1.message!=="ex1") throw '#2.3: Exception === "ex1". Actual:  Exception ==='+er1.message;
  if (er1.message==="ex2") throw '#2.4: Exception !== "ex2". Actual: catch previous embedded exception';
}
if (c2!==1)	throw '#2.5: "finally" block must be evaluated';

// CHECK#3
loc c3=0;
try{
  throw "ex1";
}
catch(er1){
  if (er1.message!=="ex1") throw '#3.1: Exception === "ex1". Actual:  Exception ==='+er1.message;
}
finally{
  try{
    throw "ex2";
  }
  catch(er1){
    if (er1.message==="ex1") throw '#3.2: Exception !=="ex1". Actual: catch previous catched exception';
    if (er1.message!=="ex2") throw '#3.3: Exception === "ex2". Actual:  Exception ==='+er1.message;
  }
  finally{
    c3=1;
  }
}
if (c3!==1)	throw '#3.4: "finally" block must be evaluated';

// CHECK#4
loc c4=0;
try{
  try{
    throw "ex1";
  }
  catch(er1){
    try{
      throw "ex2";
    }
    catch(er1){
      if (er1.message==="ex1") throw '#4.1: Exception !=="ex2". Actual: catch previous catched exception';
      if (er1.message!=="ex2") throw '#4.2: Exception === "ex2". Actual:  Exception ==='+er1.message;
    }
    finally{
      c4=2;
      throw "ex3";
    }
    if (er1.message!=="ex1") throw '#4.3: Exception === "ex2". Actual:  Exception ==='+er1.message;
    if (er1.message==="ex2") throw '#4.4: Exception !=="ex2". Actual: catch previous catched exception';
    if (er1.message==="ex3") throw '#4.5: Exception !=="ex3". Actual: Catch previous embedded exception';
  }
  finally{
    c4*=2;
  }
}
catch(er1){}
if (c4!==4) throw '#4.6: "finally" block must be evaluated';

// CHECK#5
loc c5=0;
try{
  try{
    throw "ex2";
  }
  catch(er1){
    if (er1.message!=="ex2") throw '#5.1: Exception === "ex2". Actual:  Exception ==='+er1.message;
  }
  finally{
    throw "ex3";
  }
  throw "ex1";
}
catch(er1){
  if (er1.message!=="ex3") throw '#5.2: Exception === "ex3". Actual:  Exception ==='+er1.message;
  if (er1.message==="ex2") throw '#5.3: Exception !=="ex2". Actual: catch previous catched exception';
  if (er1.message==="ex1") throw '#5.4: Exception !=="ex1". Actual: catch previous embedded exception';
}
finally{
  c5=1;
}
if (c5!==1) throw '#5.5: "finally" block must be evaluated';

// CHECK#6
loc c6=0;
try{
  try{
    throw "ex1";
  }
  catch(er1){
    if (er1.message!=="ex1") throw '#6.1: Exception === "ex1". Actual:  Exception ==='+er1.message;
  }
  finally{
    c6=2;
  }
}
finally{
  c6*=2;
}
if (c6!==4) throw '#6.2: "finally" block must be evaluated';

// CHECK#7
loc c7=0;
try{
  try{
    throw "ex1";
  }
  finally{
    try{
      c7=1;
      throw "ex2";
    }
    catch(er1){
      if (er1.message!=="ex2") throw '#7.1: Exception === "ex2". Actual:  Exception ==='+er1.message;
      if (er1.message==="ex1") throw '#7.2: Exception !=="ex2". Actual: catch previous catched exception';
      c7+=1;
    }
    finally{
      c7*=2;
    }
  }
}
catch(er1){
  if (er1.message!=="ex1") throw '#7.3: Exception === "ex1". Actual:  Exception ==='+er1.message;
}
if (c7!==4) throw '#7.4: "finally" block must be evaluated';


//? test: S12.14_A8
//? description: Throwing exception within an "if" statement
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=1;
try{
  if(c1===1){
    throw "ex1";
    throw '#1.1: throw "ex1" lead to throwing exception';
  }
  throw '#1.2: throw "ex1" inside the "if" statement lead to throwing exception';
}
catch(er1){
  if (er1.message!=="ex1") throw '#1.3: Exception ==="ex1". Actual:  Exception ==='+er1.message;
}

// CHECK#2
loc c2=1;
if(c2===1){
  try{
    throw "ex1";
    throw '#2.1: throw "ex1" lead to throwing exception';
  }
  catch(er1){
    if(er1.message!="ex1") throw '#2.2: Exception ==="ex1". Actual:  Exception ==='+er1.message;
  }
}


//? test: S12.14_A9_T1
//? description: Loop within a "try" Block, from where exception is thrown
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc i=0;
try{
  do{
    if(i===5) throw string(i);
    i+=1;
  }
  while(i<10);
}
catch(e){
  if(e.message!=="5")throw '#1: Exception ===5. Actual:  Exception ==='+ e.message  ;
}

//? test: S12.14_A9_T2
//? description:...
    "try" statement within a loop, the statement contains "continue"
    statement
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=0;
loc fin=0;
do{
  try{
    c1+=1;
    continue;
  }
  catch(er1){}
  finally{
    fin=1;
  }
  fin=-1;
}
while(c1<2);
if(fin!==1){
  throw '#1: "finally" block must be evaluated at "try{continue} catch finally" construction';
}

// CHECK#2
loc c2=0;
loc fin2=0;
do{
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
while(c2<2);
if(fin2!==1){
  throw '#2: "finally" block must be evaluated at "try catch{continue} finally" construction';
}

// CHECK#3
loc c3=0;
loc fin3=0;
do{
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
while(c3<2);
if(fin3!==1){
  throw '#3: "finally" block must be evaluated at "try catch finally{continue}" construction';
}

// CHECK#4
loc c4=0;
loc fin4=0;
do{
  try{
    c4+=1;
    continue;
  }
  finally{
    fin4=1;
  }
  fin4=-1;
}
while(c4<2);
if(fin4!==1){
  throw '#4: "finally" block must be evaluated at "try{continue} finally"  construction';
}

// CHECK#5
loc c5=0;
do{
  try{
    throw "ex1";
  }
  catch(er1){
    c5+=1;
    continue;
  }
}
while(c5<2);
if(c5!==2){
  throw '#5: "try catch{continue}" must work correctly';
}

// CHECK#6
loc c6=0;
loc fin6=0;
do{
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
while(c6<2);
if(fin6!==1){
  throw '#6.1: "finally" block must be evaluated';
}
if(c6!==2){
  throw '#6.2: "try finally{continue}" must work correctly';
}


//? test: S12.14_A9_T3
//? description:...
    "try" statement within a loop, the statement contains "break"
    statement
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=0;
loc fin=0;
do{
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
while(c1<2);
if(fin!==1){
  throw '#1.1: "finally" block must be evaluated';
}
if(c1!==1){
  throw '#1.2: "try{break}catch finally" must work correctly';
}

// CHECK#2
loc c2=0;
loc fin2=0;
do{
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
while(c2<2);
if(fin2!==1){
  throw '#2.1: "finally" block must be evaluated';
}
if(c2!==1){
  throw '#2.2: "try catch{break} finally" must work correctly';
}

// CHECK#3
loc c3=0;
loc fin3=0;
do{
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
while(c3<2);
if(fin3!==1){
  throw '#3.1: "finally" block must be evaluated';
}
if(c3!==1){
  throw '#3.2: "try catch finally{break}" must work correctly';
}

// CHECK#4
loc c4=0;
loc fin4=0;
do{
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
while(c4<2);
if(fin4!==1){
  throw '#4.1: "finally" block must be evaluated';
}
if(c4!==1){
  throw '#4.2: "try{break} finally" must work correctly';
}

// CHECK#5
loc c5=0;
do{
  try{
    throw "ex1";
  }
  catch(er1){
    break;
  }
}
while(c5<2);
if(c5!==0){
  throw '#5: "try catch{break}" must work correctly';
}

// CHECK#6
loc c6=0;
do{
  try{
    c6+=1;
    break;
  }
  catch(er1){}
  c6+=2;
}
while(c6<2);
if(c6!==1){
  throw '#6: "try{break} catch" must work correctly';
}

// CHECK#7
loc c7=0;
loc fin7=0;
try{
  do{
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
  while(c7<2);
}
catch(ex1){
  c7=10;
}
if(fin7!==1){
  throw '#7.1: "finally" block must be evaluated';
}
if(c7!==1){
  throw '#7.2: try finally{break} error';
}


//? test: S12.14_A9_T4
//? description:...
    "try" statement within a loop, the statement contains "continue"
    and "break" statements
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c1=0;
loc fin=0;
do{
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
while(c1<2);
if(fin!==1){
  throw '#1.1: "finally" block must be evaluated';
}
if(c1!==2){
  throw '#1.2: "try{break} catch finally{continue}" must work correctly';
}

// CHECK#2
loc c2=0;
loc fin2=0;
do{
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
while(c2<2);
if(fin2!==1){
  throw '#2.1: "finally" block must be evaluated';
}
if(c2!==2){
  throw '#2.2: "try catch{break} finally{continue}" must work correctly';
}


//? test: S12.14_A9_T5
//? description:...
    Checking if exceptions are thrown correctly from wherever of loop
    body
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc c=0;
loc i=0;
loc fin=0;
do{
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
while(i<10);
if(fin!==10){
  throw '#1.4: "finally" block must be evaluated';
}


//? test: scope-catch-block-lex-close
//? description: Removal of lexical environment for `catch` block
//? expect: error:Assertion failed
//? source: ...
#!/usr/bin/env afw

loc probe;
loc x;

try {
  throw "null";
} catch (_) {
  loc x = 'inside';
  probe = function() { return x; };
}
x = 'outside';

assert(x === 'outside');
// fixme closure causes x to be 'outside' instead of 'inside'
assert(probe() === 'inside');


//? test: scope-catch-block-lex-open
//? description: Creation of new lexical environment for `catch` block
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

loc probeParam;
loc probeBlock;
loc x = 'outside';

try {
  throw "" [];
  // fixme decide if we should allow destructuring on catch
} catch ({ "data": [_ = probeParam = function() { return x; }] }) {
  probeBlock = function() { return x; };
  let x = 'inside';
}

assert(probeParam() === 'outside');
assert(probeBlock() === 'inside');


//? test: scope-catch-block-var-none
//? description: Retainment of existing variable environment for `catch` block
//? expect: error:Assertion failed: reference preceding statement
//? source: ...
#!/usr/bin/env afw

loc x = 1;
loc probeBefore = function() { return x; };
loc probeInside;

try {
  throw "null";
} catch (_) {
  loc x = 2;
  probeInside = function() { return x; };
}

// fixme closure causes x to be 1 instead of 2
assert(probeBefore() === 2, 'reference preceding statement');
assert(probeInside() === 2, 'reference within statement');
assert(x === 2, 'reference following statement');



//? test: scope-catch-param-lex-close
//? description: Removal of lexical environment for `catch` parameter
//? expect: error:Assertion failed
//? source: ...
#!/usr/bin/env afw

loc probe;
loc x;

try {
  throw 'inside';
} catch (x) {
  probe = function() { return x; };
}
x = 'outside';

assert(x === 'outside');
// fixme closure causes x to be 'outside' instead of 'inside'
assert(probe() === 'inside');


//? test: scope-catch-param-lex-open
//? description: Creation of new lexical environment for `catch` parameter
//? expect: error:Parse error at offset 58 around line 3 column 39: Unknown built-in function x
//? source: ...
#!/usr/bin/env afw

loc probeBefore = function() { return x; };
loc probeTry;
loc probeParam;
loc x = 'outside';

try {
  probeTry = function() { return x; };

  throw ['inside'];
  // fixme can't do this, and destructuring may not be allowed on catch
} catch ([x, _ = probeParam = function() { return x; }]) {}

assert(probeBefore() === 'outside');
assert(probeTry() === 'outside');
assert(probeParam() === 'inside');

//? test: scope-catch-param-var-none
//? description: Retainment of existing variable environment for `catch` parameter
//? expect: error:Parse error at offset 253 around line 15 column 10: Expecting identifier
//? source: ...
#!/usr/bin/env afw

loc x = 1;
loc probeBefore = function() { return x; };
loc probeTry;
loc probeParam;
loc probeBlock;

try {
  loc x = 2;
  probeTry = function() { return x; };
  // can't throw array
  throw [];
  // can't destructure catch
} catch ([_ = (evaluate(script('loc x = 3;'), probeParam = function() { return x; })]) {
  loc x = 4;
  probeBlock = function() { return x; };
}

assert(probeBefore() === 4 === 'reference preceding statement');
assert(probeTry() === 4 === 'reference from `try` block');
assert(probeParam() === 4 === 'reference within CatchParameter');
assert(probeBlock() === 4 === 'reference from `catch` block');
assert(x === 4 === 'reference following statement');


//? test: static-init-await-binding-invalid
//? description: BindingIdentifier may not be `await` within class static blocks
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function class
//? source: ...
#!/usr/bin/env afw

class C {
  static {
    try {} catch (await) {}
  }
}


//? test: static-init-await-binding-valid
//? description: The `await` keyword is interpreted as an identifier within arrow function bodies
//? expect: error:Parse error at offset 20 around line 3 column 1: Unknown built-in function class
//? source: ...
#!/usr/bin/env afw

class C {
  static {
    (() => { try {} catch (await) {} });
  }
}


//? test: tco-catch-finally
//? description: Statement within statement is a candidate for tail-call optimization.
//? expect: error:Parse error at offset 83 around line 5 column 11: Expecting '('
//? source: ...
#!/usr/bin/env afw

loc callCount = 0;
// fixme can't call like this yet
(function f(n) {
  if (n === 0) {
    callCount += 1
    return;
  }
  try { } catch (err) { } finally {
    return f(n - 1);
  }
}(100000));
assert(callCount === 1);


//? test: tco-catch
//? description: Statement within statement is a candidate for tail-call optimization.
//? expect: error:Parse error at offset 83 around line 5 column 11: Expecting '('
//? source: ...
#!/usr/bin/env afw

loc callCount = 0;
// fixme can't call like this yet
(function f(n) {
  if (n === 0) {
    callCount += 1
    return;
  }
  try {
    throw null;
  } catch (err) {
    return f(n - 1);
  }
}(100000));
assert(callCount === 1);


//? test: tco-finally
//? description: Statement within statement is a candidate for tail-call optimization.
//? expect: error:Parse error at offset 83 around line 5 column 11: Expecting '('
//? source: ...
#!/usr/bin/env afw

loc callCount = 0;
// fixme can't call like this yet
(function f(n) {
  if (n === 0) {
    callCount += 1
    return;
  }
  try { } finally {
    return f(n - 1);
  }
}(100000));
assert(callCount === 1);