#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: index_or_merge_cardinality.as
//? customPurpose: Part of lmdb tests
//? description: A genuine OR across two indexed properties builds two adapter-index cursors and merges them (afw_adapter_impl_index_cursor_list_merge). Issue #296. Two distinct, currently-FAILING defects, both in that function -- these tests are written to the correct/desired behavior and are expected to fail until the fix lands.
//? sourceType: script
//?
//? test: index_or_merge_drops_a_cursor_on_cardinality_tie_or_loss
//? description: Issue #296 (currently FAILS, even with eq/eq): afw_adapter_impl_index_cursor_list_merge only pushes this_cursor into the merged list when its cardinality is strictly greater than a that_list entry it's compared against. When it never wins a single comparison (e.g. a tie), it is silently dropped from the merged list entirely -- that whole clause's matches vanish from the OR result, with no error.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexOrMergeDropType";

// Distinct keys per test in this file (not just distinct object types) --
// on this branch these tests are expected to fail, and a failed assert
// skips this block's own index_remove cleanup below, so a later block
// must not be able to inherit a stale definition under a reused key.
index_create("lmdb", "kind_drop", undefined, [ot], undefined, undefined, false, false);
index_create("lmdb", "status_drop", undefined, [ot], undefined, undefined, false, false);

// Both indexed values are unique (cardinality 1 each) -- a tie, which is
// enough to trigger the drop: this_cardinality(1) is never > that_cardinality(1).
add_object("lmdb", ot, { kind_drop: "tool", status_drop: "active" }, generate_uuid());
add_object("lmdb", ot, { kind_drop: "toy", status_drop: "inactive" }, generate_uuid());
add_object("lmdb", ot, { kind_drop: "other", status_drop: "other" }, generate_uuid());

const kindOrStatus: array = retrieve_objects("lmdb", ot, { "filter": {
    "op": "or",
    "filters": [
        { "op": "eq", "property": "kind_drop", "value": "tool" },
        { "op": "eq", "property": "status_drop", "value": "inactive" }
    ]
}});
assert(length(kindOrStatus) === 2,
    "kind_drop=tool OR status_drop=inactive should return exactly the tool and inactive objects, not just one of them");

safe_evaluate(index_remove("lmdb", "kind_drop"), null);
safe_evaluate(index_remove("lmdb", "status_drop"), null);

return 0;


//? test: index_or_range_and_eq_across_two_indexed_properties
//? description: Issue #296 (currently FAILS): OR'ing a range clause (gt) on one indexed property with an eq clause on a different indexed property should return the union, but afw_adapter_impl_index_cursor_get_count() (called from cursor_list_merge) doesn't support gt, so the query throws instead.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexOrRangeEqType";

index_create("lmdb", "age_re", undefined, [ot], undefined, undefined, false, false);
index_create("lmdb", "status_re", undefined, [ot], undefined, undefined, false, false);

add_object("lmdb", ot, { age_re: 20, status_re: "active" }, generate_uuid());   // matches gt(age_re,9) only
add_object("lmdb", ot, { age_re: 5, status_re: "inactive" }, generate_uuid()); // matches eq(status_re,inactive) only
add_object("lmdb", ot, { age_re: 3, status_re: "active" }, generate_uuid());   // matches neither

const ageOrStatus: array = retrieve_objects("lmdb", ot, { "filter": {
    "op": "or",
    "filters": [
        { "op": "gt", "property": "age_re", "value": 9 },
        { "op": "eq", "property": "status_re", "value": "inactive" }
    ]
}});
assert(length(ageOrStatus) === 2,
    "age_re>9 OR status_re=inactive should return exactly the 2 objects that satisfy either clause");

safe_evaluate(index_remove("lmdb", "age_re"), null);
safe_evaluate(index_remove("lmdb", "status_re"), null);

return 0;


//? test: index_or_two_range_ops_across_two_indexed_properties
//? description: Issue #296 (currently FAILS): OR'ing two range clauses (gt, lt) on two different indexed properties should return the union with no duplicates, but afw_adapter_impl_index_cursor_get_count() doesn't support gt/lt, so the query throws instead.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

const ot: string = "TestIndexOrRangeRangeType";

index_create("lmdb", "age_rr", undefined, [ot], undefined, undefined, false, false);
index_create("lmdb", "score_rr", undefined, [ot], undefined, undefined, false, false);

add_object("lmdb", ot, { age_rr: 20, score_rr: 50 }, generate_uuid()); // matches gt(age_rr,9) only
add_object("lmdb", ot, { age_rr: 5, score_rr: 3 }, generate_uuid());   // matches lt(score_rr,10) only
add_object("lmdb", ot, { age_rr: 5, score_rr: 50 }, generate_uuid());  // matches neither
add_object("lmdb", ot, { age_rr: 20, score_rr: 3 }, generate_uuid()); // matches both -- must appear exactly once

const ageOrScore: array = retrieve_objects("lmdb", ot, { "filter": {
    "op": "or",
    "filters": [
        { "op": "gt", "property": "age_rr", "value": 9 },
        { "op": "lt", "property": "score_rr", "value": 10 }
    ]
}});
assert(length(ageOrScore) === 3,
    "age_rr>9 OR score_rr<10 should return exactly the 3 objects that satisfy either clause, deduped");

safe_evaluate(index_remove("lmdb", "age_rr"), null);
safe_evaluate(index_remove("lmdb", "score_rr"), null);

return 0;
