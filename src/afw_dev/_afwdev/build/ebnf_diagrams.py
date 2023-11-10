#!/usr/bin/env python3

##
# @file ebnf_diagrams.py
# @ingroup afwdev_build
# @brief This file defines the method for building EBNF diagrams.
#

import os
import shutil
import zipfile
from _afwdev.common import msg, nfc

##
# @brief Builds EBNF diagrams.
# @param options The options dictionary.
#
def build(options):        

    msg.highlighted_info('Building EBNF diagrams')

    # for now, building ebnf diagrams is only supported for afw srcdir
    options['srcdir_path'] = options['afw_package_dir_path'] + 'src/afw/'

    syntax_ebnf = options['srcdir_path'] + '/generated/ebnf/syntax.ebnf'    
    syntax_output_dir_html = options['build_directory_docs'] + 'afw/html/reference/language/ebnf/syntax/'
    syntax_output_dir_xml = options['build_directory_docs'] + 'afw/xml/reference/language/ebnf/syntax/'

    if not os.path.exists(syntax_ebnf):
        msg.error('syntax EBNF file not found: {}'.format(syntax_ebnf))
        return

    # check if the EBNF has changed since the last build
    if os.path.exists(syntax_output_dir_html):
        syntax_ebnf_mtime = os.path.getmtime(syntax_ebnf)
        syntax_output_dir_mtime = os.path.getmtime(syntax_output_dir_html)
        if syntax_ebnf_mtime < syntax_output_dir_mtime:
            msg.info('    EBNF diagrams are up to date')
            return
        
    if os.path.exists(syntax_output_dir_xml):
        syntax_ebnf_mtime = os.path.getmtime(syntax_ebnf)
        syntax_output_dir_mtime = os.path.getmtime(syntax_output_dir_xml)
        if syntax_ebnf_mtime < syntax_output_dir_mtime:
            msg.info('    EBNF diagrams are up to date')
            return

    # use Railroad generator to generate EBNF diagrams, which creates a zip file
    os.system("java -jar /usr/local/bin/rr.war -png -color:#DCDCDC -out:/tmp/syntax.zip " + syntax_ebnf)

    # check that the zip file was created
    if not os.path.isfile('/tmp/syntax.zip') or not zipfile.is_zipfile('/tmp/syntax.zip'):
        msg.error_exit('Railroad generator failed to create zip file')
        return

    # extract zip file to output directory    
    with zipfile.ZipFile('/tmp/syntax.zip', 'r') as zip_ref:
        # first delete all existing diagrams
        msg.info('    Deleting old diagrams')
        shutil.rmtree(syntax_output_dir_html, ignore_errors=True)
        shutil.rmtree(syntax_output_dir_xml, ignore_errors=True)

        # then extract the zip file
        msg.info('    Extracting EBNF diagrams from zip archive')
        zip_ref.extractall(syntax_output_dir_html)
        zip_ref.extractall(syntax_output_dir_xml)

        msg.info('    EBNF diagrams extracted to {}'.format(syntax_output_dir_html))
        msg.info('    EBNF diagrams extracted to {}'.format(syntax_output_dir_xml))