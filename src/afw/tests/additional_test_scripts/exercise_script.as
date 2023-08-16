#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: exercise_script.as
//? customPurpose: Part of core function tests
//? description: Tests to exercise adaptive script.
//? sourceType: script
//?
//? test: exercise_script_with_asserts
//? description: Script test with asserts.
//? expect: 0
//? source: ...
#!/usr/bin/env afw


// There are still parts of this script that needs to be converted to assert()

let s: string;

const deep: object = {
    a: 'a',
    b: 'b',
    c: {
        c1: 'c1',
        c2: 'c2'
    },
    d: ['d1','d2','d3','d4'],
    e: {
        e1: 'e1'
    },
    f: function (a: string): string {return a;}
};

assert(deep.a == 'a');

assert(!is_defined(deep?.x));

assert(deep.c?.c2 == 'c2');

assert(deep.c?.c3 == undefined);

assert(deep.f('xyz') == 'xyz');

assert(deep?.f('xyz') == 'xyz');

assert(deep['a'] == 'a');

assert(deep?.['notfound'] == undefined);

assert(deep.d?.[1] == 'd2');

assert(deep.d[0]->concat("!!!!") == 'd1!!!!');

assert(deep.d[0]?->concat("!!!!") == 'd1!!!!');

assert(deep.d[0]?->not::there?.("!!!!") == undefined);

// Test optional chaining for array

const deeplist: array = [
    "e1",
    "e2",
    {name:'e3', nested:['e3n1','e3n2', {name:'e3n3'}]},
    ['e4a','e4b']
];

assert(deeplist[0] == 'e1');

assert(deeplist?.[0] == 'e1');

assert(deep.c?.c2 == 'c2');

// Test ??

let nc: string;

assert(nc == undefined);

assert(undefined ?? null ?? nc ?? 'Hello' == 'Hello');

nc = nc ?? "Default value";
assert(nc == "Default value");

nc = nc ?? "Should be ignored";
assert(nc == "Default value");

// Test ??=

let nc2: string;

assert(!is_defined(nc2));

nc2 ??= "Default value";

assert(nc2 == 'Default value');

nc2 ??= "Should be ignored";
assert(nc2 == 'Default value');

// Various function with optional and default parameters.

function test1 (a: string, b: string , c: string): string (a + b + c);
assert(test1('1', '2', '3') == '123');

function test2 (a: string, b: string, c:string='3'): string (a + b + c);
assert(test2('1', '2') == '123');

function test3 (a: string, b: string, c?: string): string (a + b + string(c));
assert(test3('1', '2') == '12<undefined>');

function test4 (a: string, ... b: (array of string)): string (a + string(b));
assert(test4('1', '2', '3', '4', '5') == '1["2","3","4","5"]');

function test5 (a: string, ... b: (array of string)): string (a + string(b));
assert(test5('1') == '1[]');

function test6(a: string, b?: string): string
{
    if (b !== undefined) {
        return b;
    }
    else {
        return "b is missing";
    }
}
assert(test6('1', '2') == '2');

assert(test6('1') == 'b is missing');

function test7(a: string, b?: string): string
{
    if (b === undefined) {
        return "b is missing";
    }
    else {
        return b;
    }

}

assert(test7('1', '2') == '2');

assert(test7('1') == 'b is missing');

function test8(a: string, b?: string): string
{
    if (b != undefined) {
        return b;
    }
    else {
        return "b is missing";
    }
}

assert(test8('1', '2') == '2');

assert(test8('1') == 'b is missing');

function test9(a: string, b?: string): string
{
    if (b == undefined) {
        return "b is missing";
    }
    else {
        return b;
    }
}

assert(test9('1', '2') == '2');

assert(test9('1') == 'b is missing');

// Trailing comma in array
let y1: array = [1,3,2,4,];

assert(string(y1) == string([1,3,2,4])); //FIXME Need to support array ==

// Trailing comma in object
let y2: object = {a:1,b:2,};

assert(string(y2) == '{"a":1,"b":2}'); //FIXME Need to support object ==

// Test evaluate script  
{
    const hello: unevaluated = evaluate(script("return 'Hello ' + 'World!\n';"));
    //FIXME Should pass
    //assert(evaluate_value(hello) == 'Hello World!');
}

// Test produce a compile listing
{
    const hello: unevaluated = compile(script("return 'Hello ' + 'World!\n';"), "| ");
    //FIXME Figure out a way to assert this: hello
}


// Miscellaneous tests

// ...array
{
    const l1: (array of string) = ["a", "b"];
    const l2: array = ["d", "e"];
    const l3: array = [...l1, "c", ...l2, "f", "g", ...array("h", "i", "j"), "k",
        ...["l","m"], ...[], ...[...["n", "o", "p"], "q", "r", "s"] ];
    assert(stringify(l3) == '["a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s"]');
}

