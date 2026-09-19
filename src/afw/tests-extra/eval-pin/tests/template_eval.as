const t = evaluate(compile<template>(template("A${1+2}B")));
assert(t === "A3B", t);
const t2 = evaluate(compile<template>(template("x${\"y\"}z")));
assert(t2 === "xyz");
return true;
