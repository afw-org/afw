#! /usr/bin/env python3

##
# @ingroup afwdev_test
#

##
# @file common.py
# @brief This file contains common routines that are used by all 
#        test routines.
# @details Under src/<srcdir>/tests/, directories with test files form groups.
#          Optional config.py may define Tags, Environment, and before/after
#          hooks. Shared fixtures live in tests/environments/. Execution modes
#          are plugins under _afwdev/test/modes/.
#

import os
import shutil
import signal
import subprocess
import importlib
import importlib.machinery
import re
from os.path import exists

from _afwdev.common import msg, nfc
from _afwdev.common.errors import error_message, error_to_dict


def outcome_flag(value, default=False):
    """Coerce a case outcome (passed / skip) to bool.

    Print used ``== True`` and parse used ``== False``, so a leftover
    truthy string (a Python ``and`` chain that yielded compiler text)
    could print as fail and count as pass. After coerce they agree.
    """
    if value is None:
        return bool(default)
    return bool(value)


def normalize_test_response(response):
    """Make passed / skip real bools on a test response (in place)."""
    if not response or not isinstance(response, dict):
        return response
    if "skip" in response:
        response["skip"] = outcome_flag(response.get("skip"), False)
    for tc in response.get("tests") or []:
        if not isinstance(tc, dict):
            continue
        tc["passed"] = outcome_flag(tc.get("passed"), False)
        tc["skip"] = outcome_flag(tc.get("skip"), False)
    return response


def show_all_cases(options):
    """True when the console should print passing cases too.

    ``--show-all`` always wins. A real ``--test-pattern`` is an inspect
    loop, so cases are shown without typing --show-all. Full ``test -j``
    stays errors-only.
    """
    if not options:
        return False
    if options.get("show_all"):
        return True
    pattern = options.get("test-pattern")
    return bool(pattern) and pattern != ".*"


def errors_only_console(options):
    """True when passing cases should stay off the console."""
    return options.get("errors", True) and not show_all_cases(options)


def want_error_detail(options):
    """Fail-only extras: source, expect/result, short backtrace.

    --error-detail, --verbose, and --debug. Not a firehose: no passing
    cases, no valgrind XML, no test-case JSON (those stay --debug).
    """
    if not options:
        return False
    if options.get("error_detail"):
        return True
    return msg.is_verbose_mode() or msg.is_debug_mode()


##
# @brief Human-readable message for a process killed by a signal
# @param returncode Subprocess return code (negative when killed by signal)
# @details Python reports signal death as a negative returncode (-N for
#          signal N). Decode to a name such as SIGSEGV when possible.
#
def format_abnormal_process_exit(returncode):
    if returncode is None:
        return "Process exited abnormally"
    if returncode >= 0:
        return "Process exited with return code {}".format(returncode)
    sig = -returncode
    try:
        name = signal.Signals(sig).name
    except (ValueError, AttributeError):
        name = "signal {}".format(sig)
    return "Process exited abnormally with return code {} ({})".format(
        returncode, name)


##
# @brief Path of a test file relative to base (package root when possible)
#
def test_path_for_display(test, base=None):
    if base is None:
        base = os.getcwd()
    try:
        return os.path.relpath(test, base)
    except ValueError:
        return test


##
# @brief Normalize --tests-path (-T) option values
# @param raw None, str, or list of paths from CLI
# @return list of absolute existing directory paths (may be empty)
# @details Shared by afwdev test and afwdev blast. When non-empty, callers
#          use only these roots instead of package src/*/tests discovery.
#
def normalize_tests_paths(raw):
    if not raw:
        return []
    if isinstance(raw, str):
        raw = [raw]
    paths = []
    for p in raw:
        if p is None or p == "":
            continue
        ap = os.path.abspath(os.path.expanduser(str(p)))
        if not os.path.isdir(ap):
            msg.error_exit(
                "afwdev --tests-path is not a directory: " + ap)
        paths.append(ap)
    return paths


