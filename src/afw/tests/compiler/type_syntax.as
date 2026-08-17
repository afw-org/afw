#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: type_syntax.as
//? customPurpose: Part of compiler category tests
//? description: ...
Type *syntax* only (issue #28): parse, store, and decompile round-trip with
checking off. Leaves, T[], tuples, unions, type/interface statements,
object and function type annotations, hard cut of old (array of …) forms
and TypeScript Array<T>. Does not enable typeCheck flags or #compile;
wrong assigns still run. Enforcement lives in type_check.as /
type_check_flags.as.
//? sourceType: script
//?
//? test: type-data-type-leaf
//? description: Adaptive data type as leaf
//? expect: 0
//? source: ...

const x: integer = 1;
const y: string = "a";
const z: any = 1;
return 0;

//?
//? test: type-array-postfix
//? description: integer[] and nested string[][] parse
//? expect: 0
//? source: ...

const a: integer[] = [1, 2];
const b: string[] = ["x"];
const c: integer[][] = [[1], [2, 3]];
return 0;

//?
//? test: type-tuple-and-union
//? description: tuples and unions
//? expect: 0
//? source: ...

const t: [integer, string] = [1, "a"];
const u: integer | string = 1;
const v: integer | string | boolean = true;
return 0;

//?
//? test: type-alias-statement
//? description: type Name = Type
//? expect: 0
//? source: ...

type Id = integer;
const x: Id = 3;
return 0;

//?
//? test: type-interface-extends
//? description: interface with extends and optional props
//? expect: 0
//? source: ...

interface Base {
    id: integer
};
interface Person extends Base {
    name: string,
    age?: integer
};
const p: Person = { id: 1, name: "a" };
return 0;

//?
//? test: type-object-literal-and-function
//? description: object type literal and function type
//? expect: 0
//? source: ...

const o: { host: string, port?: integer } = { host: "h" };
const f: (a: integer) => integer = function (a: integer): integer {
    return a;
};
assert(f(2) === 2);
return 0;

//?
//? test: type-hard-cut-old-array-of
//? description: old (array of integer) is a parse error
//? expect: error
//? source: ...

const x: (array of integer) = [1];
return 0;

//?
//? test: type-decompile-roundtrip-array
//? description: decompile integer[] recompiles
//? expect: 0
//? source: ...

const src = "const x: integer[] = [1,2];\nreturn x;";
const d1 = decompile(compile<script>(script(src)));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) == [1, 2]);
return 0;

//?
//? test: type-decompile-roundtrip-union
//? description: decompile integer|string recompiles
//? expect: 0
//? source: ...

const src = "const x: integer|string = 1;\nreturn x;";
const d1 = decompile(compile<script>(script(src)));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) == 1);
return 0;

//?
//? test: type-decompile-roundtrip-function-type
//? description: function type annotation decompiles
//? expect: 0
//? source: ...

const src =
    "const f: (a:integer)=>integer = function (a:integer):integer { return a; };\nreturn f(3);";
const d1 = decompile(compile<script>(script(src)));
const d2 = decompile(compile<script>(script(d1)));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) == 3);
return 0;

//?
//? test: type-Array-generic-rejected
//? description: TypeScript Array<T> spelling is a parse error
//? expect: error
//? source: ...

const x: Array<string> = ["a"];
return 0;

//?
//? test: type-parenthesized-and-bare-function
//? description: (T), nested parens, (T|U), and (T)=>R parse
//? expect: 0
//? source: ...

const a: (integer) = 1;
const b: ((integer)) = 2;
const c: (integer | string) = "x";
const f: (integer) => integer = function (n: integer): integer {
    return n;
};
assert(f(3) === 3);
const o: ({ host: string }) = { host: "h" };
assert(o.host === "h");
return 0;

//?
//? test: type-decompile-roundtrip-alias
//? description: type Id = integer decompiles and recompiles
//? expect: 0
//? source: ...

const src = "type Id = integer;\nconst x: Id = 3;\nreturn x;";
const d1 = decompile(compile<script>(script(src)));
const d2 = decompile(compile<script>(script(d1)));
assert(includes(d1, "#type("));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) === 3);
return 0;

//?
//? test: type-decompile-roundtrip-self-ref
//? description: type Node = { next?: Node } decompiles and recompiles
//? expect: 0
//? source: ...

const src = "type Node = { next?: Node };\nconst n: Node = { next: {} };\nreturn n.next;";
const d1 = decompile(compile<script>(script(src)));
const d2 = decompile(compile<script>(script(d1)));
assert(includes(d1, "#type("));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) !== undefined);
return 0;

//?
//? test: type-decompile-roundtrip-interface
//? description: interface Person decompiles and recompiles
//? expect: 0
//? source: ...

const src =
    "interface Base { id: integer };\n" +
    "interface Person extends Base { name: string };\n" +
    "const p: Person = { id: 1, name: \"a\" };\n" +
    "return p.name;";
const d1 = decompile(compile<script>(script(src)));
const d2 = decompile(compile<script>(script(d1)));
assert(includes(d1, "#interface("));
assert(d1 == d2);
assert(evaluate(compile<script>(script(d1))) === "a");
return 0;

//?
//? test: type-alias-self-ref-parses
//? description: type Node = { next?: Node } parses with checking off
//? expect: 0
//? source: ...

type Node = { next?: Node };
const n: Node = { next: { } };
return 0;

//?
//? test: type-alias-use-before-declare-rejected
//? description: type A cannot mention B declared later
//? expect: error
//? source: ...

type A = { b: B };
type B = integer;
const x: A = { b: 1 };
return 0;

//?
//? test: type-unknown-name-rejected
//? description: unknown type name is a compile error even with checking off
//? expect: error
//? source: ...

const x: Nope = 1;
return 0;

//?
//? test: type-alias-cycle-rejected
//? description: type A = A is a compile error
//? expect: error
//? source: ...

type A = A;
return 0;

//?
//? test: type-alias-declared-before-use
//? description: type B first, then type A = { b: B } parses
//? expect: 0
//? source: ...

type B = integer;
type A = { b: B };
const x: A = { b: 1 };
return 0;

//?
//? test: type-alias-union-cycle-rejected
//? description: type A = A | integer is a compile error
//? expect: error
//? source: ...

type A = A | integer;
return 0;

//?
//? test: interface-self-ref-parses
//? description: interface with a property of its own name parses
//? expect: 0
//? source: ...

interface Node {
    next?: Node
};
const n: Node = { };
return 0;

//?
//? test: interface-cannot-extend-self
//? description: interface Node extends Node is a compile error
//? expect: error
//? source: ...

interface Node extends Node {
    id: integer
};
return 0;
