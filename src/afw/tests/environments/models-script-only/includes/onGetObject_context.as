#!/usr/bin/env afw

/*
 * When mappedAdapterId is omitted from model conf, current::mappedAdapterId
 * must be nullish / not a configured backend id (issue #109).
 *
 * Also prove issue #9 qualifier("current") snapshot for object-level onGetObject.
 */
assert(variable_exists("current::adapterId"));
assert(current::adapterId == "scriptModel");

/* Property may exist as undefined depending on runtime accessor; never a backend id. */
if (variable_exists("current::mappedAdapterId")) {
    assert(
        current::mappedAdapterId === undefined ||
        current::mappedAdapterId === null,
        "current::mappedAdapterId should be nullish when conf omits mappedAdapterId"
    );
}

/* issue #9 — snapshot of current:: during object-level hook */
const snap = qualifier("current");
assert(!is_nullish(snap), "qualifier(\"current\") should be an object during onGetObject");
assert(snap.adapterId === current::adapterId,
    "snapshot adapterId should match current::adapterId");
assert(!is_nullish(snap.objectType) || variable_exists("current::objectType"),
    "snapshot or current:: should expose objectType");
if (variable_exists("current::objectType") && !is_nullish(current::objectType)) {
    assert(snap.objectType === current::objectType,
        "snapshot objectType should match current::objectType");
}

return {
    "mappedAbsent": true,
    "snapshotOk": true
};