##
# @brief Truncate a one-line detail for digests / JSON summaries
#
def clip_detail(text, max_len=400):
    if text is None:
        return ""
    s = str(text).replace("\n", " ").strip()
    if len(s) <= max_len:
        return s
    if max_len < 4:
        return s[:max_len]
    return s[: max_len - 3] + "..."


##
# @brief Write test/blast style results summary for --output / --output-format
# @param options Must include output / output_format (optional)
# @param summary dict to serialize
# @param tool_label Short name for the "Wrote …" message (e.g. test, blast)
#
def write_results_summary(options, summary, tool_label="test"):
    import sys

    output = options.get("output") or "stdout"
    # Compat: literal "stdout" = do not write a summary artifact
    if output == "stdout":
        return

    fmt = (options.get("output_format") or "json").strip().lower()
    if fmt in ("json-min", "compact"):
        fmt = "json-compact"

    close_fd = False
    if output == "-":
        fd = sys.stdout
    else:
        fd = nfc.open(output, "w")
        close_fd = True

    try:
        if fmt == "json":
            nfc.json_dump(summary, fd, indent=2, sort_keys=True)
            fd.write("\n")
        elif fmt == "json-compact":
            nfc.json_dump(
                summary, fd, sort_keys=True, separators=(",", ":"))
            fd.write("\n")
        elif fmt == "text":
            # Generic: prefer tests/ then blast-style ok/fail counts
            t = summary.get("tests") or summary.get("requests") or {}
            if "passed" in t or "failed" in t:
                fd.write(
                    "passed={p} failed={f} skipped={s} total={n}\n"
                    "time_seconds={sec}\n".format(
                        p=t.get("passed", t.get("ok", 0)),
                        f=t.get("failed", t.get("fail", 0)),
                        s=t.get("skipped", t.get("timeout", 0)),
                        n=t.get("total", 0),
                        sec=summary.get("time_seconds", 0),
                    ))
            else:
                fd.write(nfc.json_dumps(summary, indent=2) + "\n")
            fails = summary.get("failures") or []
            if fails:
                fd.write("failures ({n}):\n".format(n=len(fails)))
                for item in fails:
                    fd.write(
                        "  {path}  {detail}\n".format(
                            path=item.get("test") or item.get("path") or "?",
                            detail=item.get("detail") or "",
                        ))
            else:
                fd.write("failures: none\n")
        else:
            msg.error_exit(
                "Unknown --output-format {!r} "
                "(use json, json-compact, or text)".format(fmt))
    finally:
        if close_fd:
            fd.close()

    if output != "-":
        msg.highlighted_info(
            "Wrote {tool} summary ({fmt}) to {path}".format(
                tool=tool_label, fmt=fmt, path=output))


##
# @brief Print a short end-of-run list of failing tests
# @param failures List of dicts with at least 'test' and optional 'detail'
# @details Console-only helper so parallel runs still end with a greppable
#          list of paths. Does not change --output JSON shape.
#
def print_failure_digest(failures):
    if not failures:
        return
    msg.highlighted_info("")
    msg.error("Failures ({}):".format(len(failures)))
    for f in failures:
        path = f.get('test') or '?'
        detail = f.get('detail')
        if detail:
            msg.error("  {}  {}".format(path, detail))
        else:
            msg.error("  {}".format(path))
    msg.highlighted_info(
        "Re-run:  afwdev test --test-pattern '<filename>'  "
        "(see also -p / --list)")


##
# @brief Return True if the test group Tags match --tags (test_tags) pattern
# @param options The options dictionary
# @param testGroupConfig The loaded test group config, or None
# @details Default pattern ".*" matches all groups. Otherwise at least one
#          Tags entry must match the regex. Groups with no Tags are skipped
#          when a non-default pattern is set.
#
def test_group_matches_tags(options, testGroupConfig):
    tag_pattern = options.get("test_tags") or ".*"
    if tag_pattern == ".*":
        return True
    tags = (testGroupConfig or {}).get("Tags") or []
    try:
        return any(re.search(tag_pattern, tag) for tag in tags)
    except re.error as e:
        msg.error_exit("Invalid --tags regex '" + tag_pattern + "': " + str(e))


