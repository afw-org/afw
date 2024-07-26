#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: service_get.as
//? customPurpose: Part of administrative category tests
//? description: Test administrative service_get function
//? sourceType: script
//?
//? test: service_get-nonexistent
//? description: Test service_get when it does
//? skip: false
//? expect: 0
//? source: ...

assert(
    safe_evaluate(
        service_get("nonexistent"),
        "error"
    ) === "error",
    "service_get should return an error for nonexisent service"
);

return 0;

//? test: service_get-afw
//? description: Test service_get on afw service
//? skip: false
//? expect: 0
//? source: ...

let service: object = service_get("adapter-afw");

assert(service.serviceId === "adapter-afw", "serviceId should be 'adapter-afw'");

return 0;