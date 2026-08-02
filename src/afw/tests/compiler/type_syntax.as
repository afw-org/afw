#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: type_syntax.as
//? customPurpose: Part of compiler category tests
//? description: TS-like type syntax parse/store (issue #28; no checking yet)
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
//? test: type-array-and-Array-generic
//? description: integer[] and Array<integer> parse
//? expect: 0
//? source: ...

const a: integer[] = [1, 2];
const b: Array<string> = ["x"];
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
