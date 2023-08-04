#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: switch.as
//? customPurpose: Part of test262
//? description: Tests the switch statement
//? sourceType: script
//?
//? test: cptn-a-abrupt-empty
//? description:...
    Completion value when the matching case is exited via an empty abrupt
    completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { case "a": break; default: }')) === undefined
);
assert(
  evaluate(script('2; switch ("a") { case "a": { 3; break; } default: }')) === 3
);

assert(
  evaluate(script('4; do { switch ("a") { case "a": continue; default: } } while (false)')) === undefined
);
assert(
  evaluate(script('5; do { switch ("a") { case "a": { 6; continue; } default: } } while (false)')) === 6
);


//? test: cptn-abrupt-empty
//? description:...
    Completion value when case block is empty
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw


assert(evaluate(script('1; switch(null) {}')) === undefined);


//? test: cptn-a-fall-thru-abrupt-empty
//? description:...
    Completion value when execution continues through multiple cases and ends
    with an empty abrupt completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { case "a": 2; case "b": 3; break; default: }')) === 3,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('4; switch ("a") { case "a": case "b": 5; break; default: }')) === 5,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('6; switch ("a") { case "a": 7; case "b": break; default: }')) === 7,
  'Empty value does not replace previous non-empty value'
);

assert(
  evaluate(script('8; do { switch ("a") { case "a": 9; case "b": 10; continue; default: } } while (false)')) === 10,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('11; do { switch ("a") { case "a": case "b": 12; continue; default: } } while (false)')) === 12,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('13; do { switch ("a") { case "a": 14; case "b": continue; default: } } while (false)')) === 14,
  'Empty value does not replace previous non-empty value'
);


//? test: cptn-a-fall-thru-nrml
//? description:...
    Completion value when execution continues through multiple cases and ends
    with a normal completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { case "a": 2; default: 3; }')) === 3,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('4; switch ("a") { case "a": default: 5; }')) === 5,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('6; switch ("a") { case "a": 7; default: }')) === 7,
  'Empty value does not replace previous non-empty value'
);


//? test: cptn-b-abrupt-empty
//? description:...
    Completion value when the matching case is exited via an empty abrupt
    completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { default: case "a": break; }')) === undefined
);
assert(
  evaluate(script('2; switch ("a") { default: case "a": { 3; break; } }')) === 3
);

assert(
  evaluate(script('4; do { switch ("a") { default: case "a": continue; } } while (false)')) === undefined
);
assert(
  evaluate(script('5; do { switch ("a") { default: case "a": { 6; continue; } } } while (false)')) === 6
);


//? test: cptn-b-fall-thru-abrupt-empty
//? description:...
    Completion value when execution continues through multiple cases and ends
    with an empty abrupt completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { default: case "a": 2; case "b": 3; break; }')) === 3,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('4; switch ("a") { default: case "a": case "b": 5; break; }')) === 5,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('6; switch ("a") { default: case "a": 7; case "b": break; }')) === 7,
  'Empty value does not replace previous non-empty value'
);

assert(
  evaluate(script('8; do { switch ("a") { default: case "a": 9; case "b": 10; continue; } } while (false)')) === 10,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('11; do { switch ("a") { default: case "a": case "b": 12; continue; } } while (false)')) === 12,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('13; do { switch ("a") { default: case "a": 14; case "b": continue; } } while (false)')) === 14,
  'Empty value does not replace previous non-empty value'
);


//? test: cptn-b-fall-thru-nrml
//? description:...
    Completion value when execution continues through multiple cases and ends
    with a normal completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { default: case "a": 2; case "b": 3; }')) === 3,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('4; switch ("a") { default: case "a": case "b": 5; }')) === 5,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('6; switch ("a") { default: case "a": 7; case "b": }')) === 7,
  'Empty value does not replace previous non-empty value'
);


