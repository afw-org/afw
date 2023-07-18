#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: hanoi.as
//? customPurpose: Part of core function tests
//? description: Tests lambda function with recursive by solving Towers of Hanoi.
//? sourceType: script
//?
//? test: hanoi
//? description: Solve Towers of Hanoi.
//? expect: "Move disk from post 1 to 3\nMove disk from post 1 to 2\nMove disk from post 3 to 2\nMove disk from post 1 to 3\nMove disk from post 2 to 1\nMove disk from post 2 to 3\nMove disk from post 1 to 3\n"
//? source: ...
#!/usr/bin/env afw

(function hanoi(num, s, i, d)
     {
        loc result = "";
        if (num >= 1) {
            result += hanoi(num - 1, s, d, i);
            result += "Move disk from post " + s + " to " + d + "\n";
            result += hanoi(num - 1, i, s, d);
        }
        return result;
     })
(3, "1", "2", "3");

//?
//? test: hanoi_typed
//? description: Solve Towers of Hanoi test with types.
//? expect: "Move disk from post 1 to 3\nMove disk from post 1 to 2\nMove disk from post 3 to 2\nMove disk from post 1 to 3\nMove disk from post 2 to 1\nMove disk from post 2 to 3\nMove disk from post 1 to 3\n"
//? source: ...
#!/usr/bin/env afw

(function hanoi(num: integer, s: string, i: string, d: string): string
     {
        loc result: string = "";
        if (num >= 1) {
            result += hanoi(num - 1, s, d, i);
            result += "Move disk from post " + s +" to " + d + "\n";
            result += hanoi(num - 1, i, s, d);
         }
         return result;
     })
(3, "1", "2", "3");
