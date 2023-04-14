#! /usr/bin/env python3

import os

from _afwdev.common import msg, nfc

def load_json_from_file(file):
    with nfc.open(file) as f:
        return nfc.json_load(f)        

# Runs a coverage report for tests that are implemented vs missing
def coverage(options, srcdir, objects_dir):       
    
    if os.path.exists(objects_dir + '_AdaptiveFunctionGenerate_'):

        total = 0
        covered = 0

        functions = os.listdir(objects_dir + '_AdaptiveFunctionGenerate_')
        #poly = os.listdir(objects_dir + '_AdaptivePolymorphicFunction_')

        if len(functions) > 0:
            msg.highlighted_info("\nTest Coverage for package {}:\n".format(srcdir))

        if len(functions) > 0:
            for file in sorted(functions):
                if file.endswith('.json'):
                    func = load_json_from_file(objects_dir + '_AdaptiveFunctionGenerate_/' + file)
                    functionId = func.get('functionId')                

                    total += 1
                    if os.path.exists(objects_dir + '_AdaptiveFunctionTest_/' + file):
                        covered += 1
                        msg.success("  YES     " + functionId)
                        continue
                    
                    msg.error("   NO     " + functionId)

        '''
        if len(poly) > 0:
            for file in sorted(poly):
                if file.endswith('.json'):
                    func = load_json_from_file(objects_dir + '_AdaptivePolymorphicFunction_/' + file)
                    functionId = func.get('functionId')                

                    total += 1
                    if os.path.exists(objects_dir + '_AdaptiveFunctionTest_/' + file):
                        covered += 1
                        msg.success("  " + functionId)
                        continue
                    
                    msg.error("  " + functionId)
        '''

        #if total > 0:
        msg.highlighted_info("\n  Coverage:  {}/{}".format(covered, total))

def run(options, srcdirs):

    for srcdir, _, objects_dir, _, _ in srcdirs:
        coverage(options, srcdir, objects_dir)