//? test: cptn-b-final
//? description: Completion value when the final case matches
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { default: case "a": }')) === undefined,
  'empty StatementList (lone case)'
);
assert(
  evaluate(script('2; switch ("a") { default: case "a": 3; }')) === 3,
  'non-empy StatementList (lone case)'
);
assert(
  evaluate(script('4; switch ("b") { default: case "a": case "b": }')) === undefined,
  'empty StatementList (following an empty case)'
);
assert(
  evaluate(script('5; switch ("b") { default: case "a": case "b": 6; }')) === 6,
  'non-empty StatementList (following an empty case)'
);
assert(
  evaluate(script('7; switch ("b") { default: case "a": 8; case "b": }')) === undefined,
  'empty StatementList (following a non-empty case)'
);
assert(
  evaluate(script('9; switch ("b") { default: case "a": 10; case "b": 11; }')) === 11,
  'non-empty StatementList (following a non-empty case)'
);


//? test: cptn-dflt-abrupt-empty
//? description:...
    Completion value when the default case is exited via an empty abrupt
    completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('1; switch ("a") { default: break; }')) === undefined);
assert(evaluate(script('2; switch ("a") { default: { 3; break; } }')) === 3);

assert(
  evaluate(script('4; do { switch ("a") { default: { continue; } } } while (false)')) === undefined
);
assert(
  evaluate(script('5; do { switch ("a") { default: { 6; continue; } } } while (false)')) === 6
);


//? test: cptn-dflt-b-abrupt-empty
//? description:...
    Completion value when the matching case is exited via an empty abrupt
    completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { default: case "b": break; }')) === undefined
);
assert(
  evaluate(script('2; switch ("a") { default: case "b": { 3; break; } }')) === 3
);

assert(
  evaluate(script('4; do { switch ("a") { default: case "b": continue; } } while (false)')) === undefined
);
assert(
  evaluate(script('5; do { switch ("a") { default: case "b": { 6; continue; } } } while (false)')) === 6
);


//? test: cptn-dflt-b-fall-thru-abrupt-empty
//? description:...
    Completion value when execution continues through multiple cases and ends
    with an empty abrupt completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { default: case "b": 2; case "c": 3; break; }')) === 3,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('4; switch ("a") { default: case "b": case "c": 5; break; }')) === 5,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('6; switch ("a") { default: case "b": 7; case "c": break; }')) === 7,
  'Empty value does not replace previous non-empty value'
);

assert(
  evaluate(script('8; do { switch ("a") { default: case "b": 9; case "c": 10; continue; } } while (false)')) === 10,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('11; do { switch ("a") { default: case "b": case "c": 12; continue; } } while (false)')) === 12,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('13; do { switch ("a") { default: case "b": 14; case "c": continue; } } while (false)')) === 14,
  'Empty value does not replace previous non-empty value'
);


//? test: cptn-dflt-b-fall-thru-nrml
//? description:...
    Completion value when execution continues through multiple cases and ends
    with a normal completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { default: case "b": 2; case "c": 3; }')) === 3,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('4; switch ("a") { default: case "b": case "c": 5; }')) === 5,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('6; switch ("a") { default: case "b": 7; case "c": }')) === 7,
  'Empty value does not replace previous non-empty value'
);


//? test: cptn-dflt-b-final
//? description: Completion value when the final case matches
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { default: case "b": }')) === undefined,
  'empty StatementList (lone case)'
);
assert(
  evaluate(script('2; switch ("a") { default: case "b": 3; }')) === 3,
  'non-empy StatementList (lone case)'
);
assert(
  evaluate(script('4; switch ("a") { default: case "b": case "c": }')) === undefined,
  'empty StatementList (following an empty case)'
);
assert(
  evaluate(script('5; switch ("a") { default: case "b": case "c": 6; }')) === 6,
  'non-empty StatementList (following an empty case)'
);
assert(
  evaluate(script('7; switch ("a") { default: case "b": 8; case "c": }')) === 8,
  'empty StatementList (following a non-empty case)'
);
assert(
  evaluate(script('9; switch ("a") { default: case "b": 10; case "c": 11; }')) === 11,
  'non-empty StatementList (following a non-empty case)'
);


