#! /usr/bin/env python3

import os
import time
from threading import Timer
import watchdog
from watchdog.observers import Observer

from _afwdev.common import msg, nfc
from _afwdev.test.common import is_test_file, run_test, parse_test_run, \
    print_test_response, load_test_environments, load_test_group_config, \
    before_all, before_each, after_all, after_each


def debounce(wait):

    """Postpone a functions execution until after some time has elapsed
 
    :type wait: int
    :param wait: The amount of Seconds to wait before the next call can execute.
    """
 
    def decorator(fun):
        def debounced(*args, **kwargs):
            def call_it():
                fun(*args, **kwargs)
 
            try:
                debounced.t.cancel()
            except AttributeError:
                pass
 
            debounced.t = Timer(wait, call_it)
            debounced.t.start()
 
        return debounced
 
    return decorator

def clearScreen():

    if os.name == "nt":
        _ = os.system("cls")

    else:
        _ = os.system("clear")

class WatchEventHandler(watchdog.events.PatternMatchingEventHandler):

    def __init__(self, options, testEnvironments=None):

        self.options = options 
        self.testEnvironments = testEnvironments

        super().__init__(patterns=["*.as","*.py","*.sh"], ignore_directories=True)       

    @debounce(1)
    def on_modified(self, event):                         

        # remember the current working directory
        pwd = os.getcwd()

        try:                  
            if is_test_file(os.path.basename(event.src_path)): 
                
                root = os.path.dirname(event.src_path)
                testGroupConfig = load_test_group_config(root)

                clearScreen()                           

                before_all(root, testGroupConfig, self.testEnvironments)
                before_each(root, testGroupConfig, self.testEnvironments)

                # Run the test and parse the output                
                start = time.time()
                response, error, debug = run_test(event.src_path, self.options, self.testEnvironments, testGroupConfig)
                end = time.time()   
                
                test_run = parse_test_run(event.src_path, self.options, response, error)         
                hasFailures, allSuccess, \
                    numFailures, numSkipped, numPassed, allSkipped = test_run

                after_each(root, testGroupConfig, self.testEnvironments)

                if error != None:                
                    msg.highlighted_info("{}  ({}ms)".format(os.path.relpath(event.src_path), round((end - start) * 1000)))  
                    if error:
                        msg.error("\n    \u2717 {}\n".format(error))                   
                    if debug:
                        msg.debug(debug)       

                else:
                    msg.highlighted_info("{}  ({}ms)".format(os.path.relpath(event.src_path), round((end - start) * 1000)))

                print_test_response(self.options, event.src_path, response, hasFailures, allSuccess, allSkipped)                           
                
                after_all(root, testGroupConfig, self.testEnvironments)

        finally:
            # always switch back to original working directory        
            os.chdir(pwd)

    def on_created(self, event):                          
        #print("New Test File: ", event.src_path, event.event_type)
        pass


def watch_tests(options, srcdir, testEnvironments):

    clearScreen()
    msg.highlighted_info("Watching for tests (press Ctl-C to quit)...")

    event_handler = WatchEventHandler(options, testEnvironments)

    observer = Observer()
    observer.schedule(event_handler, srcdir, recursive=True)
    observer.start()

    try:
        while True:
            time.sleep(1)
    finally:
        observer.stop()
        observer.join()


def run(options, srcdirs):

    # for now, just choose the first one to watch...
    _, srcdir_path, _, _, _ = srcdirs[0]      

    # load up all the test environments for this srcdir
    testEnvironments = load_test_environments(srcdir_path)

    watch_tests(options, srcdir_path, testEnvironments)