#!/usr/bin/env -S afw --syntax test_script
//? testScript: switch.as
//? customPurpose: Part of language/script tests
//? description: Additional switch tests
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


//?
//? test: switch-return-fn
//? description: return from switch within function
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

function f(x) {

    switch (x) {
        case 1:
            return true;
    }

    return false;
}

assert(f(1) === true);

//?
//? test: switch-throw-fn
//? description: return from switch within function
//? expect: undefined
//? source: ...
#!/usr/bin/env afw

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