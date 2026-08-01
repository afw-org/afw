#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: happy_anniversary_39.as
//? customPurpose: Part of miscellaneous category tests
//? description: Easter egg Rube Goldberg using param/catch Patterns, call-site spread, string/computed keys (issues #140 / #38). Birthday card for Jeremy.
//? sourceType: script
//?
//? test: happy-anniversary-of-becoming-39
//? description: Overengineered wish that evaluates to Happy Anniversary of becoming 39!
//? expect: "Happy Anniversary of becoming 39!"
//? source: ...

/* Rube Goldberg anniversary machine.
 *
 * Uses: array + object Patterns on formals, nested catch Pattern,
 * string / computed property keys, Expression defaults, rest formals,
 * call-site f(...arr) spread, for-of, throw data, assert.
 * Age 39 is not a literal — it is 30 plus nine rest candles. Obviously.
 */

function yearsAfter([decade, ...candles]) {
    return decade + length(candles);
}

function reassemble(guest, age, _postage, ...words) {
    let out = "";
    for (const w of words) {
        if (out === "") {
            out = w;
        } else {
            out = out + " " + w;
        }
    }
    assert(guest === "Jeremy");
    assert(age === 39);
    return out;
}

function openEnvelope(fn) {
    try {
        fn();
        return "no cake?";
    } catch ({
        message,
        data: {
            guest,
            age,
            tokens: [a, b, c, d, e, ...extra]
        }
    }) {
        /* Official reassembly line: rest formal + call-site spread. */
        return reassemble(guest, age, message, ...[a, b, c, d, e], ...extra);
    }
}

function partyMail({
    "guest of honor": guest,
    wish: [happy, anniversary],
    bridge: { of: ofWord, becoming },
    ["sparkle"]: bang = "!"
} = {}) {
    const age = yearsAfter([30, 1, 2, 3, 4, 5, 6, 7, 8, 9]);
    throw "do not open until birthday" {
        guest: guest,
        age: age,
        tokens: [happy, anniversary, ofWord, becoming, string(age) + bang]
    };
}

const label = "sparkle";
return openEnvelope(function () {
    partyMail({
        "guest of honor": "Jeremy",
        wish: ["Happy", "Anniversary"],
        bridge: { of: "of", becoming: "becoming" },
        [label]: "!"
    });
});
