#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: happy_anniversary_29.as
//? customPurpose: Part of miscellaneous category tests
//? description: Easter egg Rube Goldberg using param/catch Patterns, call-site spread, string/computed keys (issues #140 / #38). Birthday card for Jeremy (age 29 forever). Source shebang uses JeremyScript easter egg.
//? sourceType: script
//?
//? test: happy-anniversary-of-becoming-29
//? description: Overengineered wish with party emotes and an egg
//? expect: "🎉 Happy Anniversary of becoming 29! \u{1F95A}🥳🎊"
//? source: ...
#! JeremyScript

/* Rube Goldberg anniversary machine.
 *
 * Uses: array + object Patterns on formals, nested catch Pattern,
 * string / computed property keys, Expression defaults, rest formals,
 * call-site f(...arr) spread, for-of, throw data, assert.
 * Age 29 is not a literal — it is 20 plus nine rest candles. Obviously.
 * Shebang: JeremyScript easter egg (accepted in place of afw).
 * Finale: party popper, egg, partying face, confetti (because easter egg).
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
    assert(age === 29);
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
    const age = yearsAfter([20, 1, 2, 3, 4, 5, 6, 7, 8, 9]);
    throw "do not open until birthday" {
        guest: guest,
        age: age,
        /* party popper, partying face, confetti; egg hides as \u{1F95A} */
        tokens: ["🎉", happy, anniversary, ofWord, becoming,
            string(age) + bang, "\u{1F95A}🥳🎊"]
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
