/* #2 lab: compile listing copies into dest p then last-releases
 * the unit. If the unit is abandoned at RC 1, this climbs like
 * compile-in-a-loop. compile() without listing is a unit pin
 * (evaluate(compile()) / closures); do not extra-hold that path.
 */
while (true) {
    compile<script>(script("return 1;"), 1);
    add(0, 0);
}
return 0;
