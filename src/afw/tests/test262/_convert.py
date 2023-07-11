#!/usr/bin/env python3

# temporary script to convert test262 .js conformance tests
# into .as test scripts

import sys
import re


test = sys.argv[1].replace(".js", "")
testInput = sys.argv[1]

if test == "convert.py":
    sys.exit(0)

if "bigint" in test:
    sys.exit(0)

if "-class" in test:
    sys.exit(0)

with open(testInput, "r") as fd:
    lines = fd.readlines()

code = ""
description = ""
descriptions = []
multiLineDescr = False
inComments = False
inCode = False
inDescr = False
skip = False
expectError = False
finishedTopComments = False

for line in lines:
    stripped = line.rstrip()

    if stripped.startswith("description: >"):
        multiLineDescr = True
        inDescr = True

    elif stripped.startswith("description: "):
        description = stripped.replace("description: ", "")

    elif inDescr and multiLineDescr and stripped.startswith("    "):
        descriptions.append(stripped)
    else:
        inDescr = False

    if not finishedTopComments and "/*" in stripped:
        inComments = True

    elif not finishedTopComments and "*/" in stripped and inComments:
        inComments = False
        finishedTopComments = True
        inCode = True

    elif inCode:
        converted = stripped

        if "throw new Test262Error(" in converted:
            converted = converted.replace("throw new Test262Error(", "throw ")
            # replace last ) with ;
            converted = converted[::-1].replace(")", "", 1)[::-1]

        #if "eval(" in converted:
        #    converted = re.sub(r'(eval(":[.*]"))', r'evaluate(script("\1"))')

        if "var " in converted:
            converted = converted.replace("var ", "loc ")

        if "Number.NaN" in converted:
            converted = converted.replace("Number.NaN", "NaN");

        if "Number.NEGATIVE_INFINITY" in converted:
            converted = converted.replace("Number.NEGATIVE_INFINITY", "-Infinity")

        if "Number.POSITIVE_INFINITY" in converted:
            converted = converted.replace("Number.POSITIVE_INFINITY", "Infinity")

        if "isNaN" in converted:
            skip = True

        if "Number.MIN_VALUE" in converted:
            skip = True

        if "Number.MAX_VALUE" in converted:
            skip = True

        if "Math." in converted:
            skip = True

        if "$DONOTEVALUATE();" in converted:
            expectError = True
            converted = ""

        # convert assert.sameValue(x, y) to assert(x == y)
        if "assert.sameValue(" in converted:
            converted = converted.replace("assert.sameValue(", "assert(")
            converted = converted.replace(", ", " === ")

        code += converted + "\n"


print("//? test: " + test)
if multiLineDescr:
    print("//? description:...")
    for d in descriptions:
        print(d)
else:
    print("//? description: " + description)
if skip:
    print("//? skip: true")
if expectError:
    print("//? expect: error")
else:
    print("//? expect: null")

print("//? source: ...")
print("#!/usr/bin/env afw")
print("")
print(code)
print("")