# common routine for calling before/after functions
def test_wrapper(root, testGroupConfig, testEnvironment, action):
    if testGroupConfig and testGroupConfig.get(action):
        testGroupConfig[action]()        

    elif exists(root + '/' + action + '.py'):
        msg.debug("Running " + action + ".py")
        rc = subprocess.run(['python3', root + '/' + action + '.py'])
        if rc.returncode != 0:
            raise Exception(
                action + '.py failed with exit code ' + str(rc.returncode))

# function that is run before all tests in a test group
def before_all(root, testGroupConfig, testEnvironment):

    test_wrapper(root, testGroupConfig, testEnvironment, 'before_all')

# function that is run before each test in a test group
def before_each(root, testGroupConfig, testEnvironment):

    test_wrapper(root, testGroupConfig, testEnvironment, 'before_each')

# function that is run after each test in a test group
def after_each(root, testGroupConfig, testEnvironment):

    test_wrapper(root, testGroupConfig, testEnvironment, 'after_each')

# function that is run after all tests in a test group
def after_all(root, testGroupConfig, testEnvironment):

    test_wrapper(root, testGroupConfig, testEnvironment, 'after_all')


##
# @brief Checks if a file is a test file
# @details This routine checks to determine if a discovered file is a test file.
#          Some names are reserved for specific purposes, and are ignored as a 
#          test file. For now, we only look for .as,.py,.sh scripts, ignoring 
#          '_' prefixed filenames.
# @param file The file to check.
#
def is_test_file(file):    
    skip = [        
        "config.py",
        # orchestrated-test markers are discovered as leaf units, not as scripts
        "orchestration.yaml",
        "orchestration.json",
    ]

    if (file.endswith(".as") or
        file.endswith(".py") or
        file.endswith(".sh") or
        re.match(r"^commands_.+\.txt$", file)):
        if file in skip or file.startswith("_"):
            return False        
        return True
    return False


def _test_pattern_matches(pattern, path_or_name):
    """True if --test-pattern matches basename or full path string."""
    if not pattern or pattern == ".*":
        return True
    try:
        if re.search(pattern, path_or_name):
            return True
        base = os.path.basename(path_or_name)
        if base != path_or_name and re.search(pattern, base):
            return True
        # leaf directory name (for orchestration.yaml under smoke/)
        parent = os.path.basename(os.path.dirname(path_or_name))
        if parent and re.search(pattern, parent):
            return True
    except re.error:
        return False
    return False

##
# @brief Checks if a directory is a test group
# @details This routine checks a directory to determine if all of the files
#          belong to a single test group. Test groups have multiple test files
#          with common configuration or environments, which may even share 
#          databases. The expectation is that a test group should not be split 
#          apart and run in parallel.
# @param dir The directory to check.
#
def is_test_group(dir):
    
    # just check of a config.py exists
    if exists(dir + '/config.py'):
        return True
    
    return False


##
# @brief Find the matching test environment
# @details For a given testGroupConfig and a list of all testEnvironments, this 
#          routine will return the matching test environment. If one does not 
#          exist, then a default one will be created to setup the proper working 
#          directory.
#
#          It will also prime the working directory with any files and 
#          configurations that the test may need.
# @param testGroup The test group tuple containing the source directory, the
#                  test group directory, and a list of test files.
# @param testEnvironments The list of all test environments.
# @param testGroupConfig The test group configuration.
# @param work_dir_prefix The prefix to use for the working directory.
#
def get_test_environment(testGroup, testEnvironments, testGroupConfig, work_dir_prefix):

    _, root, _ = testGroup
    testEnv = None
    
    if testEnvironments and testGroupConfig:
        environment = testGroupConfig.get('environment')
        if environment:
            for env in testEnvironments:
                if env['name'] == environment:
                    testEnv = env

    # get the directory path, relative to srcdir/src path    
    relpath = os.path.relpath(root, os.getcwd() + "/src")

    if not testEnv:
        # if we don't have a configured test environment, create a default one
        testEnv = {
            'name': relpath,
            'path': root
        }

    # set the output working directory
    #
    # Note: The relpath gets "flattened" by subbing '/' for '.'
    #       this is because the test runner will create a directory
    #       for each test group, and we want to make sure that the
    #       directory names do not overlap.
    work_dir = work_dir_prefix + "/" + relpath.replace("/", ".")
    testEnv['work_dir'] = work_dir

    # check if we've created a directory for this testEnv yet    
    if not os.path.isdir(work_dir):
        msg.debug("Creating test environment directory: " + work_dir)        

        # make the working directory structure
        os.makedirs(work_dir)        

        # copy everything from testEnv['path'] to work_dir
        if testEnv.get('path'):
            msg.debug("Copying test environment files from: " + testEnv['path'])
            for name in os.listdir(testEnv['path']):
                src = os.path.join(testEnv['path'], name)
                dst = os.path.join(work_dir, name)
                if os.path.isdir(src):
                    shutil.copytree(src, dst)
                else:
                    shutil.copy2(src, dst)

    return testEnv