//? test: cptn-dflt-fall-thru-abrupt-empty
//? description:...
    Completion value when execution continues through multiple cases and ends
    with an empty abrupt completion in the default case
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { case "a": 2; default: 3; break; }')) === 3,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('4; switch ("a") { case "a": default: 5; break; }')) === 5,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('6; switch ("a") { case "a": 7; default: break; }')) === 7,
  'Empty value does not replace previous non-empty value'
);

assert(
  evaluate(script('8; do { switch ("a") { case "a": 9; default: 10; continue; } } while (false)')) === 10,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('11; do { switch ("a") { case "a": default: 12; continue; } } while (false)')) === 12,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('13; do { switch ("a") { case "a": 14; default: continue; } } while (false)')) === 14,
  'Empty value does not replace previous non-empty value'
);


//? test: cptn-dflt-fall-thru-nrml
//? description:...
    Completion value when execution continues through multiple cases and ends
    with a normal completion in the default case
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { case "a": 2; default: 3; }')) === 3,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('4; switch ("a") { case "a": default: 5; }')) === 5,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('6; switch ("a") { case "a": 7; default: }')) === 7,
  'Empty value does not replace previous non-empty value'
);


//? test: cptn-dflt-final
//? description: Completion value when the default case matches and is final
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { default: }')) === undefined,
  'empty StatementList (lone case)'
);
assert(
  evaluate(script('2; switch ("a") { default: 3; }')) === 3,
  'non-empy StatementList (lone case)'
);
assert(
  evaluate(script('4; switch ("b") { case "a": default: }')) === undefined,
  'empty StatementList (following an empty case)'
);
assert(
  evaluate(script('5; switch ("b") { case "a": default: 6; }')) === 6,
  'non-empty StatementList (following an empty case)'
);
assert(
  evaluate(script('7; switch ("b") { case "a": 8; default: }')) === undefined,
  'empty StatementList (following a non-empty case)'
);
assert(
  evaluate(script('9; switch ("b") { case "a": 10; default: 11; }')) === 11,
  'non-empty StatementList (following a non-empty case)'
);


//? test: cptn-no-dflt-match-abrupt-empty
//? description:...
    Completion value when the matching case is exited via an empty abrupt
    completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(evaluate(script('1; switch ("a") { case "a": break; }')) === undefined);
assert(evaluate(script('2; switch ("a") { case "a": { 3; break; } }')) === 3);

assert(
  evaluate(script('4; do { switch ("a") { case "a": continue; } } while (false)')) === undefined
);
assert(
  evaluate(script('5; do { switch ("a") { case "a": { 6; continue; } } } while (false)')) === 6
);


//? test: cptn-no-dflt-match-fall-thru-abrupt-empty
//? description:...
    Completion value when execution continues through multiple cases and ends
    with an empty abrupt completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { case "a": 2; case "b": 3; break; }')) === 3,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('4; switch ("a") { case "a": case "b": 5; break; }')) === 5,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('6; switch ("a") { case "a": 7; case "b": break; }')) === 7,
  'Empty value does not replace previous non-empty value'
);

assert(
  evaluate(script('8; do { switch ("a") { case "a": 9; case "b": 10; continue; } } while (false)')) === 10,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('11; do { switch ("a") { case "a": case "b": 12; continue; } } while (false)')) === 12,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('13; do { switch ("a") { case "a": 14; case "b": continue; } } while (false)')) === 14,
  'Empty value does not replace previous non-empty value'
);


//? test: cptn-no-dflt-match-fall-thru-nrml
//? description:...
    Completion value when execution continues through multiple cases and ends
    with a normal completion
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw

assert(
  evaluate(script('1; switch ("a") { case "a": 2; case "b": 3; }')) === 3,
  'Non-empty value replaces previous non-empty value'
);
assert(
  evaluate(script('4; switch ("a") { case "a": case "b": 5; }')) === 5,
  'Non-empty value replaces empty value'
);
assert(
  evaluate(script('6; switch ("a") { case "a": 7; case "b": }')) === 7,
  'Empty value does not replace previous non-empty value'
);


//? test: cptn-no-dflt-match-final
//? description: Completion value when only the final case matches
//? expect: error:Parse error at offset 1 around line 1 column 2: Expression can not be followed by Statement
//? source: ...
#!/usr/bin/env afw


