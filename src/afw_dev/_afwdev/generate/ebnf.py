#! /usr/bin/env python3

##
# @file ebnf.py
# @ingroup afwdev_generate
# @brief This file contains the functions used to maintain EBNF integration
#        with associated C source.
#

import os
import glob
import fnmatch
from _afwdev.common import msg, nfc

def generate_ebnf_scan(path, out_fd):

    line_number = 0
    indent = 0
    msg.info('Reading in ' + os.path.basename(path))
    with nfc.open(path, 'r') as in_fd:
        while True:
            line = in_fd.readline()
            line_number += 1
            if not line:
                break
            if 'ebnf>>>' in line:
                line = ' From ' + os.path.basename(path)
                padding = ' '
                if len(line) < 75:
                    padding = ' ' * (76 - len(line))
                out_fd.write('\n/*' + line + padding + '*/\n')
                while True:
                    line = in_fd.readline()
                    line_number += 1
                    if '<<<ebnf' in line:
                        break
                    if not line:
                        msg.warn(os.path.basename(path) + ' contains ebnf>>> without corresponding <<<ebnf' )
                        break
                    if '*#' in line:
                        line = line[line.find('*#') + 2:].rstrip()
                        padding = ' '
                        if len(line) < 75:
                            padding = ' ' * (76 - len(line))
                        out_fd.write('/*' + line + padding + '*/\n')
                    else:
                        line = line[line.find('*') + 1:]
                        line = line.rstrip() + '\n'
                        if '::=' in line:
                            indent = len(line) - len(line.lstrip())
                            out_fd.write('\n')
                        if len(line) > indent:
                            line = line[indent:]
                        if len(line) > 1 and line != '/\n':
                            out_fd.write(line)
            elif '<<<ebnf' in line:
                msg.warning(os.path.basename(path) + ' contains <<<ebnf without corresponding ebnf>>>' )


def generate_ebnf(generated_by, output_dir_path, srcdir_path, ebnf_dir_path, ebnf_dir_file):
    with nfc.open(ebnf_dir_path + ebnf_dir_file, 'r') as fd:
        while True:
            line = fd.readline();
            if not line or not line.startswith('#'):
                break
        if line != '':
            output_ebnf_file = line.rstrip()
            msg.info('Generating ' + output_ebnf_file)
            with nfc.open(output_dir_path + output_ebnf_file, 'w') as out_fd:
                out_fd.write('/* Generate by: ' + generated_by + ' */\n\n')
                while True:
                    line = fd.readline();
                    if not line:
                        break
                    if line.startswith('#'):
                        continue
                    pattern = line.rstrip()
                    for path in sorted(glob.glob(srcdir_path + '/' + pattern)):
                        generate_ebnf_scan(path, out_fd)



def generate(generated_by, ebnf_dir_path, generated_dir_path, srcdir_path):

    # Make sure generated/ directory structure exists
    os.makedirs(generated_dir_path, exist_ok=True)
    output_dir_path = generated_dir_path + "ebnf/"
    os.makedirs(output_dir_path, exist_ok=True)

    # Process *.txt ebnf generate files in optional ebnf dir.
    for ebnf_dir_file in sorted(os.listdir(ebnf_dir_path)):
        if fnmatch.fnmatch(ebnf_dir_file, '*.txt') and ebnf_dir_file != 'README.txt':
            generate_ebnf(generated_by, output_dir_path, srcdir_path, ebnf_dir_path, ebnf_dir_file)
