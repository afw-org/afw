#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: log_conf_context_type.as
//? customPurpose: Log conf format/filter contextType + property parentPaths inheritance
//? description: Specialized _AdaptiveConf_log_* set contextType logType-*; inherit format/filter meta
//? sourceType: script
//?
//? test: logType-standard-context-registered
//? description: runtime context type logType-standard has log current and parents application
//? expect: 0
//? source: ...

const ct = get_object("afw", "_AdaptiveContextType_", "logType-standard");
assert(ct !== null && ct !== undefined, "logType-standard contextType required");
assert(ct.contextTypeId === "logType-standard" ||
    ct._meta_.objectId === "logType-standard");
const qd = ct.qualifierDefinitions;
assert(qd !== null && qd !== undefined);
assert(qd.current !== undefined, "log current:: bag");
assert(qd.current.message !== undefined, "current::message");
assert(qd.current.source !== undefined, "current::source");
assert(qd.current.xctxUUID !== undefined, "current::xctxUUID");
assert(qd.log !== undefined, "log:: from conf OT");
const m = meta(qd);
assert(m.parentPaths !== undefined, "should parent application qualifiers");
assert(length(m.parentPaths) >= 1);
const expect = "/afw/_AdaptiveContextType_/application/qualifierDefinitions";
assert(m.parentPaths[0] === expect || string(m.parentPaths[0]) === expect,
    "parent path should be application qualifierDefinitions");
return 0;


//?
//? test: log-conf-standard-format-filter-contextType
//? description: conf OT standard format/filter declare contextType logType-standard
//? expect: 0
//? source: ...

const ot = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveConf_log_standard");
assert(ot !== null && ot !== undefined);
const pts = ot.propertyTypes;
assert(pts.format !== undefined);
assert(pts.filter !== undefined);
assert(pts.format.contextType === "logType-standard",
    "format.contextType should be logType-standard");
assert(pts.filter.contextType === "logType-standard",
    "filter.contextType should be logType-standard");
/* Without composite, inherited fields are not local. */
assert(pts.format.dataType === undefined || pts.format.dataType === null,
    "format.dataType should come from parent path, not local bag");
return 0;


//?
//? test: log-conf-standard-format-inherits-via-composite
//? description: composite resolves format meta from parent propertyTypes/format
//? expect: 0
//? source: ...

const ot = get_object(
    "afw",
    "_AdaptiveObjectType_",
    "_AdaptiveConf_log_standard",
    { composite: true }
);
const f = ot.propertyTypes.format;
assert(f.contextType === "logType-standard");
assert(f.dataType === "template", "inherited dataType template");
assert(f.dataTypeParameter === "string");
assert(f.brief === "Log format" || length(string(f.brief)) > 0);
assert(f.label === "Format" || length(string(f.label)) > 0);
assert(length(string(f.description)) > 0, "inherited description");

const filt = ot.propertyTypes.filter;
assert(filt.contextType === "logType-standard");
assert(filt.dataType === "script", "inherited dataType script");
assert(filt.dataTypeParameter === "boolean");
return 0;


//?
//? test: log-conf-standard-format-parentPaths
//? description: format property meta parentPaths points at parent conf log format
//? expect: 0
//? source: ...

const ot = get_object("afw", "_AdaptiveObjectType_", "_AdaptiveConf_log_standard");
const f = ot.propertyTypes.format;
const m = meta(f);
assert(m.parentPaths !== undefined, "format should have parentPaths on meta");
assert(length(m.parentPaths) >= 1);
const expect =
    "/afw/_AdaptiveObjectType_/_AdaptiveConf_log/propertyTypes/format";
assert(m.parentPaths[0] === expect || string(m.parentPaths[0]) === expect,
    "format parentPaths should point at _AdaptiveConf_log format");
return 0;


//?
//? test: log-conf-syslog-format-filter-contextType
//? description: conf OT syslog format/filter use contextType logType-syslog
//? expect: 0
//? source: ...

const ot = get_object(
    "afw",
    "_AdaptiveObjectType_",
    "_AdaptiveConf_log_syslog",
    { composite: true }
);
assert(ot.propertyTypes.format.contextType === "logType-syslog");
assert(ot.propertyTypes.filter.contextType === "logType-syslog");
assert(ot.propertyTypes.format.dataType === "template");
assert(ot.propertyTypes.filter.dataType === "script");
/* syslog-specific props still present */
assert(ot.propertyTypes.facility !== undefined ||
    ot.propertyTypes.ident !== undefined);
return 0;


//?
//? test: log-conf-event_log-format-filter-contextType
//? description: conf OT event_log format/filter use contextType logType-event_log
//? expect: 0
//? source: ...

const ot = get_object(
    "afw",
    "_AdaptiveObjectType_",
    "_AdaptiveConf_log_event_log",
    { composite: true }
);
assert(ot.propertyTypes.format.contextType === "logType-event_log");
assert(ot.propertyTypes.filter.contextType === "logType-event_log");
assert(ot.propertyTypes.format.dataType === "template");
assert(ot.propertyTypes.filter.dataType === "script");
return 0;


//?
//? test: logType-syslog-context-registered-on-linux
//? description: logType-syslog context type exists when OS registered syslog
//? expect: 0
//? source: ...

/*
 * On Linux, os init registers syslog log type and its context type.
 * Skip-friendly: if not present (e.g. Windows-only event_log host), pass.
 */
const ct = get_object("afw", "_AdaptiveContextType_", "logType-syslog");
if (ct === null || ct === undefined) {
    return 0;
}
assert(ct.qualifierDefinitions !== undefined);
assert(ct.qualifierDefinitions.current !== undefined);
assert(ct.qualifierDefinitions.current.message !== undefined);
return 0;
