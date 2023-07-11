#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: throw.as
//? customPurpose: Part of test262
//? description: Tests the throw statement
//? sourceType: script
//?
//? test: S12.13_A1
//? description: Trying to throw exception with "throw"
//? expect: null
//? source: ...
#!/usr/bin/env afw


loc inCatch = false;

try {
  throw "expected_message";
} catch (err) {
  assert(err.message === "expected_message");
  inCatch = true;
}

assert(inCatch === true);


//? test: S12.13_A2_T4
//? description: Throwing string
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  throw "exception #1";
}
catch(e){
  if (e.message!=="exception #1") throw '#1: Exception ==="exception #1". Actual:  Exception ==='+ e.message ;
}

// CHECK#2
loc b="exception #1";
try{
  throw b;
}
catch(e){
  if (e.message!=="exception #1") throw '#2: Exception ==="exception #1". Actual:  Exception ==='+ e.message ;
}



//? test: S12.13_A3_T2
//? description: Evaluating string expression
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
try{
  throw "exception"+" #1";
}
catch(e){
  if (e.message!=="exception #1" )
    throw '#1: Exception === "exception #1"(operation +. Actual:  Exception ==='+ e.message ;
}

// CHECK#2
loc b="exception";
loc a=" #1";
try{
  throw b+a;
}
catch(e){
  if (e.message!=="exception #1")
     throw '#2: Exception === "exception #1"(operation +. Actual:  Exception ==='+ e.message ;
}



//? test: S12.13_A3_T5
//? description: Evaluating equation expression
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc a=true;
loc b=false;
try{
  throw ((a&&(!b))?"exception":" #1");
}
catch(e){
  if (e.message!=="exception")
  throw '#1: Exception ==="exception"(operation ? , . Actual:  Exception ==='+e.message  ;
}


//? test: S12.13_A3_T6
//? description: Evaluating functions
//? expect: null
//? source: ...
#!/usr/bin/env afw


// CHECK#1
loc i=0;
function adding1(){
  i = i + 1;
  return string(1);
}
try{
  throw (adding1());
}
catch(e){
  if (e.message !== "1")
    throw '#1: Exception === "1". Actual:  Exception ==='+ e.message;
}

// CHECK#2
loc i=0;
function adding2(){
  i = i + 1;
  return string(i);
}
try{
  throw adding2();
}
catch(e){}
if (i!==1)
    throw '#2: i===1. Actual: i==='+ i;

// CHECK#3
loc i=0;
function adding3(){
  i = i + 1;
  return "x";
}
try{
  throw adding3();
}
catch(e){}
if (i!==1) 
    throw '#3: i===1. Actual: i==='+i;

// CHECK#4
function adding4(i){
  i = i + 1;
  return string(i);
}
try{
  throw (adding4(1));
}
catch(e){
  if (e.message!=="2")
     throw '#4: Exception ===2. Actual:  Exception ==='+ e.message;
}
