#! /usr/bin/env python3
##
# @file add.py
# @ingroup afwdev_scaffold
# @brief Handlers for add-adapter-type, add-content-type, add-log-type,
#        add-core-interface.
# @details Shared steps are in add_registry_type; these handlers supply the
#          interface list, conf skeleton, and registry_type for each variant.
#

from _afwdev.common import msg, nfc, package
from _afwdev.scaffold.common import msg_added_files
from _afwdev.scaffold.interface import add_interface
from _afwdev.scaffold import add_registry_type as art


##
# @brief Subcommand add-adapter-type
#
def subcommand_add_adapter_type(args, options):
    art.begin_add_to_srcdir(options, 'adapter type')
    art.add_interfaces(options, [
        'afw_adapter_factory',
        'afw_adapter',
        'afw_adapter_session',
    ])
    art.add_conf_object(
        options,
        '_AdaptiveConf_adapter.json',
        '_AdaptiveConf_adapter_' + options['implementation_id'] + '.json')
    art.finish_registry_type(
        options,
        'adapter_type',
        '&' + options['prefix'] + 'adapter_factory',
        'adapter type')


##
# @brief Subcommand add-content-type
#
def subcommand_add_content_type(args, options):
    art.begin_add_to_srcdir(options, 'content type')
    art.add_interfaces(options, ['afw_content_type'])
    art.finish_registry_type(
        options,
        'content_type',
        '&' + options['prefix'] + 'content_type',
        'content type')


##
# @brief Subcommand add-core-interface
#
def subcommand_add_core_interface(args, options):
    package.set_options_from_existing_package_srcdir(options, options['srcdir'])
    msg.highlighted_info(
        "Adding interface " + options['interface_name'] + ' to ' +
        options['srcdir_path'], empty_before=True)
    options['added_files'] = []
    add_interface(options)
    msg.success(
        'Skeleton interface ' + options['interface_name'] +
        ' added to ' + options['srcdir_relpath'] + '.')
    msg_added_files(options)


##
# @brief Subcommand add-log-type
#
def subcommand_add_log_type(args, options):
    art.begin_add_to_srcdir(options, 'log type')
    if options.get('verbose'):
        print("\nadd-log-type options:\n")
        print(nfc.json_dumps(options, sort_keys=True, indent=4))
    art.add_interfaces(options, ['afw_log_factory', 'afw_log'])
    art.add_conf_object(
        options,
        '_AdaptiveConf_log.json',
        '_AdaptiveConf_log_' + options['implementation_id'] + '.json')
    art.finish_registry_type(
        options,
        'log_type',
        '&' + options['prefix'] + 'log_factory',
        'log type')
