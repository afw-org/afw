const objects = retrieve_objects("file", "Demo",
    undefined, undefined, undefined, 0);
assert(length(objects) == 3);
return length(objects);
