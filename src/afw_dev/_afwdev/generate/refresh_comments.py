#!/usr/bin/env python3

##
# @file refresh_comments.py
# @ingroup afwdev_generate
#

import os
import shutil
import fnmatch

from _afwdev.common import msg, nfc

def update_comment(replacement, full_replacement, tag, o):
    state = 0
    r = o
    # 0 - searching for comment
    # 1 - first line of comment found
    # 2 - searching for end of comment

    for n in range(len(o)):
        if state == 0:
            if o[n].startswith('/*'):
                if o[n].find('*/') == -1:
                    state = 1
        elif state == 1:
            if o[n].upper() != tag.upper():
                state = 0
                continue
            else:
                begin = n
                state = 2
        elif state == 2:
            if o[n].find(' */') != -1:
                r[begin:n] = replacement[:]
                break

    # If comment not found, add new function
    if state == 0:
        r.append(str(''))
        r.append(str(''))
        r.append(str(''))
        r += full_replacement

    return r

def refresh_comments(skel, update):
    i = skel.splitlines()
    o = update.splitlines()
    state = 0
    # 0 - searching for comment
    # 1 - first line of comment found
    # 2 - searching for end of comment
    # 3 - searching for end of first {
    # 4 - searching for closing } # skeletons should not have { } in comments

    for n in range(len(i)):
        if state == 0:
            if i[n].startswith('/*'):
                if i[n].find('*/') == -1:
                    state = 1
                    begin = n+1
        elif state == 1:
            state = 0
            if i[n].startswith(' *'):
                tag = i[n]
                state = 2
        elif state == 2:
            if i[n].find(' */') != -1:
                state = 3
                end_comment = n
        elif state == 3:
            if i[n].startswith('/*'):
                state = 0 # Must have be non-function comment
                if i[n].find('*/') == -1:
                    state = 1
                    begin = n+1
            else:
                for c in range(len(i[n])):
                    if i[n][c] == '{':
                        state = 4
                        nesting = 1
                        break
        elif state == 4:
            for c in range(len(i[n])):
                if i[n][c] == '{':
                    nesting += 1
                elif i[n][c] == '}':
                    nesting -= 1
                if nesting < 1:
                    o = update_comment(i[begin:end_comment], i[begin-1:n+1], tag, o)
                    state = 0

    return '\n'.join(o) + '\n'

#
# This replaces comments from c files in closet_dir_path in the corresponding named
# c files in srcdir_path.  The comments replaced are of the form:
#
# /*
#  * some tag line such as: * Adaptive function: bag<string>
#  * ... more lines ...
#  */
#
# The purpose is to replace updated comments from generated skeletons in the
# c files based on the skeletons.
#
def generate(closet_dir_path, function_srcsubdir_path, function_srcsubdir):
    for file in os.listdir(closet_dir_path):
        if fnmatch.fnmatch(file, '*.c'):
            if os.path.exists(function_srcsubdir_path + file):
                with nfc.open(closet_dir_path + file, 'r') as fd:
                    skel = fd.read()
                with nfc.open(function_srcsubdir_path + file, 'r') as fd:
                    update = fd.read()
                msg.info('Refreshing comments in ' + function_srcsubdir + '/' + file)
                with nfc.open(function_srcsubdir_path + file, 'w') as fd:
                    fd.write(refresh_comments(skel, update))
            else:
                msg.info('Copying new skeleton ' + '/' + file) 
                shutil.copyfile(closet_dir_path + file, function_srcsubdir_path + file)
