#!/usr/bin/env python3

import os
import sys
import json
import subprocess
import shutil

response = {
    "description": "Test afwdev for",
    "tests": []
}

# create two new packages
rc = subprocess.run(["afwdev", "--noprompt", "make-afw-package", "test-package-4", "/tmp/test-package-4" ], 
    stdout=subprocess.PIPE, stderr=subprocess.PIPE)
rc = subprocess.run(["afwdev", "--noprompt", "make-afw-package", "test-package-5", "/tmp/test-package-5" ], 
    stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# check if directory was created
if not os.path.isdir("/tmp/test-package-4") or not os.path.isdir("/tmp/test-package-5"):

    response["tests"].append({
        "description": "(skipping rest of tests, due to make-afw-package failure)",
        "skip": True,        
        "test": "for"
    })

    print(json.dumps(response))    

# create an afwdev-settings.json file
with open("/tmp/afwdev-settings.json", 'w') as f:
    afwdev_settings = {
        "afwPackages": [            
            {
                "path": "test-package-4"
            },
            {
                "path": "test-package-5"
            }
        ],
        "forSets": {
            "default": [
                "test-package-4",
                "test-package-5"
            ]
        }
    }
    f.write(json.dumps(afwdev_settings))

# change to the directory where the package was created
os.chdir("/tmp/test-package-4")

# Run a simple pwd command for each package
result = subprocess.run(["afwdev", "--noprompt", "for", "pwd"], 
    stdout=subprocess.PIPE, stderr=subprocess.PIPE)
passed = True
if result.returncode != 0:
    passed = False

response["tests"].append({
    "description": "for pwd works for each package",
    "passed": passed,
    "test": "for-pwd"
})

shutil.rmtree("/tmp/test-package-4", ignore_errors=True)
shutil.rmtree("/tmp/test-package-5", ignore_errors=True)


# print response
print(json.dumps(response))