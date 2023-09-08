#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: functions/any_of.as
//? customPurpose: Part of core function tests
//? description: Tests for any_of function.
//? sourceType: script
//?
//? test: any_of-1
//? description: ...

        Test any_of with arguments:
        urn:oasis:names:tc:xacml:1.0:function:string-equal,
        'This is it.',
        { 'This is not it.', 'This is it.' }
    
//? expect: boolean(true)
//? source: ...

any_of(
    "eq<string>",
    string("This is it."),
    bag<string>(
        string("This is not it."),
        string("This is it.")
    )
)

//?
//? test: any_of-2
//? description: ...

        Test any_of with arguments:
        urn:oasis:names:tc:xacml:1.0:function:string-equal,
        'This is it.',
        { 'This is not it.', 'This is it.', 'Weve already found it..', 'So, these shouldnt be checked' }
    
//? expect: boolean(true)
//? source: ...

any_of(
    "eq<string>",
    string("This is it."),
    bag<string>(
        string("This is not it."),
        string("This is it."),
        string("Weve already found it.."),
        string("So, these shouldnt be checked")
    )
)

//?
//? test: any_of-3
//? description: ...

        Test any_of with arguments:
        urn:oasis:names:tc:xacml:1.0:function:string-equal,
        { 'This is not it.', 'This is it.' }
        'This is it.'
    
//? expect: boolean(true)
//? source: ...

any_of(
    "eq<string>",
    bag<string>(
        string("This is not it."),
        string("This is it.")
    ),
    string("This is it.")
)

//?
//? test: any_of-4
//? description: ...

        Test any_of with arguments:
        urn:oasis:names:tc:xacml:1.0:function:string-equal,
        'This is it.',
        { 'This is not it.', 'This is not it either.' }
    
//? expect: boolean(false)
//? source: ...

any_of(
    "eq<string>",
    string("This is it."),
    bag<string>(
        string("This is not it."),
        string("This is not it either.")
    )
)

//?
//? test: any_of-5
//? description: ...

        Test any_of with arguments:
        urn:oasis:names:tc:xacml:1.0:function:string-equal,
        'This is it.',
        { 'This is not it.' }
    
//? expect: boolean(false)
//? source: ...

any_of(
    "eq<string>",
    string("This is it."),
    bag<string>(
        string("This is not it.")
    )
)

//?
//? test: any_of-6
//? description: ...

        Test any_of with arguments:
        urn:oasis:names:tc:xacml:1.0:function:string-equal,
        'This is it.',
        { 'This is it.' }
    
//? expect: boolean(true)
//? source: ...

any_of(
    "eq<string>",
    string("This is it."),
    bag<string>(
        string("This is it.")
    )
)

