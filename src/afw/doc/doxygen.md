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

- **[Modules](modules.html)** — start here: pools, values, environment,
  adapters, interfaces, … Group pages hold mental models and invariants.
- **[Related Pages](pages.html)** — short developer essays (interfaces, runtime,
  opaques, …) also under `src/afw/doc/developer/` in the repo.
- **[Data Structures](annotated.html)** — C types by public `afw_*_t` name when
  possible; see @ref afw_dev_types_opaques for typedef vs multi-layout values.
- **Interface call macros** — day-to-day C API (`afw_<interface>_<method>(…)`);
  find them under Modules → each interface group (XML descriptions; link to
  instance `*_t` via `@see`).
- **Implementing interfaces** — afwdev `make-*` / `add-*` and closet skeletons;
  leave `@todo` markers until filled in.
- **Hand headers** — module helpers beyond generated interface macros.

## Developer reading order

1. @ref afw_dev_overview — audience and map  
2. @ref afw_dev_interfaces — macros and generation  
3. @ref afw_dev_implementing — scaffolds and `@todo`  
4. @ref afw_dev_runtime — pools, values, xctx, environment  
5. @ref afw_dev_extending — extension sketch  
6. @ref afw_dev_types_opaques — public `afw_*_t` vs multi-layout values  
7. @ref afw_dev_compiler_ebnf — grammar / EBNF harvest (compiler only)  
8. @ref afw_dev_doxygen_skin — HTML skin (maintainers)  

These pages also live under `src/afw/doc/developer/` in the repository for
direct reading without opening HTML.

## Source Code

The source for Adaptive Framework is located in a
[GitHub repository](https://github.com/afw-org/afw).

Public development targets the `develop` branch (then `main` when ready for
wider use). Staging and experimental work may use other long-lived branches.

## Building and Running

Refer to [Full Documentation](https://afw-org.github.io/afw) for an overview
of Adaptive Framework, including how to get started.

## Contributing

If you are interested in contributing to Adaptive Framework, please see
`CONTRIBUTING.md` in the repository root (also copied next to this HTML as
`CONTRIBUTING.md` when docs are built).

## License

Adaptive Framework is licensed under the MIT license (`LICENSE` in the
repository root).