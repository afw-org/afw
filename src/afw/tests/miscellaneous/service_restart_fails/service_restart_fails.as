#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: service_restart_fails.as
//? customPurpose: Part of core tests
//? description: A restart whose new instance fails throws and keeps the previous instance (#525)
//? sourceType: script
//?
//? test: failed-restart-throws-and-keeps-running
//? description: The new conf names an adapter type that does not exist
//? expect: true
//? source: ...

const conf = get_object("conf", "_AdaptiveServiceConf_", "adapter-things");
const good = clone(conf);
conf.conf.adapterType = "no-such-adapter-type";
replace_object("conf", "_AdaptiveServiceConf_", "adapter-things", conf);

let thrown = null;
try {
    service_restart("adapter-things");
}
catch (e) {
    thrown = e.message;
}

const svc = service_get("adapter-things");
const n = get_object("things", "Thing", "one").n;
replace_object("conf", "_AdaptiveServiceConf_", "adapter-things", good);

assert(thrown !== null, "service_restart did not throw");
assert(includes<string>(thrown, "was not restarted"), thrown);
assert(includes<string>(thrown, "no-such-adapter-type"), thrown);
assert(svc.status === "running", svc.status);
assert(svc.statusMessage === undefined, svc.statusMessage);
assert(n === 1);
return true;

//? test: restart-after-fix
//? description: With the conf fixed, a restart works
//? expect: true
//? source: ...

const svc = service_restart("adapter-things");
assert(svc.status === "running", svc.status);
assert(get_object("things", "Thing", "one").n === 1);
return true;