// ...object
{
    const o1: object = {"a":"should be replaced by a in o2", "b":"b!"};
    const o2: object = {"a":"a!", d:"d!", e:"e!"};
    const o3: object = {...o1, "c": "c!", ...o2, f:"f!", g:"g!", ...{h:"h!", i:"i!", j:"j!"}, k:"k!",
        ...{l:"l!",m:"m!"}, ...{}, ...{...{n:"n!", o:"o!", p:"p!"}, q:"q!", r:"r!", s:"s!"} };

    // Object with "name":"value" pairs where name is "a"-"s" and paired value is "a!"-"s!"
    assert(stringify(o3) == '{"a":"a!","b":"b!","c":"c!","d":"d!","e":"e!","f":"f!","g":"g!","h":"h!","i":"i!","j":"j!","k":"k!","l":"l!","m":"m!","n":"n!","o":"o!","p":"p!","q":"q!","r":"r!","s":"s!"}');
}


let hello: string = "Defined outside block";
{
    let hello: any = "Defined inside block";
    assert(hello == 'Defined inside block');
}
assert(hello == 'Defined outside block');


// Test array destructure
let world: any = "?";

[hello, , world] = ["entry1", "entry2", "entry3" ];
assert( ( hello + " " + world ) == 'entry1 entry3');

{
    const [hello: string, , world] = ["entry1", "entry2", "entry3" ];
    assert( ( hello + " " + world ) == 'entry1 entry3');
}

// Test 2 array destructure
[hello, world] = ["entry1", "entry2", "entry3", "entry4" ];
assert( ( hello + " " + world ) == 'entry1 entry2');

// Test 3 array destructure
let rest: string;
[hello, world, ...rest] = ["entry1", "entry2", "entry3", "entry4" ];
assert( ( hello + " " + world + ' rest=' + string(rest)) == 'entry1 entry2 rest=["entry3","entry4"]');

//
const immutable: string = "I can't change";
let obj: object = {hello: "OHello", world: "OWorld!", la:"LA", la2:"LA"};

// Test object destructure
({hello, world} = obj);
assert( ( hello + " " + world ) == 'OHello OWorld!');

// Test object destructure
({hello, world, ...rest} = obj);
assert( ( hello + " " + world + ' rest=' + string(rest)) == 'OHello OWorld! rest={"la":"LA","la2":"LA"}');

// Test object destructure
{
    const {hello, world="Unused default" , extra="!!!", ...rest} = obj;
    assert( ( hello + " " + world + extra + ' rest=' + string(rest)) == 'OHello OWorld!!!! rest={"la":"LA","la2":"LA"}');
}

// Test object destructure
{
    const {hello: newname, world="Unused default", extra="!!!", ...rest} = obj;
    assert( ( newname + " " + world + extra + ' rest=' + string(rest)) == 'OHello OWorld!!!! rest={"la":"LA","la2":"LA"}');
}

// Test object destructure
{
    const obj2: object = {embedded:obj, a:"?", b:2, array: ['a','****','c'] };
    const {embedded: {hello, y="OWorld!"}, a:extra, array: [,extra2,,,extra3='+crazy'] } = obj2;
    assert( ( hello + " " + y + extra + extra2 + extra3 ) == 'OHello OWorld!?****+crazy');
}

// Test let
let x: string = "Defined Variable";
assert(x == 'Defined Variable');

// Test assign
x = "Changed variable";
assert(x == 'Changed variable');

let num: integer;

// Test +=
num = 21;
num += 5;
assert( num == 26 );

// Test -=
num = 21;
num -= 5;
assert( num == 16 );

// Test /=
num = 21;
num /= 2;
assert( num == 10 );

// Test %=
num = 11;
num %= 1 + 1;
assert( num == 1 );

// Test assign before for-of
x = "outside for-of";
assert(x == 'outside for-of');

// Test for-of
{
    s = '';
    for (let x: string of ['a','b','c']) {
        s += x;
    }
    assert( s == 'abc' );
}

// Test make sure x same after for-of - should print >>>outside for-of\n>>>');
assert(x == 'outside for-of');

//FIXME Need to convert this
/*
print('\nTest for-of using retrieve_objects- should print >>> with info for each data type\n');
{
    for (const x: object of retrieve_objects('afw','_AdaptiveDataType_')) {
        print('>>> ' + x.dataType + ' - ' + x.brief + '\n');
    }
}
print("\n");
*/

//FIXME Need to convert this
/*
print('\nTest for-of using retrieve_objects- should print >>> with info for each data type\n');
for (const {dataType, brief } of retrieve_objects('afw','_AdaptiveDataType_')) {
    print('>>> ' + dataType + ' - ' + brief + '\n');
}
print("\n");
*/

//FIXME Need to convert this
/*
print('\nTest for-of using same retrieve_objects with object destructure - should print >>> with info for each data type\n');
{
    for (let {dataType, brief} of retrieve_objects('afw','_AdaptiveDataType_')) {
        print('>>> ' + dataType + ' - ' + brief + '\n');
    }    
}
print("\n");
*/

// Test while
s = '';
x = 0;
while (x < 5) {
    s += string(x);
    x += 1;
}
assert( s == '01234');

// Test do while
s = '';
x = 0;
do {
    s += string(x);
    x += 1;
} while (x < 5);
assert( s == '01234');

