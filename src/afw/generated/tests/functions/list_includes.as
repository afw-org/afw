#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/list_includes.as
//? customPurpose: Part of core function tests
//? description: Tests for includes<list> function.
//? sourceType: expression
//?
//? test: includes<list>-1
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 'one')
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-2
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 'two')
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-3
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], [1,2])
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-4
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], [1,2,3])
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-5
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], [3,1,2])
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-6
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], [1,2,3,4])
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-7
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-8
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 2)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-9
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 3)
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-10
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], { two: 'two', three: 'three' })
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-11
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], { three: 'three', two: 'two' })
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-12
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], { four: 'four', three: 'three', two: 'two' })
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-13
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 0)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-14
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 1)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-15
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 2)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-16
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 3)
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-17
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 4)
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-18
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 5)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-19
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 6)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-20
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 7)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-21
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 8)
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-22
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 9)
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-23
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, 10)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-24
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -0)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-25
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -1)
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-26
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -2)
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-27
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -3)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-28
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -4)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-29
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -5)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-30
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -6)
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-31
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -7)
//? expect: boolean(false)
//? source: ...

includes<list>(
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
//? test: includes<list>-32
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -8)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-33
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -9)
//? expect: boolean(true)
//? source: ...

includes<list>(
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
//? test: includes<list>-34
//? description: ...
includes(['one', { two: 'two', three: 'three' }, 1, 2, [1,2,3]], 1, -10)
//? expect: boolean(true)
//? source: ...

includes<list>(
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