# Given a source directory, this routine will find and load test environment 
# configurations within the srcdir/tests/environments/. Test groups will use
# use these environments when they are run.
#
# These environments allow test groups to share/use common configurations.
def load_test_environments(srcdir):

    testEnvironments = []

    # look for test/environments/ directory
    environmentsDir = srcdir + '/tests/environments'    
    if exists(environmentsDir):
        for environment in os.listdir(environmentsDir):            
            if os.path.isdir(environmentsDir + '/' + environment):                
                config = {}
                config['name'] = environment
                config['path'] = environmentsDir + '/' + environment

                # look for afw.conf
                if exists(environmentsDir + '/' + environment + '/afw.conf'):
                    config['afw_conf'] = environmentsDir + '/' + environment + '/afw.conf'

                testEnvironments.append(config)

    return testEnvironments


# if a test group has a 'config.py' file, this provides the test runner 
# with some configuration for this test group, including before/after 
# functions to call and default test environments.
def load_test_group_config(root):

    # check if this group has a config.py definition
    fullPath = root + '/config.py'
    if exists(fullPath):

        # Unique module name per path so Tags/hooks from one group are not
        # reused for another (SourceFileLoader caches by module name).
        module_name = 'afwdev_test_config_' + re.sub(r'[^A-Za-z0-9_]', '_', os.path.abspath(fullPath))
        test_config = importlib.machinery.SourceFileLoader(
            module_name, fullPath).load_module()

        groupConfig = {
            'before_all': test_config.before_all if hasattr(test_config, 'before_all') else None,
            'after_all': test_config.after_all if hasattr(test_config, 'after_all') else None,
            'before_each': test_config.before_each if hasattr(test_config, 'before_each') else None,
            'after_each': test_config.after_each if hasattr(test_config, 'after_each') else None,
            'environment': test_config.Environment if hasattr(test_config, 'Environment') else None,
            'Tags': test_config.Tags if hasattr(test_config, 'Tags') else None,
        }                        
        
        return groupConfig

# For a given folder, find all tests 
def find_tests(options, root, files):
    tests = []
    pattern = options.get("test-pattern")

    for f in files:
        # check options to filter out tests
        if pattern and pattern != ".*":
            full = os.path.join(root, f)
            if not _test_pattern_matches(pattern, full) and \
                    not _test_pattern_matches(pattern, f):
                msg.debug("Skipping test (does not match): " + f)
                continue

        if is_test_file(f):
            tests.append(os.path.join(root, f))

    return tests

