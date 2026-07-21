Runtime model (short) {#afw_dev_runtime}
=====================

@brief Pools, values, xctx, and environment — pointers into group docs.

This page is a **map**, not a full essay. Prefer the Doxygen module groups for
invariants and details.

## Pools

Hierarchical allocation; bulk release; subpools for scopes.  
See group @ref afw_pool.

## Values

`afw_value_t` = `inf` + payload. Compiled graphs, data-type values, calls,
closures, etc. Lifetimes: permanent / managed / managed_slice / unmanaged.  
See group @ref afw_value.

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

## Related developer pages

- @ref afw_dev_overview  
- @ref afw_dev_interfaces  
