#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: return-from.as
//? customPurpose: Part of test262
//? description: block-scope/return-from
//? sourceType: script
//?
//? test: block-const
//? description: return from block
//? expect: null
//? source: ...
#!/usr/bin/env afw

function fn() {
    const u = 3;
    {
        const v = 6;
        return u + v;
    }    
}
assert(fn() === 9);

//? test: block-let
//? description: return from block
//? expect: null
//? source: ...
#!/usr/bin/env afw

function fn() {
    let x = 3;
    {
        let y = 6;
        return x + y;
    }
}
assert(fn() === 9);