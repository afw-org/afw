#!/usr/bin/env -S afw --syntax test_script --conf ./afw.conf
//?
//? testScript: service_restart_compiled_conf.as
//? customPurpose: Pool last-release order regression
//? description: Restart a file adapter whose root compiled into its pool
//? sourceType: script
//?
//? test: region-cache-off
//? description: The application conf turns the region free list off
//? expect: true
//? source: ...

return process::memoryRegionFreeListMaxBytes === 0;

//? test: restart-twice
//? description: Old adapter pool last-release runs its cleanups (the compiled root's last-release) before destroying that unit's child pool; with the free list off, the other order segfaults
//? expect: true
//? source: ...

let svc = service_restart('adapter-files');
assert(svc.serviceId === "adapter-files");
svc = service_restart('adapter-files');
assert(svc.serviceId === "adapter-files");
return true;
