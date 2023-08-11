#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: identifiers.as
//? customPurpose: Part of test262
//? description: identifiers
//? sourceType: script
//?
//? test: start-underscore
//? description: "IdentifierStart :: _"
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let _ = 1;
assert(_ === 1);

let _x = 2;
assert(_x === 2);

let _$ = 3;
assert(_$ === 3);

let __ = 4;
assert(__ === 4);


//? test: start-unicode-ltr
//? description: "IdentifierStart :: UnicodeLetter"
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let x = 1;
assert(x === 1);

let xx = 2;
assert(xx === 2);

let x$ = 3;
assert(x$ === 3);

let x_ = 4;
assert(x_ === 4);


//? test: val-case
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let case = 123;



//? test: val-catch
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let catch = 123;


//? test: val-const
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let const = 123;



//? test: val-continue
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let continue = 123;



//? test: val-else
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let else = 123;



//? test: val-false
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw


let false = 123;



//? test: val-finally
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw


let finally = 123;



//? test: val-for
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let for = 123;



//? test: val-function
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let function = 123;



//? test: val-if
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let if = 123;


//? test: val-in
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let in = 123;


//? test: val-null
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let null = 123;


//? test: val-return
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let return = 123;


//? test: vals-eng-alpha-lower
//? description: Check ENGLISH SMALL ALPHABET
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let a = 1;
if (a !== 1) {
  throw '#a';
}
let b = 1;
if (b !== 1) {
  throw '#b';
}
let c = 1;
if (c !== 1) {
  throw '#c';
}
let d = 1;
if (d !== 1) {
  throw '#d';
}
let e = 1;
if (e !== 1) {
  throw '#e';
}
let f = 1;
if (f !== 1) {
  throw '#f';
}
let g = 1;
if (g !== 1) {
  throw '#g';
}
let h = 1;
if (h !== 1) {
  throw '#h';
}
let i = 1;
if (i !== 1) {
  throw '#i';
}
let j = 1;
if (j !== 1) {
  throw '#j';
}
let k = 1;
if (k !== 1) {
  throw '#k';
}
let l = 1;
if (l !== 1) {
  throw '#l';
}
let m = 1;
if (m !== 1) {
  throw '#m';
}
let n = 1;
if (n !== 1) {
  throw '#n';
}
let o = 1;
if (o !== 1) {
  throw '#o';
}
let p = 1;
if (p !== 1) {
  throw '#p';
}
let q = 1;
if (q !== 1) {
  throw '#q';
}
let r = 1;
if (r !== 1) {
  throw '#r';
}
let s = 1;
if (s !== 1) {
  throw '#s';
}
let t = 1;
if (t !== 1) {
  throw '#t';
}
let u = 1;
if (u !== 1) {
  throw '#u';
}
let v = 1;
if (v !== 1) {
  throw '#v';
}
let w = 1;
if (w !== 1) {
  throw '#w';
}
let x = 1;
if (x !== 1) {
  throw '#x';
}
let y = 1;
if (y !== 1) {
  throw '#y';
}
let z = 1;
if (z !== 1) {
  throw '#z';
}



//? test: vals-eng-alpha-upper
//? description: Check ENGLISH CAPITAL ALPHABET
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let A = 1;
assert(A === 1);

let B = 1;
assert(B === 1);

let C = 1;
assert(C === 1);

let D = 1;
assert(D === 1);

let E = 1;
assert(E === 1);

let F = 1;
assert(F === 1);

let G = 1;
assert(G === 1);

let H = 1;
assert(H === 1);

let I = 1;
assert(I === 1);

let J = 1;
assert(J === 1);

let K = 1;
assert(K === 1);

let L = 1;
assert(L === 1);

let M = 1;
assert(M === 1);

let N = 1;
assert(N === 1);

let O = 1;
assert(O === 1);

let P = 1;
assert(P === 1);

let Q = 1;
assert(Q === 1);

let R = 1;
assert(R === 1);

let S = 1;
assert(S === 1);

let T = 1;
assert(T === 1);

let U = 1;
assert(U === 1);

let V = 1;
assert(V === 1);

let W = 1;
assert(W === 1);

let X = 1;
assert(X === 1);

let Y = 1;
assert(Y === 1);

let Z = 1;
assert(Z === 1);




//? test: vals-rus-alpha-lower
//? description: Check RUSSIAN SMALL ALPHABET
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


let а = 1;
assert(а === 1);

let б = 1;
assert(б === 1);

let в = 1;
assert(в === 1);

let г = 1;
assert(г === 1);

let д = 1;
assert(д === 1);

let е = 1;
assert(е === 1);

let ж = 1;
assert(ж === 1);

let з = 1;
assert(з === 1);

let и = 1;
assert(и === 1);

let й = 1;
assert(й === 1);

let к = 1;
assert(к === 1);

let л = 1;
assert(л === 1);

let м = 1;
assert(м === 1);

let н = 1;
assert(н === 1);

let о = 1;
assert(о === 1);

let п = 1;
assert(п === 1);

let р = 1;
assert(р === 1);

let с = 1;
assert(с === 1);

let т = 1;
assert(т === 1);

let у = 1;
assert(у === 1);

let ф = 1;
assert(ф === 1);

let х = 1;
assert(х === 1);

let ц = 1;
assert(ц === 1);

let ч = 1;
assert(ч === 1);

let ш = 1;
assert(ш === 1);

let щ = 1;
assert(щ === 1);

let ъ = 1;
assert(ъ === 1);

let ы = 1;
assert(ы === 1);

let ь = 1;
assert(ь === 1);

let э = 1;
assert(э === 1);

let ю = 1;
assert(ю === 1);

let я = 1;
assert(я === 1);

let ё = 1;
assert(ё === 1);




//? test: val-throw
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let throw = 123;


//? test: val-true
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let true = 123;


//? test: val-try
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

let try = 123;



//? test: val-underscore
//? description: Create variable _
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#1
let _ = 1;

assert(_ === 1);



//? test: val-var
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw


let let = 123;