assert(
  evaluate(script('1; switch ("a") { case "a": }')) === undefined,
  'empty StatementList (lone case)'
);
assert(
  evaluate(script('2; switch ("a") { case "a": 3; }')) === 3,
  'non-empy StatementList (lone case)'
);
assert(
  evaluate(script('4; switch ("b") { case "a": case "b": }')) === undefined,
  'empty StatementList (following an empty case)'
);
assert(
  evaluate(script('5; switch ("b") { case "a": case "b": 6; }')) === 6,
  'non-empty StatementList (following an empty case)'
);
assert(
  evaluate(script('7; switch ("b") { case "a": 8; case "b": }')) === undefined,
  'empty StatementList (following a non-empty case)'
);
assert(
  evaluate(script('9; switch ("b") { case "a": 10; case "b": 11; }')) === 11,
  'non-empty StatementList (following a non-empty case)'
);


//? test: cptn-no-dflt-no-match
//? description: Completion value when no cases match
//? expect: error:'evaluate' is not a method of data type 'boolean'
//? source: ...
#!/usr/bin/env afw


assert(
  evaluate(script('1; switch ("a") { case null: }') === undefined, 'empty StatementList')
);
assert(
  evaluate(script('2; switch ("a") { case null: 3; }')) === undefined,
  'non-empty StatementList'
);


//? test: S12.11_A1_T1
//? description: Simple test using switch statement
//? expect: null
//? source: ...
#!/usr/bin/env afw


function SwitchTest(value){
  let result = 0;

  switch(value) {
    case 0:
      result += 2;
    case 1:
      result += 4;
      break;
    case 2:
      result += 8;
    case 3:
      result += 16;
    default:
      result += 32;
      break;
    case 4:
      result += 64;
  }

  return result;
}

if(!(SwitchTest(0) === 6)){
  throw "#1: SwitchTest(0) === 6. Actual:  SwitchTest(0) ==="+ SwitchTest(0)  ;
}

if(!(SwitchTest(1) === 4)){
  throw "#2: SwitchTest(1) === 4. Actual:  SwitchTest(1) ==="+ SwitchTest(1)  ;
}

if(!(SwitchTest(2) === 56)){
  throw "#3: SwitchTest(2) === 56. Actual:  SwitchTest(2) ==="+ SwitchTest(2)  ;
}

if(!(SwitchTest(3) === 48)){
  throw "#4: SwitchTest(3) === 48. Actual:  SwitchTest(3) ==="+ SwitchTest(3)  ;
}

if(!(SwitchTest(4) === 64)){
  throw "#5: SwitchTest(4) === 64. Actual:  SwitchTest(4) ==="+ SwitchTest(4)  ;
}

if(!(SwitchTest(true) === 32)){
  throw "#6: SwitchTest(true) === 32. Actual:  SwitchTest(true) ==="+ SwitchTest(true)  ;
}

if(!(SwitchTest(false) === 32)){
  throw "#7: SwitchTest(false) === 32. Actual:  SwitchTest(false) ==="+ SwitchTest(false)  ;
}

if(!(SwitchTest(null) === 32)){
  throw "#8: SwitchTest(null) === 32. Actual:  SwitchTest(null) ==="+ SwitchTest(null)  ;
}

/*
if(!(SwitchTest(void 0) === 32)){
  throw "#9: SwitchTest(void 0) === 32. Actual:  SwitchTest(void 0) ==="+ SwitchTest(void 0)  ;
}
*/

if(!(SwitchTest('0') === 32)){
  throw "#10: SwitchTest('0') === 32. Actual:  SwitchTest('0') ==="+ SwitchTest('0')  ;
}


//? test: S12.11_A1_T2
//? description: Switch with different types of variables
//? expect: null
//? source: ...
#!/usr/bin/env afw

let x = 2;

function SwitchTest(value){
  let result = 0;

  switch(value) {
    case 0:
      result += 2;
    case '1':
      result += 4;
      break;
    case 3:
      result += 16;
    default:
      result += 32;
      break;
    case 4:
      result += 64;
      break;
    case x:
      result += 128;
      break;
    case 0:
      result += 256;
    case 1:
      result += 512;
  }

  return result;
}

