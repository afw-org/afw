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