// Test while
s = '';
x = 0;
while (x < 0) {
    s += string(x);
    x += 1;
}
assert( s == '');

// Test do while
s = '';
x = 0;
do {
    s += string(x);
    x += 1;
} while (x < 0);
assert( s == '0');

// Test do while break
s = '';
x = 0;
do {
    s += string(x);
    x += 1;
    if (x >= 5) break;
} while (true);
assert( s == '01234');

// Test do while continue
s = '';
x = 0;
do {
    s += string(x);
    x += 1;
    if (x < 5) continue;
    break;
} while (true);
assert( s == '01234');

// Test template string
assert(`Should be ${(1 + 1 == 2) ? "yes" : "no"}` == 'Should be yes');

{
    // Test template string with boolean result
    assert(`${(1 + 1 == 2)}` == true);

    // Test template string with get_object for _AdaptiveDataType_/string/cType
    assert(`${
        get_object('afw', '_AdaptiveDataType_', 'string').cType
        }` == 'afw_utf8_t');

    // Test template string with get_object for _AdaptiveDataType_/string/cType
    s = `${
        const obj: object = get_object('afw', '_AdaptiveDataType_', 'string');
        return obj.cType;
        }`;
    assert(s == 'afw_utf8_t');
}

// NaN
assert(string(NaN) == 'NaN');

// -Infinity
assert(string(-Infinity) == '-Infinity');

// Test !==
assert( (1 !== 1) == false);

// Equality test
assert( (1 == integer('1'))   == true);
assert( ('1' == string(1))   == true);
assert( (1 === '1')  == false);
assert( ('1' === 1)  == false);
assert( (1 === 1)    == true);
assert( (1 !== '1')  == true);
assert( (1 !== 1)    == false);

// Comparison tests
assert( (1 > 1)  == false);
assert( (1 >= 1) == true);
assert( (1 < 1)  == false);
assert( (1 <= 1) == true);

// Comparison tests with NaN
assert( (NaN == 1.0) == false);
assert( (NaN == NaN) == false);
assert( (1.0 == NaN) == false);

assert( (NaN != 1.0) == true);
assert( (NaN != NaN) == true);
assert( (1.0 != NaN) == true);

assert( (NaN === 1.0) == false);
assert( (NaN === NaN) == false);
assert( (1.0 === NaN) == false);

assert( (NaN !== 1.0) == true);
assert( (NaN !== NaN) == true);
assert( (1.0 !== NaN) == true);

assert( (NaN <= 1.0) == false);
assert( (NaN <= NaN) == false);
assert( (1.0 <= NaN) == false);

assert( (NaN < 1.0) == false);
assert( (NaN < NaN) == false);
assert( (1.0 < NaN) == false);

assert( (NaN >= 1.0) == false);
assert( (NaN >= NaN) == false);
assert( (1.0 >= NaN) == false);

assert( (NaN > 1.0) == false);
assert( (NaN > NaN) == false);
assert( (1.0 > NaN) == false);

// If tests
if (1 + 1 == 2) {
    s = "success";
}
else {
    s = "failed";
}
assert( s == 'success');

if (1 + 1 == 3) {
    s = "failed";
}
else {
    s = "success";
}
assert( s == 'success');


if (1 + 1 == 3) {
    s = "failed";
}
else if (1 + 1 == 2) {
    s = "success";
}
assert( s == 'success');

// if without ()
if (1 + 1 == 2) {
    s = "success";
}
else {
    s = "failed";
}
assert( s == 'success');

// if (1 + 1 == 2) without () or block success
if (1 + 1 == 2) s = 'success';
else s = "failed";
assert( s == 'success');


function abc (a: string, b: string , c: string): string (a + b + c)
assert(abc("Function", " abc ", "worked!") == 'Function abc worked!');
assert(abc("Function", " abc ", "worked twice!") == 'Function abc worked twice!');

s = '';
let i: integer;
let j: integer;
for (i = 1; i < 5; i = i + 1) {
    s += string(i) + '\n';
}
assert(s == '1\n2\n3\n4\n');

s = '';
for (i = 1 , j = 2; i < 5; i = i + 1, j = i + 1) {
    s += string(i) + ',' + string(j) + '\n';
}
assert(s == '1,2\n2,3\n3,4\n4,5\n');

s = '';
for (i = 10 , j = 2; i < 20; i = i + 1, j = i * 2) {
    s += `${i},${j}\n`;
}
assert(s == "10,2\n11,22\n12,24\n13,26\n14,28\n15,30\n16,32\n17,34\n18,36\n19,38\n");

// 2.0**3 - should be 08.0E
assert(string(2.0**3.0) == '8.0E0');

// 2 + +1 - should be 3
assert(string(2 + +1) == '3');

// 2 + -1 - should be 1
assert(string(2 + -1) == '1');

// -2 + -1 - should be -3
assert(string(-2 + -1) == '-3');

// Test +=
hello = 'Hello';
hello += ' World!';
assert(hello == 'Hello World!');

return 0;
