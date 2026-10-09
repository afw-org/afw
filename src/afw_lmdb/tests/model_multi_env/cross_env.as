#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: cross_env.as
//? customPurpose: Part of lmdb tests
//? description: ...
A model mapped to lmdbA whose on* scripts also read and write lmdbB, a
second LMDB environment in the same process. config.py seeds OrderRow
s1 and s2 in lmdbA and Audit s1-add in lmdbB. All cases share one afw
process, so each adapter's transaction stays open across cases;
commit and abort at request end are covered by
../model-multi-env-fcgi/.
//? sourceType: script
//?
//? test: add_writes_both_envs
//? description: Adding an Order writes its OrderRow to lmdbA (default processing) and its Audit to lmdbB (onAddObject).
//? skip: false
//? expect: 0
//? source: ...

add_object('model', 'Order', {item: 'widget'}, 'o1');

const row: object = get_object('lmdbA', 'OrderRow', 'o1');
assert(row.item === 'widget', "OrderRow o1 not in lmdbA");

const audit: object = get_object('lmdbB', 'Audit', 'o1-add');
assert(audit.orderId === 'o1' && audit.item === 'widget',
    "Audit o1-add not in lmdbB");

return 0;

//?
//? test: no_leak_between_envs
//? description: Each environment holds only its own object types.
//? skip: false
//? expect: 0
//? source: ...

assert(length(retrieve_objects('lmdbA', 'Audit')) === 0,
    "lmdbA has Audit objects");
assert(length(retrieve_objects('lmdbB', 'OrderRow')) === 0,
    "lmdbB has OrderRow objects");
assert(length(retrieve_objects('lmdbA', 'OrderRow')) === 3,
    "lmdbA should have s1, s2 and o1");
assert(length(retrieve_objects('lmdbB', 'Audit')) === 2,
    "lmdbB should have s1-add and o1-add");

return 0;

//?
//? test: read_other_env_during_retrieve
//? description: Order.auditCount reads lmdbB while the model's retrieve of lmdbA is still open.
//? skip: false
//? expect: 0
//? source: ...

const orders: array = retrieve_objects('model', 'Order');
assert(length(orders) === 3, "expected 3 orders, got " + string(length(orders)));

for (const order: object of orders) {
    if (order.item === 's2') {
        assert(order.auditCount === 0, "s2 auditCount " + string(order.auditCount));
    } else {
        assert(order.auditCount === 1,
            order.item + " auditCount " + string(order.auditCount));
    }
}

return 0;

//?
//? test: delete_removes_from_both_envs
//? description: Deleting an Order removes its Audit from lmdbB (onDeleteObject) and its OrderRow from lmdbA (default processing).
//? skip: false
//? expect: 0
//? source: ...

delete_object('model', 'Order', 'o1');

assert(length(retrieve_objects('lmdbB', 'Audit',
    {filter: {op: 'eq', property: 'orderId', value: 'o1'}})) === 0,
    "Audit o1-add still in lmdbB");
assert(length(retrieve_objects('lmdbA', 'OrderRow')) === 2,
    "lmdbA should have s1 and s2 left");
assert(length(retrieve_objects('lmdbB', 'Audit')) === 1,
    "lmdbB should have s1-add left");

return 0;

//?
//? test: script_only_type_in_other_env
//? description: Note has no mappedObjectType; its on* scripts keep it in lmdbB although the model's mappedAdapterId is lmdbA.
//? skip: false
//? expect: 0
//? source: ...

add_object('model', 'Note', {text: 'hello'}, 'n1');
add_object('model', 'Note', {text: 'bye'}, 'n2');
modify_object('model', 'Note', 'n1', [['set_property', 'text', 'hi']]);

assert(get_object('model', 'Note', 'n1').text === 'hi', "modify of n1 lost");
assert(length(retrieve_objects('model', 'Note')) === 2, "expected 2 notes");

delete_object('model', 'Note', 'n2');

assert(length(retrieve_objects('lmdbB', 'NoteRow')) === 1,
    "lmdbB should have n1 left");
assert(length(retrieve_objects('lmdbA', 'NoteRow')) === 0,
    "lmdbA has NoteRow objects");

return 0;

//?
//? test: write_same_env_from_hook
//? description: ...
OrderSelfNote's onAddObject writes an OrderNote to lmdbA, the model's
mapped adapter, inside the model's own lmdbA write; default processing
then writes the OrderRow. Same adapter id, same session: the hook's
write reuses the request's transaction and each add_object nests its
own child transaction, so neither waits for the other.
//? skip: false
//? expect: 0
//? source: ...

add_object('model', 'OrderSelfNote', {item: 'self'}, 'n1');

assert(get_object('lmdbA', 'OrderRow', 'n1').item === 'self', "OrderRow n1 not in lmdbA");
assert(get_object('lmdbA', 'OrderNote', 'n1-note').orderId === 'n1',
    "OrderNote n1-note not in lmdbA");
assert(get_object('model', 'OrderSelfNote', 'n1').noteCount === 1, "noteCount");

delete_object('model', 'OrderSelfNote', 'n1');

assert(length(retrieve_objects('lmdbA', 'OrderNote')) === 0, "OrderNote n1-note left");
assert(length(retrieve_objects('lmdbA', 'OrderRow',
    {filter: {op: 'eq', property: 'item', value: 'self'}})) === 0, "OrderRow n1 left");

return 0;
