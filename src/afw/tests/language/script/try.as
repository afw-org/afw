#!/usr/bin/env -S afw --syntax test_script
//? testScript: try.as
//? customPurpose: Part of miscellaneous category tests
//? description: Additional try tests, some accessing error object and data.
//? sourceType: script
//?
//? test: Try-1
//? description: catch and finally
//? expect: true
//? source: ...
#!/usr/bin/env afw

let catch_entered = false;
let finally_entered = false;

try {
    let x = 1;
    x = 1 / 0;
}
catch {
    catch_entered = true;
}
finally {
    finally_entered = true;
}

return catch_entered && finally_entered;

//?
//? test: Try-2
//? description: catch and !finally
//? expect: true
//? source: ...
#!/usr/bin/env afw

let catch_entered = false;
let finally_entered = false;

try {
    let x = 1;
    x = 1 / 0;
}
catch {
    catch_entered = true;
}

return catch_entered && !finally_entered;

//?
//? test: Try-3
//? description: !catch and finally
//? expect: error:Integer divide by zero error
//? source: ...
#!/usr/bin/env afw

let catch_entered = false;
let finally_entered = false;

try {
    let x = 1;
    x = 1 / 0;
}
finally {
    finally_entered = true;
}

return !catch_entered && finally_entered;

//?
//? test: Try-4
//? description: no error: return from try block
//? expect: "I tried!"
//? source: ...
#!/usr/bin/env afw

try {
    return "I tried!";
}
catch {
    return "I failed!";
}
finally {
    // Nothing to see here.
}

//?
//? test: Try-5
//? description: error: return from catch block
//? expect: "I failed!"
//? source: ...
#!/usr/bin/env afw

try {
    let x;
    x = 1 / 0;
    return "I tried!";
}
catch {
    return "I failed!";
}
finally {
    // Nothing to see here.
}

//?
//? test: Try-6
//? description: no error: return from try block
//? expect: "I tried!"
//? source: ...
#!/usr/bin/env afw

try {
    let x;
    return "I tried!";
    x = 1 / 0;
}
catch {
    return "I failed!";
}
finally {
    // Nothing to see here.
}

//?
//? test: Try-7
//? description: error: return in finally overrides all
//? expect: "Returned finally!"
//? source: ...
#!/usr/bin/env afw

try {
    let x;
    x = 1 / 0;
    return "I tried!";
}
catch {
    return "I failed!";
}
finally {
    return "Returned finally!";
}

//?
//? test: Try-8
//? description: error: return in finally overrides all
//? expect: "Returned finally!"
//? source: ...
#!/usr/bin/env afw

try {
    let x;
    x = 1 / 0;
    return "I tried!";
}
catch {
    return "I failed!";
}
finally {
    return "Returned finally!";
}

//?
//? test: Try-9
//? description: error: return e.message
//? expect: "Integer divide by zero error"
//? source: ...
#!/usr/bin/env afw

try {
    let x;
    x = 1 / 0;
}
catch (e) {
    return e.message;
}

//?
//? test: Try-10
//? description: throw: return e.message
//? expect: "Throw it all away!"
//? source: ...
#!/usr/bin/env afw

try {
    throw "Throw it all away!";
}
catch (e) {
    return e.message;
}


//?
//? test: Try-11
//? description: throw with data: return e.data.more
//? expect: "I have more to say!"
//? source: ...
#!/usr/bin/env afw

try {
    // throw can have an data expression of any type that will be
    // available in the "data" property of the error object.
    throw "Throw it all away!" {'more':'I have more to say!'};
}
catch (e) {
    return e.data.more;
}


//?
//? test: Try-13
//? description: throw with syntax error
//? expect: error:Parse error at offset 88 around line 4 column 63: Expecting statement terminator ';'
//? source: ...
#!/usr/bin/env afw

try {
    throw "Throw it all away!" {'more':'I have more to say!'} xyz;
}
catch (e) {
    return e.data.more;
}


//?
//? test: Try-14
//? description: another error in catch block
//? expect: error:Integer divide by zero error
//? source: ...
#!/usr/bin/env afw

try {
    throw "Throw it all away!" {'more':'I have more to say!'};
}
catch (e) {
    let x;
    x = 1 / 0;
}

//?
//? test: Try-15
//? description: nested try error inner
//? expect: true
//? source: ...
#!/usr/bin/env afw

let catch_outer_entered = false;
let finally_outer_entered = false;
let catch_inner_entered = false;
let finally_inner_entered = false;

