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
assert(meta(p.afwVersion).dataType === "string");
assert(length(p.afwVersion) > 0);
assert(meta(p.startTime).dataType === "dateTime");
assert(meta(p.poolBytesInUse).dataType === "integer");
assert(p.poolBytesInUse >= 0);
assert(meta(p.peakPoolBytesInUse).dataType === "integer");
assert(p.peakPoolBytesInUse >= p.poolBytesInUse);
assert(meta(p.poolChunkBytes).dataType === "integer");
assert(p.poolChunkBytes >= 0);
assert(meta(p.peakPoolChunkBytes).dataType === "integer");
assert(p.peakPoolChunkBytes >= p.poolChunkBytes);
assert(meta(p.rss).dataType === "integer");
assert(p.rss >= 0);
assert(meta(p.limitEvaluationStackCount).dataType === "integer");
assert(p.limitEvaluationStackCount === 500);
assert(meta(p.limitRequestPoolBytes).dataType === "integer");
assert(p.limitRequestPoolBytes === 67108864);
assert(meta(p.limitCStackHeadroomBytes).dataType === "integer");
assert(p.limitCStackHeadroomBytes === 262144);
assert(meta(p.chunkMin).dataType === "integer");
assert(p.chunkMin === 65536);
assert(meta(p.compileChunkMin).dataType === "integer");
assert(p.compileChunkMin === 4096);
assert(meta(p.xctxChunkMin).dataType === "integer");
assert(p.xctxChunkMin === 65536);
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
assert(process::afwVersion === p.afwVersion);
assert(process::startTime === p.startTime);
assert(length(process::args) === length(p.args));
assert(process::args[0] === p.args[0]);
assert(meta(process::poolBytesInUse).dataType === "integer");
/* Capture current first: a live peak>=cur expression can lose if
 * reading peak allocates before reading current. */
let cur = process::poolBytesInUse;
let peak = process::peakPoolBytesInUse;
assert(peak >= cur);
cur = process::poolChunkBytes;
peak = process::peakPoolChunkBytes;
assert(peak >= cur);
assert(meta(process::rss).dataType === "integer");
assert(process::rss >= 0);
assert(process::limitEvaluationStackCount === 500);
assert(process::chunkMin === 65536);
assert(process::memoryRegionFreeListMaxBytes === 262144);
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
