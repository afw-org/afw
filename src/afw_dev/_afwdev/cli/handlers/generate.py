#! /usr/bin/env python3
##
# @defgroup afwdev_generate generate
# @ingroup afwdev
# @brief The afwdev generate subcommand
# @details The generate subcommand is used to generate different parts of
#          Adaptive Framework. This includes generated source code, generated
#          diagrams, language bindings, documentation and schema definitions.
#

##
# @file generate.py
# @ingroup afwdev_cli
#

import os
import fnmatch
from _afwdev.common import msg, package
from _afwdev.generate import generate
from _afwdev.scaffold.generate_hooks import call_generated_generate


def subcommand_generate(args, options):
    afw_package = package.get_afw_package(options)

    # The .replace('\\','') is a hack needed because visual code launch args:
    #
    # "args": ["generate", "*"],
    #
    # causes args.SRCDIR_PATTERN to have a list of all files in directory
    #
    # and
    #
    # "args": ["generate", "\\*"],
    #
    # causes args.SRCDIR_PATTERN to have '\\*' on at lease some some
    # system/python levels.
    #
    options['srcdir_pattern'] = options['srcdir_pattern'].replace('\\', '')
    options['subcommand'] = "generate"

    matched = 0
    generated = 0
    skipped_build_type = 0
    skipped_no_generate = 0

    for srcdir in afw_package['srcdirs']:
        if not fnmatch.fnmatch(srcdir, options['srcdir_pattern']):
            continue
        matched += 1
        package.set_options_from_existing_package_srcdir(
            options, srcdir, set_all=True)
        if options.get('buildType') != 'afwmake':
            skipped_build_type += 1
            msg.info('Skipping ' + srcdir + ' (buildType is not afwmake)')
            continue
        if not os.path.exists(options['srcdir_path'] + 'generate'):
            skipped_no_generate += 1
            msg.info('Skipping ' + srcdir + ' (no generate/ directory)')
            continue
        msg.info("Generating " + srcdir, empty_before=True)
        call_generated_generate(options)
        generated += 1
        msg.success('Generate ' + srcdir + ' successful')

    generate.root_generate(options)

    msg.highlighted_info(
        'Generate summary: ' +
        str(generated) + ' generated, ' +
        str(skipped_build_type) + ' skipped (buildType), ' +
        str(skipped_no_generate) + ' skipped (no generate/), ' +
        str(matched) + ' matched pattern')
    if matched == 0:
        msg.warn(
            'No srcdirs matched --srcdir-pattern ' +
            repr(options['srcdir_pattern']))