try {
    let x;

    try {
        x = 1 / 0;
    }
    catch {
        catch_inner_entered = true;
    }
    finally {
        finally_inner_entered = true;
    }
}
catch {
    catch_outer_entered = true;
}
finally {
    finally_outer_entered = true;
}

return !catch_outer_entered && finally_outer_entered && 
       catch_inner_entered && finally_inner_entered;

//?
//? test: Try-16
//? description: nested try error outer
//? expect: true
//? source: ...
#!/usr/bin/env afw

let catch_outer_entered = false;
let finally_outer_entered = false;
let catch_inner_entered = false;
let finally_inner_entered = false;

try {
    let x;
    x = 1 / 0;

    try {
        x = 1 / 0;
    }
    catch {
        catch_inner_entered = true;
    }
    finally {
        finally_inner_entered = true;
    }
}
catch {
    catch_outer_entered = true;
}
finally {
    finally_outer_entered = true;
}

return catch_outer_entered && finally_outer_entered && 
       !catch_inner_entered && !finally_inner_entered;


//?
//? test: Try-17
//? description: nested try no error
//? expect: true
//? source: ...
#!/usr/bin/env afw

let catch_outer_entered = false;
let finally_outer_entered = false;
let catch_inner_entered = false;
let finally_inner_entered = false;

try {
    let x;

    try {
        
    }
    catch {
        catch_inner_entered = true;
    }
    finally {
        finally_inner_entered = true;
    }
}
catch {
    catch_outer_entered = true;
}
finally {
    finally_outer_entered = true;
}

return !catch_outer_entered && finally_outer_entered && 
       !catch_inner_entered && finally_inner_entered;

//?
//? test: Try-18
//? description: return in catch returns immediately after try.
//? expect: "Throw it all away!"
//? source: ...
#!/usr/bin/env afw

try {
    throw "Throw it all away!";
}
catch (e) {
    return e.message;
}

// This return should be skipped.
return "How did I get here!";


//?
//? test: Try-19
//? description: return in finally returns immediately after try.
//? expect: "Returned finally!"
//? source: ...
#!/usr/bin/env afw

try {
    throw "Throw it all away!";
}
catch (e) {
    return e.message;
}
finally {
    return "Returned finally!";
}

// This return should be skipped.
return "How did I get here!";

//?
//? test: Try-20
//? description: break in try should work.
//? expect: 5
//? source: ...
#!/usr/bin/env afw

let count;

for (count = 1; count < 10; count += 1) {
    try {
        if (count == 5) {
            break;
        }
        throw "Throw it all away!";
    }
    catch (e) {
        
    }
    finally {
        
    }
}

return count;

//?
//? test: Try-21
//? description: break in catch should work.
//? expect: 3
//? source: ...
#!/usr/bin/env afw

let count;

for (count = 1; count < 10; count += 1) {
    try {
        if (count == 5) {
            break;
        }
        throw "Throw it all away!";
    }
    catch (e) {
        if (count == 3) {
            break;
        }        
    }
    finally {
        
    }
}

return count;

//?
//? test: Try-22
//? description: break in finally should work on error.
//? expect: 2
//? source: ...
#!/usr/bin/env afw

let count;

for (count = 1; count < 10; count += 1) {
    try {
        if (count == 5) {
            break;
        }
        throw "Throw it all away!";
    }
    catch (e) {
        if (count == 3) {
            break;
        }        
    }
    finally {
        if (count == 2) {
            break;
        }                
    }
}

return count;


//?
//? test: Try-23
//? description: break in finally should work on no error.
//? expect: 2
//? source: ...
#!/usr/bin/env afw

let count;

for (count = 1; count < 10; count += 1) {
    try {
        if (count == 5) {
            break;
        }
    }
    catch (e) {
        if (count == 3) {
            break;
        }        
    }
    finally {
        if (count == 2) {
            break;
        }                
    }
}

return count;

//?
//? test: Try-24
//? description: break in nested try should work.
//? expect: 2
//? source: ...
#!/usr/bin/env afw

let count;

for (count = 1; count < 10; count += 1) {
    try {
        if (count == 5) {
            break;
        }
        try {
            if (count == 5) {
                break;
            }
        }
        catch (e) {
            if (count == 4) {
                break;
            }        
        }
        finally {
            if (count == 2) {
                break;
            }                
        }
    }
    catch (e) {
        if (count == 4) {
            break;
        }        
    }
    finally {
        if (count == 3) {
            break;
        }                
    }
}

return count;

//?
//? test: Try-25
//? description: continue in body
//? expect: 10
//? source: ...
#!/usr/bin/env afw

let count;

for (count = 1; count < 10; count += 1) {
    try {
        continue;
    }
    catch {
        break;
    }
    break; // Should not get here
}

