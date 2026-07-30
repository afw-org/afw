#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: process.as
//? customPurpose: Process ambient qualifiers (environment:: / process::) at env create
//? description: Issue #74 partial + env create process object; issue #71 single env current
//? sourceType: script
//?
//? test: process-object-get
//? description: /afw/_AdaptiveProcess_/current has programName, args, and identity fields
//? expect: 0
//? source: ...

const p = get_object("afw", "_AdaptiveProcess_", "current");
assert(p !== null && p !== undefined, "process current required");
assert(meta(p.programName).dataType === "string");
assert(length(p.programName) > 0, "programName non-empty");
assert(meta(p.args).dataType === "array");
assert(length(p.args) >= 1, "args at least program path");
assert(meta(p.args[0]).dataType === "string");
assert(meta(p.pid).dataType === "integer");
assert(p.pid > 0, "pid positive");
assert(meta(p.cwd).dataType === "string");
assert(length(p.cwd) > 0, "cwd non-empty");
assert(meta(p.platform).dataType === "string");
assert(p.platform === "linux" || p.platform === "windows");
assert(meta(p.afwVersion).dataType === "string");
assert(length(p.afwVersion) > 0);
assert(meta(p.startTime).dataType === "dateTime");
return 0;


//?
//? test: process-qualifier
//? description: process:: matches get_object AdaptiveProcess current
//? expect: 0
//? source: ...

const p = get_object("afw", "_AdaptiveProcess_", "current");
assert(process::programName === p.programName);
assert(process::pid === p.pid);
assert(process::cwd === p.cwd);
assert(process::platform === p.platform);
assert(process::afwVersion === p.afwVersion);
assert(process::startTime === p.startTime);
assert(length(process::args) === length(p.args));
assert(process::args[0] === p.args[0]);
return 0;


//?
//? test: process-args-includes-program
//? description: process::args[0] path ends with programName (issue #74)
//? expect: 0
//? source: ...

const name = process::programName;
const a0 = process::args[0];
assert(ends_with<string>(a0, name) || a0 === name,
    "args[0] should end with programName");
return 0;


//?
//? test: current-pid-programName-deprecated-compat
//? description: current::pid/programName still match process:: (deprecated, still work)
//? expect: 0
//? source: ...

assert(current::pid === process::pid, "deprecated current::pid matches process::pid");
assert(current::programName === process::programName,
    "deprecated current::programName matches process::programName");
return 0;


//?
//? test: environment-still-one-current
//? description: issue #71 still one AdaptiveEnvironmentVariables current
//? expect: 0
//? source: ...

const objs = retrieve_objects("afw", "_AdaptiveEnvironmentVariables_");
assert(length(objs) === 1, "expected one current, got " + string(length(objs)));
assert(environment::HOME === get_object("afw", "_AdaptiveEnvironmentVariables_", "current").HOME);
return 0;


//?
//? test: process-context-type-registered
//? description: context type process is registered for builder docs
//? expect: 0
//? source: ...

const ct = get_object("afw", "_AdaptiveContextType_", "process");
assert(ct !== null && ct !== undefined, "process contextType required");
assert(ct.contextTypeId === "process" || ct._meta_.objectId === "process");
const qd = ct.qualifierDefinitions;
assert(qd !== null && qd !== undefined);
assert(qd.environment !== undefined);
assert(qd.process !== undefined);
assert(qd.process.args !== undefined || qd.process.programName !== undefined);
return 0;


//?
//? test: application-context-parents-process
//? description: application contextType parents process qualifierDefinitions
//? expect: 0
//? source: ...

const app = get_object("afw", "_AdaptiveContextType_", "application");
assert(app !== null && app !== undefined);
const qd = app.qualifierDefinitions;
assert(qd !== null && qd !== undefined);
const m = meta(qd);
assert(m.parentPaths !== undefined, "application qualifierDefinitions should have parentPaths");
assert(length(m.parentPaths) >= 1);
const expect = "/afw/_AdaptiveContextType_/process/qualifierDefinitions";
assert(m.parentPaths[0] === expect || string(m.parentPaths[0]) === expect,
    "parent path should be process contextType qualifierDefinitions");
return 0;


//?
//? test: conf-template-can-use-process-cwd
//? description: process::cwd is available for conf-time style templates (runtime check)
//? expect: 0
//? source: ...

/* Same ambient bag conf templates use after env create (issue #15 / #74). */
const joined = string(process::cwd, "/", process::programName);
assert(length(joined) > length(process::programName));
assert(starts_with<string>(joined, process::cwd));
return 0;