# Within a <srcdir>/tests/ directory, this routine will find all test groups
# and return a list of test groups. Each test group is a tuple containing
# the source directory, the test group directory, and a list of test files
def find_test_groups(options, srcdir, tests_dir):
    testGroups = []

    for root, dirs, files in os.walk(tests_dir):
        # exclude subdirectory environments and any directories that start with an underscore
        dirs[:] = [d for d in dirs if d != 'environments' and not d.startswith('_')]

        # orchestrated-test leaf: marker directory is one test; do not walk children
        try:
            from _afwdev.test.orchestrated.discovery import (
                find_orchestration_marker)
            marker = find_orchestration_marker(root)
        except ValueError as e:
            msg.error(str(e))
            dirs[:] = []
            testGroups.append((srcdir, root, []))
            continue
        except ImportError:
            marker = None

        if marker:
            dirs[:] = []  # prune — assets only under leaf
            pattern = options.get("test-pattern")
            if pattern and pattern != ".*" and \
                    not _test_pattern_matches(pattern, marker):
                msg.debug(
                    "Skipping orchestrated-test (does not match pattern): " +
                    marker)
                continue
            testGroups.append((srcdir, root, [marker]))
            continue

        if is_test_group(root):
            pass

        tests = find_tests(options, root, files)
        if len(tests) > 0:
            testGroups.append(
                (srcdir, root, tests)
            )

    return testGroups

# This is the main routine to run an individual test script inside a test 
# group. After running the test, it returns a tuple containing the test
# response, any errors, and the stderr output.
def run_test(test, options, testEnvironment=None, testGroupConfig=None):

    # orchestration.yaml|json — harness runner (env-mode modulates attach)
    try:
        from _afwdev.test.orchestrated.discovery import (
            is_orchestration_marker_path)
        from _afwdev.test.orchestrated.runner import run_orchestrated_test
        if is_orchestration_marker_path(test):
            return run_orchestrated_test(
                test, options, testEnvironment, testGroupConfig)
    except ImportError as e:
        if test and (
                test.endswith("orchestration.yaml") or
                test.endswith("orchestration.json")):
            msg.error("Unable to load orchestrated-test runner: " + str(e))
            return (None, str(e), None)

    # look at the test file extension to determine how to run it
    # some files, such as .py files have to be run in 'python' mode
    if test.endswith(".py"):
        mode = "python"

    # Files whose name starts with 'commands_' and has the .txt file
    # extension are run in 'commands' mode.
    elif re.match(r".*/commands_.+\.txt$", test):
        mode = "commands"

    else:
        # dynamically import the environment mode from the 'mode' directory
        mode = options.get('mode')    

    if mode:        
        try:
            mode = importlib.import_module('_afwdev.test.modes.' + mode)        

            # invoke the run_test method
            return mode.run_test(test, options, testEnvironment, testGroupConfig)

        except ImportError as e:
            msg.error("Unable to load mode: " + mode)
            msg.error(str(e))
    else:
        msg.error("No mode specified for test: " + test)

    return (None, None, None)


# Given a sourceLocation path, return the individual source "file" that 
# it refers to, in order to help with comparing two different sourceLocations
def get_source_location_basename(sourceLocation):

    # sourceLocation may look like /path/to/basename+offset(line:col)
    # parse out the basename
    if '/' in sourceLocation:
        basename = sourceLocation.split('/')[-1]
    else:
        basename = sourceLocation

    if '+' in basename:
        basename = basename.split('+')[0]

    return basename


def compare_source_locations(sourceLocation1, sourceLocation2):
    
    basename1 = get_source_location_basename(sourceLocation1)
    basename2 = get_source_location_basename(sourceLocation2)

    return basename1 == basename2

def get_source_location_coordinates(source, sourceLocation):    

    if source != None:
        cursor = 0

        sourceLines = source.split("\n")
        for index, sourceLine in enumerate(sourceLines):
            cursor += len(sourceLine) + 1

            if cursor > sourceLocation:
                offset = len(sourceLine) - (cursor - sourceLocation) + 1
                return index + 1, offset
            
    return None, None

