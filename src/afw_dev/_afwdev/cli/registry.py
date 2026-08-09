#! /usr/bin/env python3
##
# @file registry.py
# @ingroup afwdev_cli
# @brief Explicit subcommand handler and info maps (no globals() dispatch).
# @details SUBCOMMAND_HANDLERS maps each subcommand name to its callable.
#          validate_registry() runs at import and requires the same set of
#          names as _subcommand_infos in info.py.
#

from _afwdev.cli.info import _subcommand_infos, afwdev_info
from _afwdev.cli.settings import get_afwdev_settings
from _afwdev.cli.handlers import generate as generate_handlers
from _afwdev.cli.handlers import misc as misc_handlers
from _afwdev.cli.handlers import test_validate as test_validate_handlers
from _afwdev.scaffold.handlers import add as add_handlers
from _afwdev.scaffold.handlers import make as make_handlers


def _make_afw_package(args, options):
    make_handlers.subcommand_make_afw_package(
        args, options, get_afwdev_settings)


SUBCOMMAND_INFO_BY_NAME = {
    info['subcommand']: info for info in _subcommand_infos
}

SUBCOMMAND_HANDLERS = {
    'add-adapter-type': add_handlers.subcommand_add_adapter_type,
    'add-content-type': add_handlers.subcommand_add_content_type,
    'add-core-interface': add_handlers.subcommand_add_core_interface,
    'add-log-type': add_handlers.subcommand_add_log_type,
    'afwdev-parser-info': misc_handlers.subcommand_afwdev_parser_info,
    'build': misc_handlers.subcommand_build,
    'ebnf': misc_handlers.subcommand_ebnf,
    'for': misc_handlers.subcommand_for,
    'generate': generate_handlers.subcommand_generate,
    'make-afw-package': _make_afw_package,
    'make-command': make_handlers.subcommand_make_command,
    'make-extension': make_handlers.subcommand_make_extension,
    'settings': misc_handlers.subcommand_settings,
    'task': misc_handlers.subcommand_task,
    'test': test_validate_handlers.subcommand_test,
    'blast': test_validate_handlers.subcommand_blast,
    'validate': test_validate_handlers.subcommand_validate,
}


def validate_registry():
    """Fail fast if metadata and handlers disagree."""
    missing = []
    for info in _subcommand_infos:
        name = info['subcommand']
        if name not in SUBCOMMAND_HANDLERS:
            missing.append(name)
    extra = sorted(set(SUBCOMMAND_HANDLERS) - set(SUBCOMMAND_INFO_BY_NAME))
    if missing or extra:
        parts = []
        if missing:
            parts.append('missing handlers: ' + ', '.join(missing))
        if extra:
            parts.append('extra handlers: ' + ', '.join(extra))
        raise RuntimeError('afwdev subcommand registry mismatch: ' + '; '.join(parts))


validate_registry()

# Re-export for convenience
__all__ = [
    'SUBCOMMAND_HANDLERS',
    'SUBCOMMAND_INFO_BY_NAME',
    'afwdev_info',
    'validate_registry',
]
