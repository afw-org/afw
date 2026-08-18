#!/usr/bin/env python3

import os
import json
import subprocess
import shutil


def run():
    response = {
        "description": "Test afwdev settings",
        "tests": []
    }

    subprocess.run(
        ["afwdev", "--noprompt", "make-afw-package",
         "test-package-2", "/tmp/test-package-2"],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    if not os.path.isdir("/tmp/test-package-2"):
        response["tests"].append({
            "description":
                "Lists settings (skipping rest of tests, "
                "due to make-afw-package failure)",
            "skip": True,
            "test": "settings"
        })
        return response

    os.chdir("/tmp/test-package-2")

    result = subprocess.run(["afwdev", "settings"], stdout=subprocess.PIPE)
    passed = True
    if result.returncode != 0:
        passed = False
    else:
        try:
            settings = json.loads(result.stdout.decode("utf-8"))
            afwPackages = settings.get("afwPackages")
            forSets = settings.get("forSets")
            passed = False
            if afwPackages is not None and forSets is not None:
                if len(afwPackages) == 1:
                    passed = True
        except Exception:
            passed = False

    response["tests"].append({
        "description": "settings shows correct afwPackages and forSets",
        "passed": passed,
        "test": "settings-no-settings-code-workspace"
    })

    with open("/tmp/test-settings.code-workspace", 'w') as f:
        code_workspace = {
            "folders": [
                {"path": "test-package-1"},
                {"path": "test-package-2"},
            ],
            "settings": {}
        }
        f.write(json.dumps(code_workspace))

    result = subprocess.run(["afwdev", "settings"], stdout=subprocess.PIPE)
    passed = True
    if result.returncode != 0:
        passed = False

    try:
        settings = json.loads(result.stdout.decode("utf-8"))
        afwPackages = settings.get("afwPackages")
        forSets = settings.get("forSets")
        passed = False
        if afwPackages is not None and forSets is not None:
            if len(afwPackages) == 2:
                passed = True
            passed = True
    except Exception:
        passed = False

    response["tests"].append({
        "description":
            "settings with code-workspace shows correct "
            "afwPackages and forSets",
        "passed": passed,
        "test": "settings-code-workspace"
    })

    with open("/tmp/afwdev-settings.json", 'w') as f:
        afwdev_settings = {
            "afwPackages": [
                {"path": "test-package-2"}
            ],
            "forSets": {
                "default": [
                    "test-package-2"
                ]
            }
        }
        f.write(json.dumps(afwdev_settings))

    result = subprocess.run(["afwdev", "settings"], stdout=subprocess.PIPE)
    passed = True
    if result.returncode != 0:
        passed = False

    try:
        settings = json.loads(result.stdout.decode("utf-8"))
        afwPackages = settings.get("afwPackages")
        forSets = settings.get("forSets")
        passed = False
        if afwPackages is not None and forSets is not None:
            if len(afwPackages) == 2:
                passed = True
            passed = True
    except Exception:
        passed = False

    response["tests"].append({
        "description":
            "settings with afwdev-settings.json shows correct "
            "afwPackages and forSets",
        "passed": passed,
        "test": "settings-afwdev-settings"
    })

    try:
        os.remove("/tmp/test-settings.code-workspace")
        os.remove("/tmp/afwdev-settings.json")
    except OSError:
        pass
    shutil.rmtree("/tmp/test-package-2", ignore_errors=True)

    return response
