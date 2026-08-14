Runtime model (short) {#afw_dev_runtime}
=====================

@brief Pools, values, xctx, and environment — pointers into group docs.

This page is a **map**, not a full essay. Prefer the Doxygen module groups for
invariants and details.

## Pools

Hierarchical allocation; bulk release; subpools for scopes.  
See group @ref afw_pool.

## Values

Public API type is always **`const afw_value_t *`**. That is a typedef/handle:
**many different C structs** implement values (string/managed/slice, block,
call, symbol, …). Use evaluate/create APIs; do not assume one struct body.
Lifetimes: permanent / managed / managed_slice / unmanaged.  
See group @ref afw_value and @ref afw_dev_types_opaques.

## Execution context (xctx)

Unit of work: scope stack, evaluation, statement_flow, qualifiers, pool.  
See group @ref afw_xctx.

## Environment

Process-wide registries (`xctx->env`). Core registers at create; extensions
and hosts register more (often via manifests). After load, peers share the
same lookup APIs. Bootstrap order matters (functions before operator prepare).  
See group @ref afw_environment.

## Compile / evaluate

Script, template, expression, JSON, … → value graphs → evaluate.  
See groups @ref afw_compile and Adaptive Functions under the public C API.

Most extension authors only **call** compile/evaluate APIs. People changing
the **language grammar** should read @ref afw_dev_compiler_ebnf (EBNF-in-comments
harvest into `generated/ebnf/`).

## Related developer pages

- @ref afw_dev_overview  
- @ref afw_dev_interfaces  
