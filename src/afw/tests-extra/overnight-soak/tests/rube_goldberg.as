/* Rube Goldberg: Patterns, rest formals, call-site spread, computed keys.
 * Same machine as tests/miscellaneous/happy_anniversary_29.as. */
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
    throw "do not open until birthday" data {
        guest: guest,
        age: age,
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
