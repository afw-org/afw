#!/usr/bin/env python3

import os
import json
import subprocess
import shutil


def run():
    response = {
        "description": "Test afwdev for",
        "tests": []
    }

    subprocess.run(
        ["afwdev", "--noprompt", "make-afw-package",
         "test-package-4", "/tmp/test-package-4"],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    subprocess.run(
        ["afwdev", "--noprompt", "make-afw-package",
         "test-package-5", "/tmp/test-package-5"],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if not os.path.isdir("/tmp/test-package-4") or \
            not os.path.isdir("/tmp/test-package-5"):
        response["tests"].append({
            "description":
                "(skipping rest of tests, due to make-afw-package failure)",
            "skip": True,
            "test": "for"
        })
        return response

    with open("/tmp/afwdev-settings.json", 'w') as f:
        afwdev_settings = {
            "afwPackages": [
                {"path": "test-package-4"},
                {"path": "test-package-5"},
            ],
            "forSets": {
                "default": [
                    "test-package-4",
                    "test-package-5",
                ]
            }
        }
        f.write(json.dumps(afwdev_settings))

    os.chdir("/tmp/test-package-4")

    result = subprocess.run(
        ["afwdev", "--noprompt", "for", "pwd"],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    passed = result.returncode == 0

    response["tests"].append({
        "description": "for pwd works for each package",
        "passed": passed,
        "test": "for-pwd"
    })

    shutil.rmtree("/tmp/test-package-4", ignore_errors=True)
    shutil.rmtree("/tmp/test-package-5", ignore_errors=True)

    return response