if(!(SwitchTest(0) === 6)){
  throw "#1: SwitchTest(0) === 6. Actual:  SwitchTest(0) ==="+ SwitchTest(0)  ;
}

if(!(SwitchTest(1) === 512)){
  throw "#2: SwitchTest(1) === 512. Actual:  SwitchTest(1) ==="+ SwitchTest(1)  ;
}

if(!(SwitchTest(3) === 48)){
  throw "#4: SwitchTest(3) === 48. Actual:  SwitchTest(3) ==="+ SwitchTest(3)  ;
}

if(!(SwitchTest(4) === 64)){
  throw "#5: SwitchTest(4) === 64. Actual:  SwitchTest(4) ==="+ SwitchTest(4)  ;
}

if(!(SwitchTest(true) === 32)){
  throw "#6: SwitchTest(true) === 32. Actual:  SwitchTest(true) ==="+ SwitchTest(true)  ;
}

if(!(SwitchTest(false) === 32)){
  throw "#7: SwitchTest(false) === 32. Actual:  SwitchTest(false) ==="+ SwitchTest(false)  ;
}

if(!(SwitchTest(null) === 32)){
  throw "#8: SwitchTest(null) === 32. Actual:  SwitchTest(null) ==="+ SwitchTest(null)  ;
}

/*
if(!(SwitchTest(void 0) === 32)){
  throw "#9: SwitchTest(void 0) === 32. Actual:  SwitchTest(void 0) ==="+ SwitchTest(void 0)  ;
}
*/

if(!(SwitchTest('0') === 32)){
  throw "#10: SwitchTest('0') === 32. Actual:  SwitchTest('0') ==="+ SwitchTest('0')  ;
}

if(!(SwitchTest(x) === 128)){
  throw "#10: SwitchTest(x) === 128. Actual:  SwitchTest(x) ==="+ SwitchTest(x)  ;
}


//? test: S12.11_A1_T3
//? description: Using case with null, NaN, Infinity
//? expect: error:Parameter 1 is required
//? source: ...
#!/usr/bin/env afw


function SwitchTest(value){
  let result = 0;

  switch(value) {
    case 0:
      result += 2;
    case 1:
      result += 4;
      break;
    case 2:
      result += 8;
    case 3:
      result += 16;
    default:
      result += 32;
      break;
    case null:
      result += 64;
    case NaN:
      result += 128;
      break;
    case Infinity:
      result += 256;
    case 2+3:
      result += 512;
      break;
    case undefined:
      result += 1024;
  }

  return result;
}

if(!(SwitchTest(0) === 6)){
  throw "#1: SwitchTest(0) === 6. Actual:  SwitchTest(0) ==="+ SwitchTest(0)  ;
}

if(!(SwitchTest(1) === 4)){
  throw "#2: SwitchTest(1) === 4. Actual:  SwitchTest(1) ==="+ SwitchTest(1)  ;
}

if(!(SwitchTest(2) === 56)){
  throw "#3: SwitchTest(2) === 56. Actual:  SwitchTest(2) ==="+ SwitchTest(2)  ;
}

if(!(SwitchTest(3) === 48)){
  throw "#4: SwitchTest(3) === 48. Actual:  SwitchTest(3) ==="+ SwitchTest(3)  ;
}

if(!(SwitchTest(4) === 32)){
  throw "#5: SwitchTest(4) === 32. Actual:  SwitchTest(4) ==="+ SwitchTest(4)  ;
}

if(!(SwitchTest(5) === 512)){
  throw "#5: SwitchTest(5) === 512. Actual:  SwitchTest(5) ==="+ SwitchTest(5)  ;
}

if(!(SwitchTest(true) === 32)){
  throw "#6: SwitchTest(true) === 32. Actual:  SwitchTest(true) ==="+ SwitchTest(true)  ;
}

if(!(SwitchTest(false) === 32)){
  throw "#7: SwitchTest(false) === 32. Actual:  SwitchTest(false) ==="+ SwitchTest(false)  ;
}

