service_stop("adapter-model");
service_stop("adapter-backend");
let failed = false;
try {
    const started = service_start("adapter-model");
    if (started.status === "error") {
        failed = true;
    }
} catch (e) {
    failed = true;
}
assert(failed, "model start needs backend to load the model");
const back = service_start("adapter-backend");
assert(back.status === "running", back.statusMessage);
const model = service_start("adapter-model");
assert(model.status === "running", model.statusMessage);
return true;
