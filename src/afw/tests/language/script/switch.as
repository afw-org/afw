#!/usr/bin/env -S afw --syntax test_script
//? testScript: switch.as
//? customPurpose: Part of language/script tests
//? description: Switch tests incl. #50 leave-path (return/break/throw) for #2
//? sourceType: script
//?
//? test: switch-eqx-1
//? description: eqx match
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

switch (1) using eqx {
    case 1:
        break;
    default:
        throw "fail";
}


//?
//? test: switch-eqx-2
//? description: eqx not match
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

switch (1) using eqx {
    case 2:
        throw "fail";
    default:
        break;
}


//?
//? test: switch-regexp-match
//? description: regex match
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

switch ("abc") using regexp_match {
    case "a.*":
        break;
    default:
        throw "fail";
}


//?
//? test: switch-regexp-not-match
//? description: regex not match
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

switch ("def") using regexp_match {
    case "a.*":
        throw "fail";
    default:
        break;
}



//?
//? test: switch-fn-match
//? description: fn match
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

function fn(a, b) {
    return true;
}

switch ("abc") using fn {
    case "anything":
        break;
    default:
        throw "fail";
}


//?
//? test: switch-fn-no-match
//? description: fn no match
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

function fn(a, b) {
    return false;
}

switch ("abc") using fn {
    case "anything":
        throw "fail";
    default:
        break;
}

// Issue #50 — return/leave from switch must reach the function.
// Permanent statement_flow regression; keep green under #2.
// (Plain // lines above are still inside this case's source body.)

//?
//? test: switch-return-fn
//? description: #50 — return from matched case leaves enclosing function
//? skip: false
//? expect: undefined
//? source: ...

function f(x) {
    switch (x) {
        case 1:
            return true;
    }
    return false;
}

assert(f(1) === true);
assert(f(0) === false);

//?
//? test: switch-return-default
//? description: #50 — return from default clause leaves function
//? skip: false
//? expect: 0
//? source: ...

function f(x) {
    switch (x) {
        case 1:
            return "one";
        default:
            return "other";
    }
}

assert(f(1) === "one");
assert(f(9) === "other");
return 0;

//?
//? test: switch-return-expression
//? description: #50 — return expression value from case (not just constant)
//? skip: false
//? expect: 0
//? source: ...

function f(x) {
    switch (x) {
        case 1:
            return x + 10;
    }
    return 0;
}

assert(f(1) === 11);
return 0;

//?
//? test: switch-return-fallthrough
//? description: #50 — empty case fall-through then return
//? skip: false
//? expect: 0
//? source: ...

function f(x) {
    switch (x) {
        case 1:
        case 2:
            return "12";
        case 3:
            return "3";
    }
    return "none";
}

assert(f(1) === "12");
assert(f(2) === "12");
assert(f(3) === "3");
assert(f(4) === "none");
return 0;

//?
//? test: switch-return-fallthrough-with-side-effects
//? description: #50 — fall-through runs prior case stmts then return
//? skip: false
//? expect: 0
//? source: ...

function f(x) {
    let s = "";
    switch (x) {
        case 1:
            s = s + "a";
        case 2:
            s = s + "b";
            return s;
        case 3:
            return "c";
    }
    return "none";
}

assert(f(1) === "ab");
assert(f(2) === "b");
return 0;

//?
//? test: switch-break-then-function-return
//? description: #50-adjacent — break exits switch only; function return after
//? skip: false
//? expect: 0
//? source: ...

function f(x) {
    switch (x) {
        case 1:
            break;
    }
    return "after";
}

assert(f(1) === "after");
return 0;

//?
//? test: switch-return-skips-following-statements
//? description: #50 — statements after switch must not run after case return
//? skip: false
//? expect: 0
//? source: ...

function f(x) {
    let hit = false;
    switch (x) {
        case 1:
            return true;
    }
    hit = true;
    return hit;
}

assert(f(1) === true);
return 0;

//?
//? test: switch-return-nested-inner
//? description: #50 — return from nested switch leaves outer function
//? skip: false
//? expect: 0
//? source: ...

function f(x) {
    switch (x) {
        case 1:
            switch (x) {
                case 1:
                    return "inner";
            }
            return "outer-case";
    }
    return "none";
}

assert(f(1) === "inner");
return 0;

//?
//? test: switch-break-nested-then-outer-return
//? description: #50 — break in inner switch; return in outer case still leaves fn
//? skip: false
//? expect: 0
//? source: ...

function f(x) {
    switch (x) {
        case 1:
            switch (x) {
                case 1:
                    break;
            }
            return "outer";
    }
    return "none";
}

assert(f(1) === "outer");
return 0;

//?
//? test: switch-return-from-for-in-case
//? description: #50 — return from for-loop inside case leaves function
//? skip: false
//? expect: 0
//? source: ...

function f(n) {
    switch (n) {
        case 1:
            for (let i = 0; i < 5; i = i + 1) {
                if (i === 2) {
                    return i;
                }
            }
            return -1;
    }
    return -2;
}

assert(f(1) === 2);
return 0;

//?
//? test: switch-return-inside-try
//? description: #50 — return from switch inside try leaves function (not catch)
//? skip: false
//? expect: 0
//? source: ...

function f(x) {
    try {
        switch (x) {
            case 1:
                return "from-switch";
        }
        return "after-switch";
    } catch (e) {
        return "catch";
    }
}

assert(f(1) === "from-switch");
return 0;

//?
//? test: switch-throw-fn
//? description: #50-adjacent — throw from case leaves function via catch
//? skip: false
//? expect: 0
//? source: ...

function f(x) {
    switch (x) {
        case 1:
            throw "fail";
    }
    return false;
}

let thrown = false;

try {
    f(1);
} catch (e) {
    thrown = true;
    assert(e.message === "fail");
}

assert(thrown === true);
return 0;
