#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: leave.as
//? customPurpose: Part of test262
//? description: block-scope/leave
//? sourceType: script
//?
//? test: finally-block-let-declaration-only-shadows-outer-parameter-value-1
//? description: finally block let declaration only shadows outer parameter value 1
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

/*
We need try/catch/throw for this
try {
  (function(x) {
    try {
      let x = 'inner';
      throw 0;
    } finally {
      assert.sameValue(x, 'outer');
    }
  })('outer');
} catch (e) {}
*/


//?
//? test: finally-block-let-declaration-only-shadows-outer-parameter-value-2
//? description: finally block let declaration only shadows outer parameter value 2
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

/*
We need try/catch/throw for this
(function(x) {
  try {
    let x = 'middle';
    {
      let x = 'inner';
      throw 0;
    }
  } catch(e) {

  } finally {
    assert.sameValue(x, 'outer');
  }
})('outer');
*/


//?
//? test: for-loop-block-let-declaration-only-shadows-outer-parameter-value-1
//? description: for loop block let declaration only shadows outer parameter value 1
//? expect: null
//? source: ...
#!/usr/bin/env afw

function(x) {
    for (loc i = 0; i < 10; i = i + 1) {
        loc x = 'inner' + string(i);
        continue;
    }
    assert(x === 'outer');
}('outer');


//?
//? test: outermost-binding-updated-in-catch-block-nested-block-let-declaration-unseen-outside-of-block
//? description: outermost binding updated in catch block; nested block let declaration unseen outside of block
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

/*
We need try/catch/throw for this

var caught = false;
try {
  {
    let xx = 18;
    throw 25;
  }
} catch (e) {
  caught = true;
  assert.sameValue(e, 25);
  (function () {
    try {
      // NOTE: This checks that the block scope containing xx has been
      // removed from the context chain.
      assert.sameValue(xx, undefined);
      eval('xx');
      assert(false);  // should not reach here
    } catch (e2) {
      assert(e2 instanceof ReferenceError);
    }
  })();
}
assert(caught);
*/


//?
//? test: try-block-let-declaration-only-shadows-outer-parameter-value-1
//? description: try block let declaration only shadows outer parameter value 1
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

/*
We need try/catch/throw for this

(function(x) {
  try {
    let x = 'inner';
    throw 0;
  } catch (e) {
    assert.sameValue(x, 'outer');
  }
})('outer');
*/

//?
//? test: try-block-let-declaration-only-shadows-outer-parameter-value-2
//? description: try block let declaration only shadows outer parameter value 2
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

/*
We need try/catch/throw for this

(function(x) {
  try {
    let x = 'middle';
    {
      let x = 'inner';
      throw 0;
    }
  } catch (e) {
    assert.sameValue(x, 'outer');
  }
})('outer');
*/


//?
//? test: verify-context-in-finally-block
//? description: verify context in finally block 1
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

/*
We need try/catch/throw for this

function f() {}

(function(x) {
  try {
    let x = 'inner';
    throw 0;
  } catch(e) {

  } finally {
    f();
    assert.sameValue(x, 'outer');
  }
})('outer');
*/

//?
//? test: verify-context-in-for-loop-block
//? description: verify context in for loop block 2
//? expect: null
//? source: ...
#!/usr/bin/env afw

function f() {}

function(x) {
    for (loc i = 0; i < 10; i = i + 1) {
        loc x = "inner" + string(i);
        continue;
    }    
    f();
    assert(x === "outer");
}("outer");

//?
//? test: verify-context-in-for-loop-block
//? description: verify context in try block 1
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

/*
We need try/catch/throw for this

function f() {}

(function(x) {
  try {
    let x = 'inner';
    throw 0;
  } catch (e) {
    f();
    assert.sameValue(x, 'outer');
  }
})('outer');
*/


//?
//? test: x-before-continue
//? description: x before continue
//? expect: null
//? source: ...
#!/usr/bin/env afw

do {
    loc x = 4;
    assert(x === 4);
    {
        loc x = 5;
        assert(x === 5);
        continue;
        assert(false);
    }
} while (false);