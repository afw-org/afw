#!/usr/bin/env python3

import os
import sys
import json
import subprocess
import shutil

response = {
    "description": "Test afwdev settings",
    "tests": []
}

# create a new package
rc = subprocess.run(["afwdev", "--noprompt", "make-afw-package", "test-package-2", "/tmp/test-package-2" ], 
    stdout=subprocess.PIPE, stderr=subprocess.PIPE)

# check if directory was created
if not os.path.isdir("/tmp/test-package-2"):    

    response["tests"].append({
        "description": "Lists settings (skipping rest of tests, due to make-afw-package failure)",
        "skip": True,        
        "test": "settings"
    })

    print(json.dumps(response))
    #sys.exit(1)

# change to the directory where the package was created
os.chdir("/tmp/test-package-2")



#
# Test settings with no code-workspace or settings files
#
result = subprocess.run(["afwdev", "settings"], stdout=subprocess.PIPE)
passed = True
if result.returncode != 0:    
    passed = False
else:
    try:
        settings = json.loads(result.stdout.decode("utf-8"))    

        # check that settings shows afwPackages and forSets
        afwPackages = settings.get("afwPackages")
        forSets = settings.get("forSets")

        passed = False
        if afwPackages is not None and forSets is not None:    
            if len(afwPackages) == 1:
                passed = True        

    except:
        passed = False

response["tests"].append({
    "description": "settings shows correct afwPackages and forSets",
    "passed": passed,
    "test": "settings-no-settings-code-workspace"
})  


#
# Test settings with code-workspace file
#
with open("/tmp/test-settings.code-workspace", 'w') as f:
    code_workspace = {
        "folders": [
            {
                "path": "test-package-1"
            },
            {
                "path": "test-package-2"
            }
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

    # check that settings shows afwPackages and forSets
    afwPackages = settings.get("afwPackages")
    forSets = settings.get("forSets")

    passed = False
    if afwPackages is not None and forSets is not None:    
        if len(afwPackages) == 2:
            passed = True
        passed = True

except:
    passed = False

response["tests"].append({
    "description": "settings with code-workspace shows correct afwPackages and forSets",
    "passed": passed,
    "test": "settings-code-workspace"
}) 




#
# Test settings with afwdev-settings.json file
#
with open("/tmp/afwdev-settings.json", 'w') as f:
    afwdev_settings = {
        "afwPackages": [            
            {
                "path": "test-package-2"
            }
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

    # check that settings shows afwPackages and forSets
    afwPackages = settings.get("afwPackages")
    forSets = settings.get("forSets")

    passed = False
    if afwPackages is not None and forSets is not None:    
        if len(afwPackages) == 2:
            passed = True
        passed = True

except:
    passed = False

response["tests"].append({
    "description": "settings with afwdev-settings.json shows correct afwPackages and forSets",
    "passed": passed,
    "test": "settings-afwdev-settings"
}) 



# cleanup
try:
    os.remove("/tmp/test-settings.code-workspace")
    os.remove("/tmp/afwdev-settings.json")
except OSError:
    pass
shutil.rmtree("/tmp/test-package-2", ignore_errors=True)

print(json.dumps(response))

#sys.exit(0)