#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/array_includes.as
//? customPurpose: Part of core function tests
//? description: Tests for includes<array> function.
//? sourceType: expression
//?
//? test: includes<array>-1
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 'one')
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    string("one")
)

//?
//? test: includes<array>-2
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 'two')
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    string("two")
)

//?
//? test: includes<array>-3
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], [1,2])
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    [
    1,
    2
]
)

//?
//? test: includes<array>-4
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], [1,2,3])
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    [
    1,
    2,
    3
]
)

//?
//? test: includes<array>-5
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], [3,1,2])
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    [
    3,
    1,
    2
]
)

//?
//? test: includes<array>-6
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], [1,2,3,4])
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    [
    1,
    2,
    3,
    4
]
)

//?
//? test: includes<array>-7
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1)
)

//?
//? test: includes<array>-8
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 2)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(2)
)

//?
//? test: includes<array>-9
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 3)
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(3)
)

//?
//? test: includes<array>-10
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], { two: 'two', three: 'three' })
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    object({
    "two": "two",
    "three": "three"
})
)

//?
//? test: includes<array>-11
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], { three: 'three', two: 'two' })
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    object({
    "three": "three",
    "two": "two"
})
)

//?
//? test: includes<array>-12
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], { four: 'four', three: 'three', two: 'two' })
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    object({
    "four": "four",
    "three": "three",
    "two": "two"
})
)

//?
//? test: includes<array>-13
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 0)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(0)
)

//?
//? test: includes<array>-14
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 1)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(1)
)

//?
//? test: includes<array>-15
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 2)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(2)
)

//?
//? test: includes<array>-16
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 3)
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(3)
)

//?
//? test: includes<array>-17
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 4)
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(3)
)

//?
//? test: includes<array>-18
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 5)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(5)
)

//?
//? test: includes<array>-19
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 6)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(6)
)

//?
//? test: includes<array>-20
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 7)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(7)
)

//?
//? test: includes<array>-21
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 8)
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(8)
)

//?
//? test: includes<array>-22
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 9)
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(9)
)

//?
//? test: includes<array>-23
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 10)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(10)
)

//?
//? test: includes<array>-24
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -0)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-0)
)

//?
//? test: includes<array>-25
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -1)
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-1)
)

//?
//? test: includes<array>-26
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -2)
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-2)
)

//?
//? test: includes<array>-27
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -3)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-3)
)

//?
//? test: includes<array>-28
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -4)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-4)
)

//?
//? test: includes<array>-29
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -5)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-5)
)

//?
//? test: includes<array>-30
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -6)
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-6)
)

//?
//? test: includes<array>-31
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -7)
//? expect: boolean(false)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-7)
)

//?
//? test: includes<array>-32
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -8)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-8)
)

//?
//? test: includes<array>-33
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -9)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-9)
)

//?
//? test: includes<array>-34
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -10)
//? expect: boolean(true)
//? source: ...

includes<array>(
    [
    "one",
    {
        "two": "two",
        "three": "three"
    },
    1,
    2,
    [
        1,
        2,
        3
    ]
],
    integer(1),
    integer(-10)
)