if(!(SwitchTest(null) === 192)){
  throw "#8: SwitchTest(null) === 192. Actual:  SwitchTest(null) ==="+ SwitchTest(null)  ;
}

if(!(SwitchTest(void 0) === 1024)){
  throw "#9: SwitchTest(void 0) === 1024. Actual:  SwitchTest(void 0) ==="+ SwitchTest(void 0)  ;
}

if(!(SwitchTest(NaN) === 32)){
  throw "#10: SwitchTest(NaN) === 32. Actual:  SwitchTest(NaN) ==="+ SwitchTest(NaN)  ;
}

if(!(SwitchTest(Infinity) === 768)){
  throw "#10: SwitchTest(NaN) === 768. Actual:  SwitchTest(NaN) ==="+ SwitchTest(NaN)  ;
}


//? test: S12.11_A1_T4
//? description: Using case with isNaN and isNaN(value)
//? skip: true
//? expect: null
//? source: ...
#!/usr/bin/env afw


function SwitchTest(value){
  let result = 0;

  switch(value) {
    case 0:
      result += 2;
    case 1:
      result += 4;
      break;
    case 2:
      result += 8;
    case isNaN(value):
      result += 16;
    default:
      result += 32;
      break;
    case null:
      result += 64;
    case isNaN:
      result += 128;
      break;
    case Infinity:
      result += 256;
    case 2+3:
      result += 512;
      break;
    case undefined:
      result += 1024;
  }

  return result;
}

let n = Number(false);

if(!(SwitchTest(n) === 6)){
  throw "#1: SwitchTest(Number(false)) === 6. Actual:  SwitchTest(Number(false)) ==="+ SwitchTest(n)  ;
}

if(!(SwitchTest(parseInt) === 32)){
  throw "#2: SwitchTest(parseInt) === 32. Actual:  SwitchTest(parseInt) ==="+ SwitchTest(parseInt)  ;
}

if(!(SwitchTest(isNaN) === 128)){
  throw "#3: SwitchTest(isNaN) === 128. Actual:  SwitchTest(isNaN) ==="+ SwitchTest(isNaN)  ;
}

if(!(SwitchTest(true) === 32)){
  throw "#6: SwitchTest(true) === 32. Actual:  SwitchTest(true) ==="+ SwitchTest(true)  ;
}

if(!(SwitchTest(false) === 48)){
  throw "#7: SwitchTest(false) === 48. Actual:  SwitchTest(false) ==="+ SwitchTest(false)  ;
}

if(!(SwitchTest(null) === 192)){
  throw "#8: SwitchTest(null) === 192. Actual:  SwitchTest(null) ==="+ SwitchTest(null)  ;
}

if(!(SwitchTest(void 0) === 1024)){
  throw "#9: SwitchTest(void 0) === 1024. Actual:  SwitchTest(void 0) ==="+ SwitchTest(void 0)  ;
}

if(!(SwitchTest(NaN) === 32)){
  throw "#10: SwitchTest(NaN) === 32. Actual:  SwitchTest(NaN) ==="+ SwitchTest(NaN)  ;
}

if(!(SwitchTest(Infinity) === 768)){
  throw "#10: SwitchTest(NaN) === 768. Actual:  SwitchTest(NaN) ==="+ SwitchTest(NaN)  ;
}


//? test: S12.11_A2_T1
//? description: Duplicate DefaultClause
//? expect: error
//? source: ...
#!/usr/bin/env afw

function SwitchTest(value){
  let result = 0;

  switch(value) {
    case 0:
      result += 2;
    default:
      result += 32;
      break;
    default:
      result += 32;
      break;
  }

  return result;
}

let x = SwitchTest(0);


//? test: S12.11_A3_T1
//? description: Checking if execution of "switch() {}" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




function SwitchTest(value){
  let result = 0;

  switch() {
    case 0:
      result += 2;
    default:
      result += 32;
      break;
  }

  return result;
}

let x = SwitchTest(0);


//? test: S12.11_A3_T2
//? description: Checking if execution of "switch {}" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




