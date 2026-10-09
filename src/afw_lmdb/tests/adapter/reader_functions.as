#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: reader_functions.as
//? customPurpose: Part of lmdb tests
//? description: ...
reader_check() and reader_list(), the LMDB Adaptive functions over
LMDB's reader lock table (mdb_reader_check, mdb_reader_list). All cases
share one afw process, so the case with no readers runs first. With a
reader open: ../reader-functions-fcgi/.
//? sourceType: script
//?
//? test: reader_list_no_readers
//? description: With no read transaction open, reader_list() is LMDB's "(no active readers)" line.
//? skip: false
//? expect: "(no active readers)\n"
//? source: ...

return reader_list("lmdb");

//?
//? test: reader_check_nothing_stale
//? description: reader_check() returns how many stale reader slots it cleared: none in a fresh process.
//? skip: false
//? expect: 0
//? source: ...

return reader_check("lmdb");

//?
//? test: reader_functions_not_lmdb
//? description: reader_check() and reader_list() on an adapter that is not LMDB throw.
//? skip: true
//? skipReason: ...
FIXME (beta-backlog.md): both cast any adapter to afw_lmdb_adapter_t and
pass its memory to LMDB as an MDB_env; on the runtime adapter ("afw")
the process dies with SIGSEGV.
//? expect: true
//? source: ...

let thrown: integer = 0;
try { reader_check("afw"); }
catch (e) {
    assert(includes<string>(e.message, "is not an LMDB adapter"), e.message);
    thrown = thrown + 1;
}
try { reader_list("afw"); }
catch (e) {
    assert(includes<string>(e.message, "is not an LMDB adapter"), e.message);
    thrown = thrown + 1;
}
assert(thrown === 2, "expected both to throw");
return true;
