const f = get_object("vfs", "_AdaptiveFile_vfs", "hello.txt");
assert(f.data === "hello\n");
return true;