##
# @brief Formats a chunk of source code for printing
# @details Formats a chunk of source code, with an optional title and 
#          highlightOffset, to indicate where in the source code the 
#          user should focus on.
# @param source The source code to format.
# @param title An optional title to print before the source code.
# @param highlightOffset An optional offset to highlight in the source code.
# @param maxLines The maximum number of lines to print.
# @param message An optional message to print after the source code.
#
def format_source_code(source, title = None, highlightOffset = 0, maxLines = 10, message = None):

    if source != None:
        if title:
            msg.error("    {}\n".format(title))

        highlightLine = None
        sourceLines = source.split("\n")
        if highlightOffset:
            cursor = 0           
            outputLines = []             

            for index, sourceLine in enumerate(sourceLines):
                cursor += len(sourceLine) + 1                                

                if cursor > highlightOffset:
                    offset = len(sourceLine) - (cursor - highlightOffset) + 1
                    if offset >= 0:
                        outputLines.append("  > | " + sourceLine)                        
                        outputLines.append("    | " + (" " * offset) + "^") 

                        if message:
                            outputLines.append("")
                            outputLines.append("      " + message) 
                            outputLines.append("")

                        highlightLine = index
                    else:
                        outputLines.append("    | " + sourceLine)                        
                else:
                    outputLines.append("    | " + sourceLine)     

            numLines = len(outputLines)
            if numLines > maxLines:
                cut = int(maxLines / 2)
                
                if highlightLine is not None:                    
                    start = highlightLine - cut
                    end = highlightLine + cut + 2
                else:
                    start = 0
                    end = maxLines
                if message:
                    # adjust for the error message
                    end = end + 3

                if start < 0:
                    start = 0
                if end > numLines:
                    end = numLines

                outputLines = outputLines[start:end]

                if start > 0:
                    outputLines.insert(0, "    | ...")
                if end < numLines:
                    outputLines.append("    | ...")                                             

            for line in outputLines:
                msg.error(line)

            msg.error("\n")            

        else:            
            if maxLines > 0:                
                for sourceLine in sourceLines[0:maxLines]:
                    msg.error("    | " + sourceLine)
                if len(sourceLines) > maxLines:
                    msg.error("    | ...")
            else:
                for sourceLine in sourceLines:
                    msg.error("    | " + sourceLine)

            msg.error("\n")

def get_rel_source_location_nav(test, testCase):

    tc_sourceLocation = testCase.get('sourceLocation')
    tc_sourceLineNumberInTestScript = testCase.get('sourceLineNumberInTestScript', 0)
    tc_sourceColumnNumberInTestScript = testCase.get('sourceColumnNumberInTestScript', 0)

    # format sourceLocation so that it's printed value can be easily navigated to              
    if tc_sourceLocation:                                  
        tc_sourceLocationCoordinates = str(tc_sourceLineNumberInTestScript) + ":" + str(tc_sourceColumnNumberInTestScript)        
        tc_sourceLocationNav = test + ':' + tc_sourceLocationCoordinates

        return tc_sourceLocationNav
    
def get_rel_error_source_location_nav(test, testCase):

    tc_sourceLineNumberInTestScript = testCase.get('sourceLineNumberInTestScript', 0)
    error = error_to_dict(testCase.get('error', None))

    if error:
        lineNumber = error.get("parserLineNumber")
        columnNumber = error.get("parserColumnNumber")
        offset = error.get("offset")

        if not lineNumber or not columnNumber:
            if offset:
                source = testCase.get('source')
                lineNumber, columnNumber = get_source_location_coordinates(source, offset)

        if not lineNumber or not columnNumber:
            return None

        # format sourceLocation so that it's printed value can be easily navigated to              
        tc_sourceErrorLocationCoordinates = str(
            tc_sourceLineNumberInTestScript + lineNumber - 1
            ) + ":" + str(columnNumber)
        tc_sourceLocationNav = test + ':' + tc_sourceErrorLocationCoordinates

        return tc_sourceLocationNav

    return None


