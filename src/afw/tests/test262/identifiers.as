#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: identifiers.as
//? customPurpose: Part of test262
//? description: identifiers
//? sourceType: script
//?
//? test: start-underscore
//? description: "IdentifierStart :: _"
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc _ = 1;
assert(_ === 1);

loc _x = 2;
assert(_x === 2);

loc _$ = 3;
assert(_$ === 3);

loc __ = 4;
assert(__ === 4);


//? test: start-unicode-ltr
//? description: "IdentifierStart :: UnicodeLetter"
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc x = 1;
assert(x === 1);

loc xx = 2;
assert(xx === 2);

loc x$ = 3;
assert(x$ === 3);

loc x_ = 4;
assert(x_ === 4);


//? test: val-case
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc case = 123;



//? test: val-catch
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc catch = 123;


//? test: val-const
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc const = 123;



//? test: val-continue
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc continue = 123;



//? test: val-else
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc else = 123;



//? test: val-false
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw


loc false = 123;



//? test: val-finally
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw


loc finally = 123;



//? test: val-for
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc for = 123;



//? test: val-function
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc function = 123;



//? test: val-if
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc if = 123;


//? test: val-in
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc in = 123;


//? test: val-null
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc null = 123;


//? test: val-return
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc return = 123;


//? test: vals-eng-alpha-lower
//? description: Check ENGLISH SMALL ALPHABET
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc a = 1;
if (a !== 1) {
  assert(false, '#a');
}
loc b = 1;
if (b !== 1) {
  assert(false, '#b');
}
loc c = 1;
if (c !== 1) {
  assert(false, '#c');
}
loc d = 1;
if (d !== 1) {
  assert(false, '#d');
}
loc e = 1;
if (e !== 1) {
  assert(false, '#e');
}
loc f = 1;
if (f !== 1) {
  assert(false, '#f');
}
loc g = 1;
if (g !== 1) {
  assert(false, '#g');
}
loc h = 1;
if (h !== 1) {
  assert(false, '#h');
}
loc i = 1;
if (i !== 1) {
  assert(false, '#i');
}
loc j = 1;
if (j !== 1) {
  assert(false, '#j');
}
loc k = 1;
if (k !== 1) {
  assert(false, '#k');
}
loc l = 1;
if (l !== 1) {
  assert(false, '#l');
}
loc m = 1;
if (m !== 1) {
  assert(false, '#m');
}
loc n = 1;
if (n !== 1) {
  assert(false, '#n');
}
loc o = 1;
if (o !== 1) {
  assert(false, '#o');
}
loc p = 1;
if (p !== 1) {
  assert(false, '#p');
}
loc q = 1;
if (q !== 1) {
  assert(false, '#q');
}
loc r = 1;
if (r !== 1) {
  assert(false, '#r');
}
loc s = 1;
if (s !== 1) {
  assert(false, '#s');
}
loc t = 1;
if (t !== 1) {
  assert(false, '#t');
}
loc u = 1;
if (u !== 1) {
  assert(false, '#u');
}
loc v = 1;
if (v !== 1) {
  assert(false, '#v');
}
loc w = 1;
if (w !== 1) {
  assert(false, '#w');
}
loc x = 1;
if (x !== 1) {
  assert(false, '#x');
}
loc y = 1;
if (y !== 1) {
  assert(false, '#y');
}
loc z = 1;
if (z !== 1) {
  assert(false, '#z');
}



//? test: vals-eng-alpha-upper
//? description: Check ENGLISH CAPITAL ALPHABET
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc A = 1;
assert(A === 1);

loc B = 1;
assert(B === 1);

loc C = 1;
assert(C === 1);

loc D = 1;
assert(D === 1);

loc E = 1;
assert(E === 1);

loc F = 1;
assert(F === 1);

loc G = 1;
assert(G === 1);

loc H = 1;
assert(H === 1);

loc I = 1;
assert(I === 1);

loc J = 1;
assert(J === 1);

loc K = 1;
assert(K === 1);

loc L = 1;
assert(L === 1);

loc M = 1;
assert(M === 1);

loc N = 1;
assert(N === 1);

loc O = 1;
assert(O === 1);

loc P = 1;
assert(P === 1);

loc Q = 1;
assert(Q === 1);

loc R = 1;
assert(R === 1);

loc S = 1;
assert(S === 1);

loc T = 1;
assert(T === 1);

loc U = 1;
assert(U === 1);

loc V = 1;
assert(V === 1);

loc W = 1;
assert(W === 1);

loc X = 1;
assert(X === 1);

loc Y = 1;
assert(Y === 1);

loc Z = 1;
assert(Z === 1);




//? test: vals-rus-alpha-lower
//? description: Check RUSSIAN SMALL ALPHABET
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc а = 1;
assert(а === 1);

loc б = 1;
assert(б === 1);

loc в = 1;
assert(в === 1);

loc г = 1;
assert(г === 1);

loc д = 1;
assert(д === 1);

loc е = 1;
assert(е === 1);

loc ж = 1;
assert(ж === 1);

loc з = 1;
assert(з === 1);

loc и = 1;
assert(и === 1);

loc й = 1;
assert(й === 1);

loc к = 1;
assert(к === 1);

loc л = 1;
assert(л === 1);

loc м = 1;
assert(м === 1);

loc н = 1;
assert(н === 1);

loc о = 1;
assert(о === 1);

loc п = 1;
assert(п === 1);

loc р = 1;
assert(р === 1);

loc с = 1;
assert(с === 1);

loc т = 1;
assert(т === 1);

loc у = 1;
assert(у === 1);

loc ф = 1;
assert(ф === 1);

loc х = 1;
assert(х === 1);

loc ц = 1;
assert(ц === 1);

loc ч = 1;
assert(ч === 1);

loc ш = 1;
assert(ш === 1);

loc щ = 1;
assert(щ === 1);

loc ъ = 1;
assert(ъ === 1);

loc ы = 1;
assert(ы === 1);

loc ь = 1;
assert(ь === 1);

loc э = 1;
assert(э === 1);

loc ю = 1;
assert(ю === 1);

loc я = 1;
assert(я === 1);

loc ё = 1;
assert(ё === 1);




//? test: val-throw
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc throw = 123;


//? test: val-true
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc true = 123;


//? test: val-try
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc try = 123;



//? test: val-underscore
//? description: Create variable _
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
loc _ = 1;

assert(_ === 1);



//? test: val-var
//? description:...
    SyntaxError expected: reserved words used as Identifier
//? expect: error
//? source: ...
#!/usr/bin/env afw


loc loc = 123;