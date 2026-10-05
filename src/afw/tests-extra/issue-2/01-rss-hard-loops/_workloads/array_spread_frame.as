/* #2 lab: array spread held in the body frame. Should stay flat. */
const pair = [1, 2];
while (true) {
    const a = [0, ...pair];
}
return 0;
