#!/usr/bin/env -S afw --syntax test_script
//? testScript: try.as
//? customPurpose: Part of miscellaneous category tests
//? description: Test try statement
//? sourceType: script
//?
//? test: Try-1
//? description: catch and finally
//? expect: true
//? source: ...
#!/usr/bin/env afw

loc catch_entered = false;
loc finally_entered = false;

try {
    loc x = 1;
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

loc catch_entered = false;
loc finally_entered = false;

try {
    loc x = 1;
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

loc catch_entered = false;
loc finally_entered = false;

try {
    loc x = 1;
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
    loc x;
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
    loc x;
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
    loc x;
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
    loc x;
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
    loc x;
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
//? description: throw with additional: return e.additional.more
//? expect: "I have more to say!"
//? source: ...
#!/usr/bin/env afw

try {
    // throw can have an additional expression of any type that will be
    // available in the "additional" property of the error object.
    throw "Throw it all away!" {'more':'I have more to say!'};
}
catch (e) {
    return e.additional.more;
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
    return e.additional.more;
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
    loc x;
    x = 1 / 0;
}

//?
//? test: Try-15
//? description: nested try error inner
//? expect: true
//? source: ...
#!/usr/bin/env afw

loc catch_outer_entered = false;
loc finally_outer_entered = false;
loc catch_inner_entered = false;
loc finally_inner_entered = false;

try {
    loc x;

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

loc catch_outer_entered = false;
loc finally_outer_entered = false;
loc catch_inner_entered = false;
loc finally_inner_entered = false;

try {
    loc x;
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

loc catch_outer_entered = false;
loc finally_outer_entered = false;
loc catch_inner_entered = false;
loc finally_inner_entered = false;

try {
    loc x;

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
