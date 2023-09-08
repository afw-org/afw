#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: shadowing.as
//? customPurpose: Part of test262
//? description: block-scope/shadowing
//? sourceType: script
//?
//? test: catch-parameter-shadowing-catch-parameter
//? description: catch parameter shadowing catch parameter
//? expect: undefined
//? source: ...

function fn() {
  let c = 1;
  try {
    throw 'stuff3';
  } catch (c) {
    try {
      throw 'stuff4';
    } catch(c) {
      assert(c.message === 'stuff4');
      // catch parameter shadowing catch parameter
      c = 3;
      assert(c === 3);
    }
    assert(c.message === 'stuff3');
  }
  assert(c === 1);
}
fn(1);


//? test: catch-parameter-shadowing-function-parameter-name
//? description: catch parameter shadowing function parameter name
//? expect: undefined
//? source: ...

function fn(a) {
  try {
    throw 'stuff1';
  } catch (a) {
    assert(a.message === 'stuff1');
    // catch parameter shadowing function parameter name
    a = 2;
    assert(a === 2);
  }
}
fn(1);


//? test: catch-parameter-shadowing-let-declaration
//? description: catch parameter shadowing let declaration
//? expect: undefined
//? source: ...

{
  let a = 3;
  try {
    throw 'stuff2';
  } catch (a) {
    assert(a.message === 'stuff2');
    // catch parameter shadowing let declaration
    a = 4;
    assert(a === 4);
  }
  assert(a === 3);
}


//? test: catch-parameter-shadowing-var-variable
//? description: catch parameter shadowing var variable
//? expect: undefined
//? source: ...

function fn() {
  let a = 1;
  try {
    throw 'stuff3';
  } catch (a) {
    // catch parameter shadowing var variable
    assert(a.message === 'stuff3');
  }
  assert(a === 1);
}
fn();


//? test: const-declaration-shadowing-catch-parameter
//? description: const declaration shadowing catch parameter
//? expect: undefined
//? source: ...

function fn() {
  let a = 1;
  try {
    throw 'stuff3';
  } catch (a) {
    {
      // const declaration shadowing catch parameter
      const a = 3;
      assert(a === 3);
    }
    assert(a.message === 'stuff3');
  }
  assert(a === 1);
}
fn();


//? test: const-declarations-shadowing-parameter-name-let-const-and-var-variables
//? description: const declarations shadowing parameter name, let, const and var variables
//? expect: undefined
//? source: ...

function fn(a) {
  let b = 1;
  //var c = 1;
  const d = 1;
  {
    const a = 2;
    const b = 2;
    //const c = 2;
    const d = 2;
    assert(a === 2);
    assert(b === 2);
    //assert.sameValue(c, 2);
    assert(d === 2);
  }

  assert(a === 1);
  assert(b === 1);
  //assert.sameValue(c, 1);
  assert(d === 1);
}
fn(1);

//? test: dynamic-lookup-from-closure
//? description: dynamic lookup from closure
//? expect: undefined
//? source: ...

function fn(one) {
  //var x = one + 1;
  let y = one + 2;
  const u = one + 4;
  {
    let z = one + 3;
    const v = one + 5;
    function f() {
      assert(one === 1);
      //assert.sameValue(x, 2);
      assert(y === 3);
      assert(z === 4);
      assert(u === 5);
      assert(v === 6);
    }

    f();
  }
}
fn(1);

//? test: dynamic-lookup-in-and-through-block-contexts
//? description: dynamic lookup in and through block contexts
//? expect: undefined
//? source: ...

function fn(one) {
  //var x = one + 1;
  let y = one + 2;
  const u = one + 4;
  {
    let z = one + 3;
    const v = one + 5;
    assert(one === 1);
    //assert.sameValue(x, 2);
    assert(y === 3);
    assert(z === 4);
    assert(u === 5);
    assert(v === 6);
  }
}

fn(1);

//? test: let-declaration-shadowing-catch-parameter
//? description: let declaration shadowing catch parameter
//? expect: undefined
//? source: ...

try {
  throw 'stuff1';
} catch (a) {
  {
    // let declaration shadowing catch parameter
    let a = 3;
    assert(a === 3);
  }
  assert(a.message === 'stuff1');
}

//? test: let-declarations-shadowing-parameter-name-let-const-and-var
//? description: let declarations shadowing parameter name, let, const and var
//? expect: undefined
//? source: ...

function fn(a) {
  let b = 1;
  //var c = 1;
  const d = 1;
  {
    let a = 2;
    let b = 2;
    //let c = 2;
    let d = 2;
    assert(a === 2);
    assert(b === 2);
    //assert.sameValue(c, 2);
    assert(d === 2);
  }
}
fn(1);


//? test: lookup-from-closure
//? description: lookup from closure
//? expect: undefined
//? source: ...

function f5(one) {
  //var x = one + 1;
  let y = one + 2;
  const u = one + 4;
  {
    let z = one + 3;
    const v = one + 5;
    function f() {
      assert(one === 1);
      //assert.sameValue(x, 2);
      assert(y === 3);
      assert(z === 4);
      assert(u === 5);
      assert(v === 6);
    }

    f();
  }
}
f5(1);

//? test: lookup-in-and-through-block-contexts
//? description: lookup in and through block contexts
//? expect: undefined
//? source: ...

function fn(one) {
  //var x = one + 1;
  let y = one + 2;
  const u = one + 4;
  {
    let z = one + 3;
    const v = one + 5;
    assert(one === 1);
    //assert.sameValue(x, 2);
    assert(y === 3);
    assert(z === 4);
    assert(u === 5);
    assert(v === 6);
  }
}

fn(1);

//? test: parameter-name-shadowing-catch-parameter
//? description: parameter name shadowing catch parameter
//? expect: undefined
//? source: ...

// can't do this function decl/call syntax
function fn() {
  let c = 1;
  try {
    throw 'stuff3';
  } catch (c) {
    (function(c) {
      // parameter name shadowing catch parameter
      c = 3;
      assert(c === 3);
    })(1);
    assert(c.message === 'stuff3');
  }
  assert(c === 1);
}
fn();

//? test: parameter-name-shadowing-parameter-name-let-const-and-var
//? description: parameter name shadowing parameter name, let, const and var
//? expect: undefined
//? source: ...

function fn(a) {
  let b = 1;
  //var c = 1;
  const d = 1;

  function(a, b, c?, d?) {
    a = 2;
    b = 2;
    c = 2;
    d = 2;

    assert(a === 2);
    assert(b === 2);
    //assert.sameValue(c, 2);
    assert(d === 2);
  }(1, 1);

  assert(a === 1);
  assert(b === 1);
  //assert.sameValue(c, 1);
  assert(d === 1);
}

fn(1);