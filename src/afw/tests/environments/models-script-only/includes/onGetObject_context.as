#!/usr/bin/env afw

/*
 * When mappedAdapterId is omitted from model conf, current::mappedAdapterId
 * must be nullish / not a configured backend id (issue #109).
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

return {
    "mappedAbsent": true
};
