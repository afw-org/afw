#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: template_substitution_memory.as
//? customPurpose: Part of language tests
//? description: A template substitution's value lasts for its scope, not the xctx (#538)
//? sourceType: script
//?
//? test: substitution-in-loop-is-flat
//? description: Substitutions evaluated on every loop trip do not grow with the trip count
//? expect: true
//? source: ...

function f(t: integer): string { throw "boom"; }

function run(kind: integer, n: integer): integer {
    let out = "";
    let s = "ab";
    function g(): string { return s; }
    const before = pool_bytes_in_use();
    for (let T = 0; T < n; T = T + 1) {
        if (kind === 0) { out = `x${T}`; }
        else if (kind === 1) { const q = `${T}`; }
        else if (kind === 2) { out = `L${"x"}`; }
        else if (kind === 3) { out = `x${ let a = T; return a; }`; }
        else if (kind === 4) {
            { [T]; }
            try { out = `x${f(T)}`; } catch (e) { }
        }
        else { { g(); } out = `x${s}`; }
    }
    return pool_bytes_in_use() - before;
}

for (let kind = 0; kind <= 5; kind = kind + 1) {
    run(kind, 50);
    const few = run(kind, 100);
    const many = run(kind, 1600);
    assert(many - few < 1024, "kind " + to_string(kind) + ", 100: " +
        to_string(few) + ", 1600: " + to_string(many));
}
return true;

//? test: substitution-values
//? description: Substitutions give their values, also with statements and after a throw
//? expect: true
//? source: ...

function f(): string { throw "boom"; }

let out = "";
for (let T = 0; T < 3; T = T + 1) {
    out = out + `<${T}|${ let a = T * 2; return a; }>`;
}
assert(out === "<0|0><1|2><2|4>", out);

let caught = "";
try { out = `x${f()}`; } catch (e) { caught = e.message; }
assert(caught === "boom", "caught: " + caught);
assert(out === "<0|0><1|2><2|4>", "out after throw: " + out);
assert(`after ${"throw"}` === "after throw");
return true;
