Adaptive Framework C API Documentation {#mainpage}
======================================

## Who this documentation is for

This Doxygen tree is aimed at **people who build on Adaptive Framework in C**:
core developers, **extension and command authors**, interface implementers,
and hosts such as `afw` / `afwfcgi`.

It is **not** the primary guide for Adaptive Script application authors or
REST-only users. For product overview, language, and getting started, see the
[Full Documentation](https://afw-org.github.io/afw) handbook.

## What is Adaptive Framework?

Adaptive Framework is a set of interfaces for developing servers, requests,
object access, and related services in an adaptive way, plus implementations,
language bindings, commands, and administrative applications. The runtime is
**C**; contracts are defined in interface XML and consumed largely as
**generated call macros** (not raw vtable arrow syntax).

## How to navigate this tree

- **Modules / groups** — start with the group list (pools, values, environment,
  adapters, interfaces, …). Group pages hold mental models and invariants.
- **Interface call macros** — the day-to-day C API for calling methods
  (`afw_<interface>_<method>(…)`). Descriptions come from interface XML.
- **Implementing interfaces** — use afwdev `make-*` / `add-*` subcommands and
  the generated closet skeletons; leave `@todo` markers until filled in.
- **Hand headers** — public module headers document related files and helpers
  beyond the generated interface macros.

## Developer reading order

1. @ref afw_dev_overview — audience and map  
2. @ref afw_dev_interfaces — macros and generation  
3. @ref afw_dev_implementing — scaffolds and `@todo`  
4. @ref afw_dev_runtime — pools, values, xctx, environment  
5. @ref afw_dev_extending — extension sketch  
6. @ref afw_dev_compiler_ebnf — grammar / EBNF harvest (compiler maintainers only)  

These pages also live under `src/afw/doc/developer/` in the repository for
direct reading without opening HTML.

**Modules (groups):** open the Modules list for topic “essays” (pools, values,
interfaces, adapters, …) authored mainly in `afw_doxygen.h`.

## Source Code

The source for Adaptive Framework is located in a
[GitHub repository](https://github.com/afw-org/afw).

Development is currently being done in the `develop` branch, but will be
merged into `main` once it's in a state appropriate for public use.

## Building and Running

Refer to [Full Documentation](https://afw-org.github.io/afw) for an overview
of Adaptive Framework, including how to get started.

## Contributing

If you are interested in contributing to Adaptive Framework, please see our
[contributing](./CONTRIBUTING.md) page.

## License

Adaptive Framework is licensed under the [MIT](./LICENSE) license.