##
# @brief Print a test failure
# @details This routine will print the result of a test that failed, including 
#          any error details that came back.
# @param test The test file that was run.
# @param testCase The test case that failed.
#
def print_test_failure(test, testCase):

    sourceLocation = testCase.get("sourceLocation")
    sourceLocationNav = get_rel_source_location_nav(test, testCase)
    sourceErrorLocationNav = get_rel_error_source_location_nav(test, testCase)
    # Normalize to Adaptive-shaped dict (issue #61 helpers)
    error = error_to_dict(testCase.get("error"))

    def _print_stream_expects():
        # Side-channel expects (expect-stdout / expect-stderr) — literal text
        if testCase.get("expect-stdout") is not None:
            msg.error("    Expected stdout: " +
                      nfc.json_dumps(testCase.get("expect-stdout")))
            msg.error("    Actual stdout:   " +
                      nfc.json_dumps(testCase.get("stdout")) + "\n")
        if testCase.get("expect-stderr") is not None:
            msg.error("    Expected stderr: " +
                      nfc.json_dumps(testCase.get("expect-stderr")))
            msg.error("    Actual stderr:   " +
                      nfc.json_dumps(testCase.get("stderr")) + "\n")

    if error:
        message = error.get("message")
        if message:
            msg.error("    " + message + "\n")        

        errorSourceLocation = error.get("sourceLocation")    
        parserCursor = error.get("parserCursor")
        errorOffset = error.get("offset")        

        source = ""
        sourceLineOffset = 0

        if sourceLocation and errorSourceLocation:
            hasMatchingSourceLocations = compare_source_locations(sourceLocation, errorSourceLocation)            
        else:
            hasMatchingSourceLocations = False

        # if parserCursor was provided, we have a syntax error and use parserSource
        if parserCursor:
            source = error.get('parserSource')
            sourceLineOffset = parserCursor

        # otherwise use the test source, if the sourceLocation value matches the error sourceLocation
        elif hasMatchingSourceLocations is not None and errorOffset is not None:
            source = testCase.get('source')
            sourceLineOffset = errorOffset       

        if sourceLineOffset: 
            if sourceErrorLocationNav:   
                title="Test Failed at: \n\t{}\n\t└─ {}".format(sourceErrorLocationNav, sourceLocationNav)
            else:
                title="Test Failed at: {}".format(sourceLocationNav)
            
            format_source_code(source, highlightOffset=sourceLineOffset, title=title, message=message)

        else:
            # In this case, we couldn't determine the exact source location of the error
            # This may be due to the error occurring in a model or external script that
            # we can't access in this context.            
            if errorSourceLocation is not None and errorOffset is not None:
                msg.error("    Error occurred at source location: {}+{}\n".format(errorSourceLocation, errorOffset))   
                format_source_code(testCase.get("source"), "Test Failed at: {}".format(sourceLocationNav))
            elif errorSourceLocation is not None:
                msg.error("Test Failed at: {}".format(sourceLocationNav) + " (id=" + error.get("id") + ")")

        if testCase.get("expect") is not None or testCase.get("result") is not None:
            msg.error("    Expected: " + nfc.json_dumps(testCase.get("expect")))
            if testCase.get("result") is not None:
                msg.error("    Result:   " + nfc.json_dumps(testCase.get("result")) + "\n")
            else:
                msg.error("    Result:   undefined\n")

        _print_stream_expects()
        _print_error_backtrace(error)

    else:
        # if there was no error object, look for expect/result and dump the source
        if (testCase.get("expect") != None):
            if testCase != None:
                msg.error("    Expected: " + nfc.json_dumps(testCase.get('expect')))
            else:
                msg.error("    Expected: undefined")

            if (testCase.get("result") != None):
                msg.error("    Result:   " + nfc.json_dumps(testCase.get("result")) + "\n")    
            else:
                msg.error("    Result:   undefined\n")

        _print_stream_expects()

        format_source_code(testCase.get("source"), "Test Failed at {}".format(sourceLocationNav))


def _print_error_backtrace(error, max_frames=12):
    """Print a clipped Adaptive error backtrace, if the object has one."""
    if not error:
        return
    bt = error.get("backtrace")
    if not bt:
        return
    if isinstance(bt, list):
        lines = [str(x) for x in bt[:max_frames]]
    else:
        lines = str(bt).splitlines()[:max_frames]
    if not lines:
        return
    msg.error("    backtrace:")
    for line in lines:
        msg.error("      " + line)


