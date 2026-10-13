#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: perform_new_value_results.as
//? description: perform of a built-in that returns a new array or object (#600)
//? sourceType: script
//?
//? test: reverse
//? description: reverse returns a new array through perform
//? expect: [2, 1]
//? source: ...

const result = perform({function: "reverse", array: [1, 2]});
assert(result.status === "success", result.error?.message);
return result.result;

//?
//? test: slice
//? description: slice returns a new array through perform
//? expect: [2, 3]
//? source: ...

const result = perform({function: "slice", array: [1, 2, 3], startIndex: 1});
assert(result.status === "success", result.error?.message);
return result.result;

//?
//? test: keys
//? description: keys returns a new array through perform
//? expect: ["a"]
//? source: ...

const result = perform({function: "keys", object: {a: 1}});
assert(result.status === "success", result.error?.message);
return result.result;

//?
//? test: add_entries
//? description: add_entries returns its modified target through perform
//? expect: [1]
//? source: ...

const result = perform({function: "add_entries", target: [], source: [1]});
assert(result.status === "success", result.error?.message);
return result.result;

//?
//? test: actions
//? description: Several actions, each returning a new array
//? expect: [[2, 1], [3]]
//? source: ...

const result = perform({
    actions: [
        {function: "reverse", array: [1, 2]},
        {function: "slice", array: [1, 2, 3], startIndex: 2}
    ]
});
assert(result.status === "success", result.error?.message);
return [result.actions[0].result, result.actions[1].result];

//?
//? test: keep_response
//? description: A kept response and result outlive the loop that made them (#607)
//? expect: [[2, 1], "success", [3, 2, 1]]
//? source: ...

let kept;
let keptResult;
for (let i = 0; i < 3; i = i + 1) {
    const r = perform({function: "reverse", array: [1, 2]});
    kept = r;
    keptResult = perform({function: "reverse", array: [1, 2, 3]}).result;
}
return [kept.result, kept.status, keptResult];
