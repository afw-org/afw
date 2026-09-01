# C probes (`run_c_probe`)

**Audience:** maintainers / assistants.  
**Issue:** [#207](https://github.com/afw-org/afw/issues/207).  
**Not** [#206](https://github.com/afw-org/afw/issues/206) (named utf8 doors / ignore-case offsets / error-object backtrace).

**Origin:** Grok (xAI) designed `run_c_probe()` for #207 in August 2026, pairing on AFW. The helper, the valgrind wrap, and the decision not to skip backtrace on a throw are that sitting. Signed in `_afwdev/test/c_probe.py` (`who()`).

## When

Prefer an Adaptive test script. Use a C probe only when script cannot reach the hole (wrapping `size_t`, a second interface impl literals never use, hand-set `afw_utf8_t`, helpers that need a live directory to go through the adapter).

The `*_probe.c` is **not** a cmake test target and is **not** part of `libafw`. It compiles against the **installed** library at test time.

## Helper

`_afwdev.test.c_probe.run_c_probe()` is the one compile-and-run path. Probe groups keep a thin Python `run()` that lists named cases. Do not copy a `cc -O0 -g -I … -lafw -rpath` blob.

Contract: `argv[1]` is the case name; exit 0 is pass.

```python
from _afwdev.test.c_probe import run_c_probe

def run():
    return run_c_probe(
        "pool_alloc_probe.c",
        "Pool malloc rejects a wrapping size",
        [
            ("overflow", "pool malloc of SIZE_MAX throws memory"),
        ],
    )
```

Extra DSOs: `libraries=("afwldap", "afw")`. Paths: `AFW_INCLUDE_DIR` / `AFW_LIB_DIR`, else `/usr/local/include/afw` and `/usr/local/lib/afw`. Extra `-I` in `extra_cflags` is searched **before** that install include dir so a leftover core `*_internal.h` in the prefix cannot win over a source-tree internal header. `pool_heap` relies on this. `--cdev` / `--fulldev` both `--install` and then prune leftover internals, denylist headers, and old generated names from that include dir.

Start with `afwdev prime-test-c-probe <path>` (cwd-relative or absolute, must be a leaf under `tests/` or `tests-extra/`). Closet source: [`src/afw_dev/_resources/closet/c_probe/`](../src/afw_dev/_resources/closet/c_probe/). Fill the marked blocks.

Live examples: `src/afw/tests/advanced/pool_alloc/` (SIZE_MAX overflow), `pool_heap/` (heap + heap-tracker impls; extra `-I` to `src/afw/pool` for the internal header), `array_view_index/`, `associative_array_set/`, `utf8_icu_bound/`, `utf8_named_doors/`, `src/afw_ldap/tests/ldap_filter/`. Helper self-tests: `src/afw_dev/tests/c_probe/`. Scaffold tests: `src/afw_dev/tests/prime_test_c_probe/`.

## Valgrind / libunwind

`afwdev test --env-mode valgrind` still wraps `.as` via the `afw` CLI. The same mode wraps C probes: python mode pushes a run context; the helper runs each case under valgrind with `valgrind.suppress`.

A throw calls `afw_os_backtrace` (except memory errors). libunwind then trips Memcheck `Param` `write(buf)` / `msync(start)` under `_ULx86_64_step`. That is **not** the hole under test. The suite suppressions cover it, including portable `...` / `_ULx86_64_step` blocks so a new `.so` path does not reopen the noise.

Standalone valgrind on the compiled binary, without that file, can still report the noise. Judge the probe by exit code and by the helper wrap.

**Decided not:** skip backtrace on every probe throw (`AFW_NO_BACKTRACE` or similar). That would hide the throw path from Memcheck. Error-object `backtrace` is `forced_safe` then NFC ([#206](https://github.com/afw-org/afw/issues/206)).

Python-mode files are loaded under a unique module name (not a shared `test`). Two `.py` files in one process no longer inherit `run()` from each other.

`passed` / `skip` are coerced to bool in `parse_test_run` so a leftover truthy string cannot print as fail and count as pass. A response with no `tests` list is a failure, not a crash. A real `--test-pattern` shows passing cases.

## Docs

Handbook Developer Guide **Writing Tests** (C Probes). Gate note: `src/afw/tests/README.md`. Day rule: `afw-tests`. Playbook: `agent-support.md`. Command: `afwdev prime-test-c-probe`. Closet: `src/afw_dev/_resources/closet/c_probe/`. Naming of the other `add-*` / `make-*` commands is [#210](https://github.com/afw-org/afw/issues/210).
