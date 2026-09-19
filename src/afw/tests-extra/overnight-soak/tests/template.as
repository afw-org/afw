const t = evaluate(compile<template>(template("A${1+2}B")));
assert(t === "A3B");
return true;