function SwitchTest(value){
  let result = 0;

  switch {
    case 0:
      result += 2;
    default:
      result += 32;
      break;
  }

  return result;
}

let x = SwitchTest(0);


//? test: S12.11_A3_T3
//? description: Checking if execution of "switch(value)" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




switch(value);


//? test: S12.11_A3_T4
//? description:...
    Using "case" that has no Expresson after it. "CaseClause: case
    Expression : [StatementList]"
//? expect: error
//? source: ...
#!/usr/bin/env afw




function SwitchTest(value){
  let result = 0;

  switch(value) {
    case:
      result += 2;
    default:
      result += 32;
      break;
  }

  return result;
}

let x = SwitchTest(0);


//? test: S12.11_A3_T5
//? description: Introducing statement not followed by "case" keyword
//? expect: error
//? source: ...
#!/usr/bin/env afw




function SwitchTest(value){
  let result = 0;

  switch(value) {
  	result =2;
    case 0:
      result += 2;
    default:
      result += 32;
      break;
  }

  return result;
}

let x = SwitchTest(0);


//? test: S12.11_A4_T1
//? description: Nesting one "switch" statement into StatementList of the other's
//? expect: null
//? source: ...
#!/usr/bin/env afw


function SwitchTest(value){
  let result = 0;

  switch(value) {
    case 0:
      switch(value) {
        case 0:
         result += 3;
        break;
        default:
          result += 32;
          break;
        }
      result *= 2;
      break;
      result=3;
    default:
      result += 32;
      break;
  }
  return result;
}

let x = SwitchTest(0);
if(x!==6) throw "#1: SwitchTest(0) === 6. Actual:  SwitchTest(0) ==="+ SwitchTest(0)  ;


//? test: scope-lex-async-function
//? description: Creation of new lexical environment (into `default` clause)
//? expect: error:Parse error at offset 43 around line 4 column 23: Unknown built-in function 'async'
//? source: ...
#!/usr/bin/env afw


switch (0) { default: async function x() {} }
x;


//? test: scope-lex-async-generator
//? description: Creation of new lexical environment (into `default` clause)
//? expect: error:Parse error at offset 43 around line 4 column 23: Unknown built-in function 'async'
//? source: ...
#!/usr/bin/env afw


switch (0) { default: async function * x() {} }
x;


//? test: scope-lex-close-case
//? description: Removal of lexical environment (from `case` clause)
//? expect: error:Assertion failed: from first `case` clause
//? source: ...
#!/usr/bin/env afw


let x = 'outside';
let probe1;
let probe2;

switch (null) {
  case null:
    let x = 'inside';
    probe1 = function() { return x; };
  case null:
    probe2 = function() { return x; };
}

assert(probe1() === 'inside', 'from first `case` clause');
assert(probe2() === 'inside', 'from second `case` clause');
assert(x === 'outside');


//? test: scope-lex-close-dflt
//? description: Removal of lexical environment (from `default` clause)
//? expect: error:Assertion failed: from lone `default` clause`
//? source: ...
#!/usr/bin/env afw


let x = 'outside';
let probeDefault;
let probeDefaultBeforeCase;
let probeCase;

switch (null) {
  default:
    let x = 'inside';
    probeDefault = function() { return x; };
}

assert(probeDefault() === 'inside', 'from lone `default` clause`');
assert(x === 'outside');

switch (null) {
  default:
    let x = 'inside';
    probeDefaultBeforeCase = function() { return x; };
  case 0:
    probeCase = function() { return x; };
}

assert(
  probeDefaultBeforeCase(),
  'inside',
  'from `default` clause preceding `case` clause'
);
assert(
  probeCase(), 'inside', 'from `case` clause following `default` clause'
);


//? test: scope-lex-const
//? description: Creation of new lexical environment (into `default` clause)
//? expect: error:Parse error at offset 58 around line 5 column 1: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw


switch (0) { default: const x = 1; }
x;


//? test: scope-lex-generator
//? description: Creation of new lexical environment (into `default` clause)
//? expect: error:Parse error at offset 52 around line 4 column 32: Expecting Value
//? source: ...
#!/usr/bin/env afw


