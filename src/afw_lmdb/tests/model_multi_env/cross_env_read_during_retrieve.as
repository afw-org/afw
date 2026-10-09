#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: cross_env_read_during_retrieve.as
//? customPurpose: Part of lmdb tests
//? description: Read lmdbB from a model property while a read-only retrieve of lmdbA is open.
//? sourceType: script
//?
//? test: read_other_env_during_read_only_retrieve
//? description: ...
Nothing in this afw process has written yet, so the model's retrieve
holds a read transaction on lmdbA (not a reused write transaction, as
in cross_env.as) while Order.auditCount begins one on lmdbB. The
cross-environment version of ../model_adapter/nested_retrieve_during_dump.as.
//? skip: false
//? expect: 0
//? source: ...

const orders: array = retrieve_objects('model', 'Order');
assert(length(orders) === 2, "expected 2 orders, got " + string(length(orders)));

for (const order: object of orders) {
    const expected: integer = order.item === 's1' ? 1 : 0;
    assert(order.auditCount === expected,
        order.item + " auditCount " + string(order.auditCount));
}

return 0;
