#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Invented zz__ labels: short sanitize, long content hash, no srcdir counter.
"""

from __future__ import print_function

import hashlib
import sys

from _afwdev.generate.strings import get_string_label


def _opts(prefix):
    return {
        'prefix': prefix,
        'const': {'string': {}},
    }


def _stem(symbol, prefix_q):
    # AFW_Q_<label> / AFW_LDAP_Q_<label>
    head = prefix_q.upper() + 'Q_'
    if not symbol.startswith(head):
        return None
    return symbol[len(head):]


def _case(name, description, passed, error=None):
    return {
        'test': name,
        'description': description,
        'passed': bool(passed),
        'skip': False,
        'error': error,
    }


def run():
    tests = []
    description = 'Invented zz__ string labels are content-stable'

    opts = _opts('afw_')
    short = get_string_label(opts, 'Hello World!', 'Q')
    tests.append(_case(
        'short-sanitize',
        'short non-ident keeps sanitized zz__ spelling',
        _stem(short, 'afw_') == 'zz__Hello_World_',
        None if _stem(short, 'afw_') == 'zz__Hello_World_' else short,
    ))

    long_a = 'This is the object type for the optional adapterTypeSpecific ' \
        'parameter of retrieve_objects() when the adapterType is ldap.'
    expect = 'zz__' + hashlib.sha256(long_a.encode('utf-8')).hexdigest()[:12]
    core = get_string_label(opts, long_a, 'Q')
    tests.append(_case(
        'long-hash',
        'long non-ident is zz__ plus sha256 of the value',
        _stem(core, 'afw_') == expect,
        None if _stem(core, 'afw_') == expect else core,
    ))

    again = get_string_label(opts, long_a, 'Q')
    tests.append(_case(
        'reuse-same-bag',
        'same value in one generate reuses the label',
        again == core,
        None if again == core else again,
    ))

    other = _opts('afw_ldap_')
    ldap = get_string_label(other, long_a, 'Q')
    tests.append(_case(
        'same-stem-other-srcdir',
        'another srcdir mints the same zz__ stem for the same text',
        _stem(ldap, 'afw_ldap_') == expect,
        None if _stem(ldap, 'afw_ldap_') == expect else ldap,
    ))

    long_b = long_a + ' x'
    other_label = get_string_label(_opts('afw_'), long_b, 'Q')
    tests.append(_case(
        'different-text-different-hash',
        'a different long value gets a different hash',
        _stem(other_label, 'afw_') != expect,
        None if _stem(other_label, 'afw_') != expect else other_label,
    ))

    return {
        'description': description,
        'tests': tests,
    }


if __name__ == '__main__':
    # Allow `python3 generate_string_labels.py` from a source checkout.
    result = run()
    bad = [t for t in result['tests'] if not t['passed']]
    for t in result['tests']:
        sys.stderr.write('%s %s\n' % (
            'ok' if t['passed'] else 'FAIL', t['test']))
        if t.get('error'):
            sys.stderr.write('  %s\n' % t['error'])
    sys.exit(1 if bad else 0)