##
# @brief This routine will print the results of a test run.
# @param options The options dictionary.
# @param test The test file that was run.
# @param response The response from the test run.
# @param hasFailures A boolean indicating if there were any failures.
# @param allSuccess A boolean indicating if all tests passed.
# @param allSkipped A boolean indicating if all tests were skipped.
#
def print_test_response(options, test, response, hasFailures, allSuccess, allSkipped):

    if response != None:     
        description = response.get('description')  
        if allSkipped:              
            msg.warn("  SKIP", end="")            
        elif hasFailures:
            msg.error("  FAIL", end="")            
        elif allSuccess:
            msg.success("  PASS", end="")        
        else:
            msg.warn("  PASS", end="")

        msg.highlighted_info(" {}".format(description))    

        if allSkipped:
            return        

        # Default is errors-only; --show-all or a real --test-pattern
        # prints passes/skips too.
        errors_only = errors_only_console(options)
            
        for testCase in response['tests']:
            
            tc_skip = outcome_flag(testCase.get('skip'), False)
            tc_passed = outcome_flag(testCase.get('passed'), False)
            tc_description = testCase.get('description')
            tc_test = testCase.get('test')               

            if tc_skip:                                            
                if not errors_only:
                    msg.warn("    \u25cb", end="")
                    msg.highlighted_info(" {}".format(tc_test))
                    if (msg.is_verbose_mode()):
                        print("\033[2m      {}\033[0m\n".format(tc_description))
                    msg.debug(nfc.json_dumps(testCase, sort_keys=True, indent=4))                  
            elif tc_passed:
                if not errors_only:
                    msg.success("    \u2713", end="")
                    msg.highlighted_info(" {}".format(tc_test))
                    if (msg.is_verbose_mode()):
                        print("\033[2m      {}\033[0m\n".format(tc_description))
                    msg.debug(nfc.json_dumps(testCase, sort_keys=True, indent=4))                            
            else:
                # failed test
                msg.error("    \u2717", end="")
                msg.highlighted_info(" {}".format(tc_test))
                # One-line reason always (structured error.message when present)
                reason = error_message(testCase.get("error"))
                if reason:
                    msg.error("      {}".format(reason))
                elif tc_description and tc_description != tc_test:
                    msg.error("      {}".format(tc_description))
                if (msg.is_verbose_mode()):
                    print("\033[2m      {}\033[0m\n".format(tc_description))                
                msg.debug(nfc.json_dumps(testCase, sort_keys=True, indent=4))
                
                # Source / expect / backtrace: --error-detail, --verbose, --debug
                if want_error_detail(options):
                    print_test_failure(test, testCase)


##
# @brief Print a test run
# @details This routine parses a test run and returns the appropriate counters 
#          summing the number of tests, failures, and errors.
# @param test The test file that was run.
# @param options The options dictionary.
# @param response The response from the test run.
# @param error The error from the test run.
#
def parse_test_run(test, options, response, error):

    allSkipped = False
    hasFailures = False
    allSuccess = True
    numFailures = 0
    numSkipped = 0
    numPassed = 0

    if response == None and error == None:
        # treat these tests as skipped
        allSkipped = True

    if response != None:
        normalize_test_response(response)
        # check if all tests were skipped
        allSkipped = outcome_flag(response.get("skip"), False)

        cases = response.get("tests")
        if not isinstance(cases, list):
            msg.debug("Parsing test run, returned no tests: {}".format(test))
            msg.debug(nfc.json_dumps(response, sort_keys=True, indent=4))
            hasFailures = True
            allSuccess = False
            numFailures += 1
        else:
            for testCase in cases:
                if not isinstance(testCase, dict):
                    hasFailures = True
                    allSuccess = False
                    numFailures += 1
                    continue
                if allSkipped or outcome_flag(testCase.get("skip"), False):
                    allSuccess = False
                    numSkipped += 1
                elif not outcome_flag(testCase.get("passed"), False):
                    hasFailures = True
                    allSuccess = False
                    numFailures += 1
                else:
                    numPassed += 1

    if error != None:
        hasFailures = True
        allSuccess = False
        numFailures += 1

    return hasFailures, allSuccess, numFailures, numSkipped, numPassed, allSkipped  
