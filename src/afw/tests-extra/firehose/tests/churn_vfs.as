const id = "churn-" + generate_uuid() + ".txt";
add_object("vfs", "_AdaptiveFile_vfs", {
    "data": "temp"
}, id);
const f = get_object("vfs", "_AdaptiveFile_vfs", id);
assert(f.data === "temp");
delete_object("vfs", "_AdaptiveFile_vfs", id);
return true;
