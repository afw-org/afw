let seed = 7;
return function (delta) {
    seed = seed + delta;
    return seed;
};