return count;

//?
//? test: Try-26
//? description: continue in catch
//? expect: 10
//? source: ...
#!/usr/bin/env afw

let count;

for (count = 1; count < 10; count += 1) {
    try {
        let x = 1 / 0;
    }
    catch {
        continue;
    }
    break; // Should not get here
}

return count;

//?
//? test: Try-27
//? description: continue in finally
//? expect: 10
//? source: ...
#!/usr/bin/env afw

let count;

for (count = 1; count < 10; count += 1) {
    try {
        let x = 1 / 0;
        break; // Finally continue overrides
    }
    catch {
        break; // Finally continue overrides
    }
    finally {
        continue;
    }
    break; // Should not get here
}

return count;

//?
//? test: Try-28
//? description: try inside of a loop in a catch block
//? expect: true
//? source: ...
#!/usr/bin/env afw

let catch_outer_entered = false;
let finally_outer_entered = false;
let catch_inner_entered = false;
let finally_inner_entered = false;

try {
    let x = 1;
    x = 1 / 0;
}
catch {
    catch_outer_entered = true;
    while (true) {
        try {
            while (true) {
                throw "Inner throw";
                break;
            }
        }
        catch (e) {
            catch_inner_entered = true;
        }
        finally {
            finally_inner_entered = true;
        }
        break;
    }
}
finally {
    finally_outer_entered = true;
}

return catch_outer_entered && finally_outer_entered && 
       catch_inner_entered && finally_inner_entered;

//?
//? test: Try-29
//? description: try inside of a loop in a catch block with rethrow
//? expect: error:Inner throw
//? source: ...
#!/usr/bin/env afw

let catch_outer_entered = false;
let finally_outer_entered = false;
let catch_inner_entered = false;
let finally_inner_entered = false;

try {
    let x = 1;
    x = 1 / 0;
}
catch {
    catch_outer_entered = true;
    while (true) {
        try {
            while (true) {
                throw "Inner throw";
                break;
            }
        }
        catch (e) {
            catch_inner_entered = true;
            throw;
        }
        finally {
            finally_inner_entered = true;
        }
        break;
    }
}
finally {
    finally_outer_entered = true;
}

return catch_outer_entered && finally_outer_entered && 
       catch_inner_entered && finally_inner_entered;

//?
//? test: Try-30
//? description: catch and rethrow since not throw statement
//? expect: error:Integer divide by zero error
//? source: ...
#!/usr/bin/env afw

let catch_entered = false;

try {
    let x = 1;
    x = 1 / 0;
}
catch (e) {
    /* Only handle if thrown by throw statement. */
    if (e.id != 'throw') {
        throw;
    }
    catch_entered = true;
}

return catch_entered;


//?
//? test: Try-31
//? description: catch and don't rethrow since throw statement
//? expect: true
//? source: ...
#!/usr/bin/env afw

let catch_entered = false;

try {
    throw "Throw it all away!";
}
catch (e) {
    /* Only handle if thrown by throw statement. */
    if (e.id != 'throw') {
        throw;
    }
    catch_entered = true;
}

return catch_entered;

//?
//? test: Try-32
//? description: rethrow must be in catch block
//? expect: error:Parse error at offset 92 around line 6 column 6: Can only rethrow ("throw;") inside a catch block
//? source: ...
#!/usr/bin/env afw

let catch_entered = false;

// Rethrow ("throw;") can not be here.
throw;

try {
    throw "Throw it all away!";
}
catch (e) {
    /* Only handle if thrown by throw statement. */
    if (e.id != 'throw') {
        throw;
    }
    catch_entered = true;
}

return catch_entered;


//?
//? test: Try-33
//? description: throw must be in catch block but can't be in inner try block
//? expect: error:Parse error at offset 343 around line 17 column 18: Can only rethrow ("throw;") inside a catch block
//? source: ...
#!/usr/bin/env afw

let catch_outer_entered = false;
let finally_outer_entered = false;
let catch_inner_entered = false;
let finally_inner_entered = false;

try {
    let x = 1;
    x = 1 / 0;
}
catch {
    catch_outer_entered = true;
    while (true) {
        try {
            // Rethrow ("throw;") can not be here either.
            throw;
            while (true) {
                throw "Inner throw";
                break;
            }
        }
        catch (e) {
            catch_inner_entered = true;
            throw;
        }
        finally {
            finally_inner_entered = true;
        }
        break;
    }
}
finally {
    finally_outer_entered = true;
}

return catch_outer_entered && finally_outer_entered && 
       catch_inner_entered && finally_inner_entered;
