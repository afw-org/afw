#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: check_function_categories.as
//? customPurpose: Part of core function tests
//? description: Checks that every adaptive function belongs to a valid category.
//? sourceType: script
//?
//? test: check_function_categories
//? description: Check each function's category against an array of _AdaptiveFunctionCategory_ objects.
//? expect: 0
//? source: ...
#!/usr/bin/env afw

// retrieve our categories
const categories: object = retrieve_objects("dev", "_AdaptiveFunctionCategory_");

// retrieve our functions
const funcs: object = retrieve_objects("dev", "_AdaptiveFunctionGenerate_");
for (const func: object of funcs) {
    const {category} /* @todo :{category: string} */ = func;

    let found: boolean = false;
    for (const c: object of categories) {
        const {category: cat} = c;        
        
        if (category === cat) {
            found = true;
            break;
        }
    }

    assert(found, "Function category not found for " + category);
}

return 0;