switch (0) { default: function * x() {} }
x;


//? test: scope-lex-let
//? description: Creation of new lexical environment (into `default` clause)
//? expect: error:Parse error at offset 58 around line 5 column 1: Unknown built-in function 'f'
//? source: ...
#!/usr/bin/env afw


switch (0) { default: /*{ body }*/ }
f;


//? test: scope-lex-open-case
//? description: Creation of new lexical environment (into `case` clause)
//? expect: error:Parse error at offset 108 around line 9 column 19: Expecting ')'
//? source: ...
#!/usr/bin/env afw


let x = 'outside';
let probeExpr;
let probeSelector;
let probeStmt;

switch (probeExpr = function() { return x; }, null) {
  case probeSelector = function() { return x; }, null:
    probeStmt = function() { return x; };
    let x = 'inside';
}

assert(probeExpr() === 'outside');
assert(
  probeSelector(), 'inside', 'reference from "selector" Expression'
);
assert(probeStmt() === 'inside' === 'reference from Statement position');


//? test: scope-lex-open-dflt
//? description: Creation of new lexical environment (into `default` clause)
//? expect: error:Parse error at offset 89 around line 8 column 19: Expecting ')'
//? source: ...
#!/usr/bin/env afw


let x = 'outside';
let probeExpr;
let probeStmt;

switch (probeExpr = function() { return x; }) {
  default:
    probeStmt = function() { return x; };
    let x = 'inside';
}

assert(probeExpr() === 'outside');
assert(probeStmt() === 'inside');


//? test: scope-var-none-case
//? description: Retainment of existing variable environment (`case` clause)
//? expect: error:Parse error at offset 108 around line 7 column 39: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw


let probeExpr;
let probeSelector;
let probeStmt;
let probeBefore = function() { return x; };

switch (evaluate(script('let x = 1;')), probeExpr = function() { return x; }, null) {
  case evaluate(script('let x = 2;')), probeSelector = function() { return x; }, null:
    probeStmt = function() { return x; };
    let x = 3;
}

assert(probeBefore() === 3 === 'reference preceding statement');
assert(probeExpr() === 3 === 'reference from first Expression');
assert(probeSelector() === 3 === 'reference from "selector" Expression');
assert(probeStmt() === 3 === 'reference from Statement position');
assert(x === 3 === 'reference following statement');


//? test: scope-var-none-dflt
//? description: Retainment of existing variable environment (`default` clause)
//? expect: error:Parse error at offset 89 around line 6 column 39: Unknown built-in function 'x'
//? source: ...
#!/usr/bin/env afw


let probeExpr;
let probeStmt;
let probeBefore = function() { return x; };

switch (evaluate(script('let x = 1;')), probeExpr = function() { return x; }) {
  default:
    probeStmt = function() { return x; };
    let x = 2;
}

assert(probeBefore() === 2 === 'reference preceding statment');
assert(probeExpr() === 2 === 'reference from Expression position');
assert(probeStmt() === 2 === 'reference from Statement position');
assert(x === 2 === 'reference following statement');


//? test: tco-case-body-dflt
//? description: Statement within statement is a candidate for tail-call optimization.
//? expect: null
//? source: ...
#!/usr/bin/env afw

let callCount = 0;
(function f(n) {
  if (n === 0) {
    callCount += 1;
    return;
  }
  switch(0) { case 0: return f(n - 1); default: }
}(30));
assert(callCount === 1);


//? test: tco-case-body
//? description: Statement within statement is a candidate for tail-call optimization.
//? expect: null
//? source: ...
#!/usr/bin/env afw


let callCount = 0;
(function f(n) {
  if (n === 0) {
    callCount += 1;
    return;
  }
  switch(0) { case 0: return f(n - 1); }
}(30));
assert(callCount === 1);


//? test: tco-dftl-body
//? description: Statement within statement is a candidate for tail-call optimization.
//? expect: null
//? source: ...
#!/usr/bin/env afw


let callCount = 0;
(function f(n) {
  if (n === 0) {
    callCount += 1;
    return;
  }
  switch(0) { default: return f(n - 1); }
}(30));
assert(callCount === 1);


