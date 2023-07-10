#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: numeric.as
//? customPurpose: Part of test262
//? description: literals/numeric
//? sourceType: script
//?
//? test: 7.8.3-1gs
//? description: Strict Mode - octal extension(010) is forbidden in strict mode
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc y = 010;


//? test: 7.8.3-2gs
//? description:...
    Strict Mode - octal extension is forbidden in strict mode (after a
    hex number is assigned to a variable)
//? expect: error
//? source: ...
#!/usr/bin/env afw

loc a;
a = 0x1;
a = 01;


//? test: 7.8.3-3gs
//? description:...
    Strict Mode - octal extension is forbidden in strict mode (after a
    hex number is assigned to a variable from an eval)
//? expect: error
//? source: ...
#!/usr/bin/env afw

evaluate(script("loc a = 0x1;a = 01;"));


//? test: binary-invalid-digit
//? description: Binary-integer-literal-like sequence containing an invalid digit
//? expect: error
//? source: ...
#!/usr/bin/env afw


0b2;


//? test: binary-invalid-leading
//? description: Binary-integer-literal-like sequence with a leading 0
//? expect: error
//? source: ...
#!/usr/bin/env afw


00b0;


//? test: binary-invalid-truncated
//? description: Binary-integer-literal-like sequence without any digits
//? expect: error
//? source: ...
#!/usr/bin/env afw


0b;


//? test: binary-invalid-unicode
//? description: Binary-integer-literal-like sequence expressed with unicode escape sequence
//? expect: error
//? source: ...
#!/usr/bin/env afw


0\u00620;


//? test: binary
//? description: Mathematical value of valid binary integer literals
//? expect: null
//? source: ...
#!/usr/bin/env afw


assert(0b0 === 0, 'lower-case head');
assert(0B0 === 0, 'upper-case head');
assert(0b00 === 0, 'lower-case head with leading zeros');
assert(0B00 === 0, 'upper-case head with leading zeros');

assert(0b1 === 1, 'lower-case head');
assert(0B1 === 1, 'upper-case head');
assert(0b01 === 1, 'lower-case head with leading zeros');
assert(0B01 === 1, 'upper-case head with leading zeros');

assert(0b10 === 2, 'lower-case head');
assert(0B10 === 2, 'upper-case head');
assert(0b010 === 2, 'lower-case head with leading zeros');
assert(0B010 === 2, 'upper-case head with leading zeros');

assert(0b11 === 3, 'lower-case head');
assert(0B11 === 3, 'upper-case head');
assert(0b011 === 3, 'lower-case head with leading zeros');
assert(0B011 === 3, 'upper-case head with leading zeros');


//? test: legacy-octal-integer
//? description: Mathematical value for LegacyOctalIntegerLiteral
//? expect: error
//? source: ...
#!/usr/bin/env afw


// LegacyOctalIntegerLiteral ::
//   0 OctalDigit
assert(00 === 0, '00');
assert(01 === 1, '01');
assert(02 === 2, '02');
assert(03 === 3, '03');
assert(04 === 4, '04');
assert(05 === 5, '05');
assert(06 === 6, '06');
assert(07 === 7, '07');

// LegacyOctalIntegerLiteral ::
//   LegacyOctalIntegerLiteral OctalDigit
assert(000 === 0, '000');
assert(001 === 1, '001');
assert(002 === 2, '002');
assert(003 === 3, '003');
assert(004 === 4, '004');
assert(005 === 5, '005');
assert(006 === 6, '006');
assert(007 === 7, '007');

assert.sameValue(070, 56);
assert.sameValue(071, 57);
assert.sameValue(072, 58);
assert.sameValue(073, 59);
assert.sameValue(074, 60);
assert.sameValue(075, 61);
assert.sameValue(076, 62);
assert.sameValue(077, 63);


//? test: legacy-octal-integer-strict
//? description: LegacyOctalIntegerLiteral is not enabled in strict mode code
//? expect: error
//? source: ...
#!/usr/bin/env afw




00;


//? test: legacy-octal-integery-000-strict
//? description: Strict Mode - octal extension (000) is forbidden in strict mode
//? expect: error
//? source: ...
#!/usr/bin/env afw




000;


//? test: legacy-octal-integery-005-strict
//? description: Strict Mode - octal extension (005) is forbidden in strict mode
//? expect: error
//? source: ...
#!/usr/bin/env afw




005;


//? test: legacy-octal-integery-010-strict
//? description: Strict Mode - octal extension (010) is forbidden in strict mode
//? expect: error
//? source: ...
#!/usr/bin/env afw




010;


//? test: legacy-octal-integery-01-strict
//? description: Strict Mode - octal extension (01) is forbidden in strict mode
//? expect: error
//? source: ...
#!/usr/bin/env afw




01;


//? test: legacy-octal-integery-06-strict
//? description: Strict Mode - octal extension (06) is forbidden in strict mode
//? expect: error
//? source: ...
#!/usr/bin/env afw




06;


//? test: legacy-octal-integery-07-strict
//? description: Strict Mode - octal extension (07) is forbidden in strict mode
//? expect: error
//? source: ...
#!/usr/bin/env afw




07;


//? test: non-octal-decimal-integer
//? description: Mathematical value for NonOctalDecimalIntegerLiteral
//? expect: error
//? source: ...
#!/usr/bin/env afw

// these are LegacyOctalIntegerLiterals, not supported

// NonOctalDecimalIntegerLiteral ::
//   0 NonOctalDigit
assert(08 === 8, '08');
assert(09 === 9, '09');

// NonOctalDecimalIntegerLiteral ::
//   LegacyOctalLikeDecimalIntegerLiteral NonOctalDigit
assert(008 === 8, '008');
assert(018 === 18, '018');
assert(028 === 28, '028');
assert(038 === 38, '038');
assert(048 === 48, '048');
assert(058 === 58, '058');
assert(068 === 68, '068');
assert(078 === 78, '078');
assert(088 === 88, '088');
assert(098 === 98, '098');
assert(0708 === 708, '708');
assert(0718 === 718, '718');
assert(0728 === 728, '728');
assert(0738 === 738, '738');
assert(0748 === 748, '748');
assert(0758 === 758, '758');
assert(0768 === 768, '768');
assert(0778 === 778, '778');
assert(0788 === 788, '788');
assert(0798 === 798, '798');

assert(009 === 9, '009');
assert(019 === 19, '019');
assert(029 === 29, '029');
assert(039 === 39, '039');
assert(049 === 49, '049');
assert(059 === 59, '059');
assert(069 === 69, '069');
assert(079 === 79, '079');
assert(089 === 89, '089');
assert(099 === 99, '099');
assert(0709 === 709, '0709');
assert(0719 === 719, '0719');
assert(0729 === 729, '0729');
assert(0739 === 739, '0739');
assert(0749 === 749, '0749');
assert(0759 === 759, '0759');
assert(0769 === 769, '0769');
assert(0779 === 779, '0779');
assert(0789 === 789, '0789');
assert(0799 === 799, '0799');

// NonOctalDecimalIntegerLiteral ::
//   NonOctalDecimalIntegerLiteral DecimalDigit
assert(080 === 80, '080');
assert(081 === 81, '081');
assert(082 === 82, '082');
assert(083 === 83, '083');
assert(084 === 84, '084');
assert(085 === 85, '085');
assert(086 === 86, '086');
assert(087 === 87, '087');
assert(088 === 88, '088');
assert(089 === 89, '089');

assert(0780 === 780, '0780');
assert(0781 === 781, '0781');
assert(0782 === 782, '0782');
assert(0783 === 783, '0783');
assert(0784 === 784, '0784');
assert(0785 === 785, '0785');
assert(0786 === 786, '0786');
assert(0787 === 787, '0787');
assert(0788 === 788, '0788');
assert(0789 === 789, '0789');

assert(090 === 90, '090');
assert(091 === 91, '091');
assert(092 === 92, '092');
assert(093 === 93, '093');
assert(094 === 94, '094');
assert(095 === 95, '095');
assert(096 === 96, '096');
assert(097 === 97, '097');
assert(098 === 98, '098');
assert(099 === 99, '099');

assert(0790 === 790, '0790');
assert(0791 === 791, '0791');
assert(0792 === 792, '0792');
assert(0793 === 793, '0793');
assert(0794 === 794, '0794');
assert(0795 === 795, '0795');
assert(0796 === 796, '0796');
assert(0797 === 797, '0797');
assert(0798 === 798, '0798');
assert(0799 === 799, '0799');


//? test: non-octal-decimal-integer-strict
//? description: NonOctalDecimalIntegerLiteral is not enabled in strict mode code
//? expect: error
//? source: ...
#!/usr/bin/env afw




08;


//? test: numeric-followed-by-ident
//? description: numeric-followed-by-ident
//? expect: error
//? source: ...
#!/usr/bin/env afw




3in []


//? test: octal-invalid-digit
//? description: Octal-integer-literal-like sequence containing an invalid digit
//? expect: error
//? source: ...
#!/usr/bin/env afw




0o8;


//? test: octal-invalid-leading
//? description: Octal-integer-literal-like sequence with a leading 0
//? expect: error
//? source: ...
#!/usr/bin/env afw




00o0;


//? test: octal-invalid-truncated
//? description: Octal-integer-literal-like sequence without any digits
//? expect: error
//? source: ...
#!/usr/bin/env afw




0o;


//? test: octal-invalid-unicode
//? description: Octal-integer-literal-like sequence expressed with unicode escape sequence
//? expect: error
//? source: ...
#!/usr/bin/env afw




0\u006f0;


//? test: octal
//? description: Mathematical value of valid octal integer literals
//? expect: null
//? source: ...
#!/usr/bin/env afw


assert(0o0 === 0, 'lower-case head');
assert(0O0 === 0, 'upper-case head');
assert(0o00 === 0, 'lower-case head with leading zeros');
assert(0O00 === 0, 'upper-case head with leading zeros');

assert(0o1 === 1, 'lower-case head');
assert(0O1 === 1, 'upper-case head');
assert(0o01 === 1, 'lower-case head with leading zeros');
assert(0O01 === 1, 'upper-case head with leading zeros');

assert(0o7 === 7, 'lower-case head');
assert(0O7 === 7, 'upper-case head');
assert(0o07 === 7, 'lower-case head with leading zeros');
assert(0O07 === 7, 'upper-case head with leading zeros');

assert(0o10 === 8, 'lower-case head');
assert(0O10 === 8, 'upper-case head');
assert(0o010 === 8, 'lower-case head with leading zeros');
assert(0O010 === 8, 'upper-case head with leading zeros');

assert(0o11 === 9, 'lower-case head');
assert(0O11 === 9, 'upper-case head');
assert(0o011 === 9, 'lower-case head with leading zeros');
assert(0O011 === 9, 'upper-case head with leading zeros');

assert(0o77 === 63, 'lower-case head');
assert(0O77 === 63, 'upper-case head');
assert(0o077 === 63, 'lower-case head with leading zeros');
assert(0O077 === 63, 'upper-case head with leading zeros');


//? test: S7.8.3_A1.1_T1
//? description: "DecimalIntegerLiteral :: 0, NoNZeroDigit"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0 !== 0) {
  throw '#0: 0 === 0';
}

//CHECK#1
if (1 !== 1) {
  throw '#1: 1 === 1';
}

//CHECK#2
if (2 !== 2) {
  throw '#2: 2 === 2';
}

//CHECK#3
if (3 !== 3) {
  throw '#3: 3 === 3';
}

//CHECK#4
if (4 !== 4) {
  throw '#4: 4 === 4';
}

//CHECK#5
if (5 !== 5) {
  throw '#5: 5 === 5';
}

//CHECK#6
if (6 !== 6) {
  throw '#6: 6 === 6';
}

//CHECK#7
if (7 !== 7) {
  throw '#7: 7 === 7';
}

//CHECK#8
if (8 !== 8) {
  throw '#8: 8 === 8';
}

//CHECK#9
if (9 !== 9) {
  throw '#9: 9 === 9';
}


//? test: S7.8.3_A1.1_T2
//? description: "DecimalIntegerLiteral :: NoNZeroDigit DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (11 !== 11) {
  throw '#1: 11 === 11';
}

//CHECK#2
if (22 !== 22) {
  throw '#2: 22 === 22';
}

//CHECK#3
if (33 !== 33) {
  throw '#3: 33 === 33';
}

//CHECK#4
if (44 !== 44) {
  throw '#4: 44 === 44';
}

//CHECK#5
if (55 !== 55) {
  throw '#5: 55 === 55';
}

//CHECK#6
if (66 !== 66) {
  throw '#6: 66 === 66';
}

//CHECK#7
if (77 !== 77) {
  throw '#7: 77 === 77';
}

//CHECK#8
if (88 !== 88) {
  throw '#8: 88 === 88';
}

//CHECK#9
if (99 !== 99) {
  throw '#9: 99 === 99';
}


//? test: S7.8.3_A1.2_T1
//? description: "ExponentPart :: e DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0e1 !== 0.0) {
  throw '#0: 0e1 === 0';
}

//CHECK#1
if (1e1 !== 10.0) {
  throw '#1: 1e1 === 10';
}

//CHECK#2
if (2e1 !== 20.0) {
  throw '#2: 2e1 === 20';
}

//CHECK#3
if (3e1 !== 30.0) {
  throw '#3: 3e1 === 30';
}

//CHECK#4
if (4e1 !== 40.0) {
  throw '#4: 4e1 === 40';
}

//CHECK#5
if (5e1 !== 50.0) {
  throw '#5: 5e1 === 50';
}

//CHECK#6
if (6e1 !== 60.0) {
  throw '#6: 6e1 === 60';
}

//CHECK#7
if (7e1 !== 70.0) {
  throw '#7: 7e1 === 70';
}

//CHECK#8
if (8e1 !== 80.0) {
  throw '#8: 8e1 === 80';
}

//CHECK#9
if (9e1 !== 90.0) {
  throw '#9: 9e1 === 90';
}


//? test: S7.8.3_A1.2_T2
//? description: "ExponentPart :: E DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0E1 !== 0.0) {
  throw '#0: 0E1 === 0';
}

//CHECK#1
if (1E1 !== 10.0) {
  throw '#1: 1E1 === 1';
}

//CHECK#2
if (2E1 !== 20.0) {
  throw '#2: 2E1 === 20';
}

//CHECK#3
if (3E1 !== 30.0) {
  throw '#3: 3E1 === 30';
}

//CHECK#4
if (4E1 !== 40.0) {
  throw '#4: 4E1 === 40';
}

//CHECK#5
if (5E1 !== 50.0) {
  throw '#5: 5E1 === 50';
}

//CHECK#6
if (6E1 !== 60.0) {
  throw '#6: 6E1 === 60';
}

//CHECK#7
if (7E1 !== 70.0) {
  throw '#7: 7E1 === 70';
}

//CHECK#8
if (8E1 !== 80.0) {
  throw '#8: 8E1 === 80';
}

//CHECK#9
if (9E1 !== 90.0) {
  throw '#9: 9E1 === 90';
}


//? test: S7.8.3_A1.2_T3
//? description: "ExponentPart :: e -DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0e-1 !== 0.0) {
  throw '#0: 0e-1 === 0';
}

//CHECK#1
if (1e-1 !== 0.1) {
  throw '#1: 1e-1 === 0.1';
}

//CHECK#2
if (2e-1 !== 0.2) {
  throw '#2: 2e-1 === 0.2';
}

//CHECK#3
if (3e-1 !== 0.3) {
  throw '#3: 3e-1 === 0.3';
}

//CHECK#4
if (4e-1 !== 0.4) {
  throw '#4: 4e-1 === 0.4';
}

//CHECK#5
if (5e-1 !== 0.5) {
  throw '#5: 5e-1 === 0.5';
}

//CHECK#6
if (6e-1 !== 0.6) {
  throw '#6: 6e-1 === 0.6';
}

//CHECK#7
if (7e-1 !== 0.7) {
  throw '#7: 7e-1 === 0.7';
}

//CHECK#8
if (8e-1 !== 0.8) {
  throw '#8: 8e-1 === 0.8';
}

//CHECK#9
if (9e-1 !== 0.9) {
  throw '#9: 9e-1 === 0.9';
}


//? test: S7.8.3_A1.2_T4
//? description: "ExponentPart :: E -DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0E-1 !== 0.0) {
  throw '#0: 0E-1 === 0';
}

//CHECK#1
if (1E-1 !== 0.1) {
  throw '#1: 1E-1 === 0.1';
}

//CHECK#2
if (2E-1 !== 0.2) {
  throw '#2: 2E-1 === 0.2';
}

//CHECK#3
if (3E-1 !== 0.3) {
  throw '#3: 3E-1 === 0.3';
}

//CHECK#4
if (4E-1 !== 0.4) {
  throw '#4: 4E-1 === 0.4';
}

//CHECK#5
if (5E-1 !== 0.5) {
  throw '#5: 5E-1 === 0.5';
}

//CHECK#6
if (6E-1 !== 0.6) {
  throw '#6: 6E-1 === 0.6';
}

//CHECK#7
if (7E-1 !== 0.7) {
  throw '#7: 7E-1 === 0.7';
}

//CHECK#8
if (8E-1 !== 0.8) {
  throw '#8: 8E-1 === 0.8';
}

//CHECK#9
if (9E-1 !== 0.9) {
  throw '#9: 9E-1 === 0.9';
}


//? test: S7.8.3_A1.2_T5
//? description: "ExponentPart :: e +DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0e+1 !== 0.0) {
  throw '#0: 0e+1 === 0';
}

//CHe+CK#1
if (1e+1 !== 10.0) {
  throw '#1: 1e+1 === 10';
}

//CHe+CK#2
if (2e+1 !== 20.0) {
  throw '#2: 2e+1 === 20';
}

//CHe+CK#3
if (3e+1 !== 30.0) {
  throw '#3: 3e+1 === 30';
}

//CHe+CK#4
if (4e+1 !== 40.0) {
  throw '#4: 4e+1 === 40';
}

//CHe+CK#5
if (5e+1 !== 50.0) {
  throw '#5: 5e+1 === 50';
}

//CHe+CK#6
if (6e+1 !== 60.0) {
  throw '#6: 6e+1 === 60';
}

//CHe+CK#7
if (7e+1 !== 70.0) {
  throw '#7: 7e+1 === 70';
}

//CHe+CK#8
if (8e+1 !== 80.0) {
  throw '#8: 8e+1 === 80';
}

//CHe+CK#9
if (9e+1 !== 90.0) {
  throw '#9: 9e+1 === 90';
}


//? test: S7.8.3_A1.2_T6
//? description: "ExponentPart :: E +DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0E+1 !== 0.0) {
  throw '#0: 0E+1 === 0';
}

//CHE+CK#1
if (1E+1 !== 10.0) {
  throw '#1: 1E+1 === 10';
}

//CHE+CK#2
if (2E+1 !== 20.0) {
  throw '#2: 2E+1 === 20';
}

//CHE+CK#3
if (3E+1 !== 30.0) {
  throw '#3: 3E+1 === 30';
}

//CHE+CK#4
if (4E+1 !== 40.0) {
  throw '#4: 4E+1 === 40';
}

//CHE+CK#5
if (5E+1 !== 50.0) {
  throw '#5: 5E+1 === 50';
}

//CHE+CK#6
if (6E+1 !== 60.0) {
  throw '#6: 6E+1 === 60';
}

//CHE+CK#7
if (7E+1 !== 70.0) {
  throw '#7: 7E+1 === 70';
}

//CHE+CK#8
if (8E+1 !== 80.0) {
  throw '#8: 8E+1 === 80';
}

//CHE+CK#9
if (9E+1 !== 90.0) {
  throw '#9: 9E+1 === 90';
}


//? test: S7.8.3_A1.2_T7
//? description: "ExponentPart :: e 0"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0e0 !== 0.0) {
  throw '#0: 0e0 === 0';
}

//CHECK#1
if (1e0 !== 1.0) {
  throw '#1: 1e0 === 1';
}

//CHECK#2
if (2e0 !== 2.0) {
  throw '#2: 2e0 === 2';
}

//CHECK#3
if (3e0 !== 3.0) {
  throw '#3: 3e0 === 3';
}

//CHECK#4
if (4e0 !== 4.0) {
  throw '#4: 4e0 === 4';
}

//CHECK#5
if (5e0 !== 5.0) {
  throw '#5: 5e0 === 5';
}

//CHECK#6
if (6e0 !== 6.0) {
  throw '#6: 6e0 === 6';
}

//CHECK#7
if (7e0 !== 7.0) {
  throw '#7: 7e0 === 7';
}

//CHECK#8
if (8e0 !== 8.0) {
  throw '#8: 8e0 === 8';
}

//CHECK#9
if (9e0 !== 9.0) {
  throw '#9: 9e0 === 9';
}


//? test: S7.8.3_A1.2_T8
//? description: "ExponentPart :: E 0"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0E0 !== 0.0) {
  throw '#0: 0E0 === 0';
}

//CHECK#1
if (1E0 !== 1.0) {
  throw '#1: 1E0 === 1';
}

//CHECK#2
if (2E0 !== 2.0) {
  throw '#2: 2E0 === 2';
}

//CHECK#3
if (3E0 !== 3.0) {
  throw '#3: 3E0 === 3';
}

//CHECK#4
if (4E0 !== 4.0) {
  throw '#4: 4E0 === 4';
}

//CHECK#5
if (5E0 !== 5.0) {
  throw '#5: 5E0 === 5';
}

//CHECK#6
if (6E0 !== 6.0) {
  throw '#6: 6E0 === 6';
}

//CHECK#7
if (7E0 !== 7.0) {
  throw '#7: 7E0 === 7';
}

//CHECK#8
if (8E0 !== 8.0) {
  throw '#8: 8E0 === 8';
}

//CHECK#9
if (9E0 !== 9.0) {
  throw '#9: 9E0 === 9';
}


//? test: S7.8.3_A2.1_T1
//? description: Use .DecimalDigit
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.0 !== 0.0) {
  throw '#0: .0 === 0.0';
}

//CHECK#1
if (.1 !== 0.1) {
  throw '#1: .1 === 0.1';
}

//CHECK#2
if (.2 !== 0.2) {
  throw '#2: .2 === 0.2';
}

//CHECK#3
if (.3 !== 0.3) {
  throw '#3: .3 === 0.3';
}

//CHECK#4
if (.4 !== 0.4) {
  throw '#4: .4 === 0.4';
}

//CHECK#5
if (.5 !== 0.5) {
  throw '#5: .5 === 0.5';
}

//CHECK#6
if (.6 !== 0.6) {
  throw '#6: .6 === 0.6';
}

//CHECK#7
if (.7 !== 0.7) {
  throw '#7: .7 === 0.7';
}

//CHECK#8
if (.8 !== 0.8) {
  throw '#8: .8 === 0.8';
}

//CHECK#9
if (.9 !== 0.9) {
  throw '#9: .9 === 0.9';
}


//? test: S7.8.3_A2.1_T2
//? description: Use .DecimalDigits
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.00 !== 0.00) {
  throw '#0: .00 === 0.00';
}

//CHECK#1
if (.11 !== 0.11) {
  throw '#1: .11 === 0.11';
}

//CHECK#2
if (.22 !== 0.22) {
  throw '#2: .22 === 0.22';
}

//CHECK#3
if (.33 !== 0.33) {
  throw '#3: .33 === 0.33';
}

//CHECK#4
if (.44 !== 0.44) {
  throw '#4: .44 === 0.44';
}

//CHECK#5
if (.55 !== 0.55) {
  throw '#5: .55 === 0.55';
}

//CHECK#6
if (.66 !== 0.66) {
  throw '#6: .66 === 0.66';
}

//CHECK#7
if (.77 !== 0.77) {
  throw '#7: .77 === 0.77';
}

//CHECK#8
if (.88 !== 0.88) {
  throw '#8: .88 === 0.88';
}

//CHECK#9
if (.99 !== 0.99) {
  throw '#9: .99 === 0.99';
}


//? test: S7.8.3_A2.1_T3
//? description: Use .DecimalDigits that have at the end zeros
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.00 !== 0.0) {
  throw '#0: .0 === 0.0';
}

//CHECK#1
if (.10 !== 0.1) {
  throw '#1: .1 === 0.1';
}

//CHECK#2
if (.20 !== 0.2) {
  throw '#2: .2 === 0.2';
}

//CHECK#3
if (.30 !== 0.3) {
  throw '#3: .3 === 0.3';
}

//CHECK#4
if (.40 !== 0.4) {
  throw '#4: .4 === 0.4';
}

//CHECK#5
if (.50 !== 0.5) {
  throw '#5: .5 === 0.5';
}

//CHECK#6
if (.60 !== 0.6) {
  throw '#6: .6 === 0.6';
}

//CHECK#7
if (.70 !== 0.7) {
  throw '#7: .7 === 0.7';
}

//CHECK#8
if (.80 !== 0.8) {
  throw '#8: .8 === 0.8';
}

//CHECK#9
if (.90 !== 0.9) {
  throw '#9: .9 === 0.9';
}


//? test: S7.8.3_A2.2_T1
//? description: "ExponentPart :: e DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.0e1 !== 0.0) {
  throw '#0: .0e1 === 0';
}

//CHECK#1
if (.1e1 !== 1.0) {
  throw '#1: .1e1 === 1';
}

//CHECK#2
if (.2e1 !== 2.0) {
  throw '#2: .2e1 === 2';
}

//CHECK#3
if (.3e1 !== 3.0) {
  throw '#3: .3e1 === 3';
}

//CHECK#4
if (.4e1 !== 4.0) {
  throw '#4: .4e1 === 4';
}

//CHECK#5
if (.5e1 !== 5.0) {
  throw '#5: .5e1 === 5';
}

//CHECK#6
if (.6e1 !== 6.0) {
  throw '#6: .6e1 === 6';
}

//CHECK#7
if (.7e1 !== 7.0) {
  throw '#7: .7e1 === 7';
}

//CHECK#8
if (.8e1 !== 8.0) {
  throw '#8: .8e1 === 8';
}

//CHECK#9
if (.9e1 !== 9.0) {
  throw '#9: .9e1 === 9';
}


//? test: S7.8.3_A2.2_T2
//? description: "ExponentPart :: E DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.0E1 !== 0.0) {
  throw '#0: .0E1 === 0';
}

//CHECK#1
if (.1E1 !== 1.0) {
  throw '#1: .1E1 === 1';
}

//CHECK#2
if (.2E1 !== 2.0) {
  throw '#2: .2E1 === 2';
}

//CHECK#3
if (.3E1 !== 3.0) {
  throw '#3: .3E1 === 3';
}

//CHECK#4
if (.4E1 !== 4.0) {
  throw '#4: .4E1 === 4';
}

//CHECK#5
if (.5E1 !== 5.0) {
  throw '#5: .5E1 === 5';
}

//CHECK#6
if (.6E1 !== 6.0) {
  throw '#6: .6E1 === 6';
}

//CHECK#7
if (.7E1 !== 7.0) {
  throw '#7: .7E1 === 7';
}

//CHECK#8
if (.8E1 !== 8.0) {
  throw '#8: .8E1 === 8';
}

//CHECK#9
if (.9E1 !== 9.0) {
  throw '#9: .9E1 === 9';
}


//? test: S7.8.3_A2.2_T3
//? description: "ExponentPart :: e +DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.0e-1 !== 0.0) {
  throw '#0: .0e-1 === 0';
}

//CHECK#1
if (.1e-1 !== 0.01) {
  throw '#1: .1e-1 === 0.01';
}

//CHECK#2
if (.2e-1 !== 0.02) {
  throw '#2: .2e-1 === 0.02';
}

//CHECK#3
if (.3e-1 !== 0.03) {
  throw '#3: .3e-1 === 0.03';
}

//CHECK#4
if (.4e-1 !== 0.04) {
  throw '#4: .4e-1 === 0.04';
}

//CHECK#5
if (.5e-1 !== 0.05) {
  throw '#5: .5e-1 === 0.05';
}

//CHECK#6
if (.6e-1 !== 0.06) {
  throw '#6: .6e-1 === 0.06';
}

//CHECK#7
if (.7e-1 !== 0.07) {
  throw '#7: .7e-1 === 0.07';
}

//CHECK#8
if (.8e-1 !== 0.08) {
  throw '#8: .8e-1 === 0.08';
}

//CHECK#9
if (.9e-1 !== 0.09) {
  throw '#9: .9e-1 === 0.09';
}


//? test: S7.8.3_A2.2_T4
//? description: "ExponentPart :: E +DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.0E-1 !== 0.0) {
  throw '#0: .0E-1 === 0';
}

//CHECK#1
if (.1E-1 !== 0.01) {
  throw '#1: .1E-1 === 0.01';
}

//CHECK#2
if (.2E-1 !== 0.02) {
  throw '#2: .2E-1 === 0.02';
}

//CHECK#3
if (.3E-1 !== 0.03) {
  throw '#3: .3E-1 === 0.03';
}

//CHECK#4
if (.4E-1 !== 0.04) {
  throw '#4: .4E-1 === 0.04';
}

//CHECK#5
if (.5E-1 !== 0.05) {
  throw '#5: .5E-1 === 0.05';
}

//CHECK#6
if (.6E-1 !== 0.06) {
  throw '#6: .6E-1 === 0.06';
}

//CHECK#7
if (.7E-1 !== 0.07) {
  throw '#7: .7E-1 === 0.07';
}

//CHECK#8
if (.8E-1 !== 0.08) {
  throw '#8: .8E-1 === 0.08';
}

//CHECK#9
if (.9E-1 !== 0.09) {
  throw '#9: .9E-1 === 0.09';
}


//? test: S7.8.3_A2.2_T5
//? description: "ExponentPart :: e +DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.0e+1 !== 0.0) {
  throw '#0: .0e+1 === 0';
}

//CHECK#1
if (.1e+1 !== 1.0) {
  throw '#1: .1e+1 === 1';
}

//CHECK#2
if (.2e+1 !== 2.0) {
  throw '#2: .2e+1 === 2';
}

//CHECK#3
if (.3e+1 !== 3.0) {
  throw '#3: .3e+1 === 3';
}

//CHECK#4
if (.4e+1 !== 4.0) {
  throw '#4: .4e+1 === 4';
}

//CHECK#5
if (.5e+1 !== 5.0) {
  throw '#5: .5e+1 === 5';
}

//CHECK#6
if (.6e+1 !== 6.0) {
  throw '#6: .6e+1 === 6';
}

//CHECK#7
if (.7e+1 !== 7.0) {
  throw '#7: .7e+1 === 7';
}

//CHECK#8
if (.8e+1 !== 8.0) {
  throw '#8: .8e+1 === 8';
}

//CHECK#9
if (.9e+1 !== 9.0) {
  throw '#9: .9e+1 === 9';
}


//? test: S7.8.3_A2.2_T6
//? description: "ExponentPart :: E +DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.0E+1 !== 0.0) {
  throw '#0: .0E+1 === 0';
}

//CHECK#1
if (.1E+1 !== 1.0) {
  throw '#1: .1E+1 === 1';
}

//CHECK#2
if (.2E+1 !== 2.0) {
  throw '#2: .2E+1 === 2';
}

//CHECK#3
if (.3E+1 !== 3.0) {
  throw '#3: .3E+1 === 3';
}

//CHECK#4
if (.4E+1 !== 4.0) {
  throw '#4: .4E+1 === 4';
}

//CHECK#5
if (.5E+1 !== 5.0) {
  throw '#5: .5E+1 === 5';
}

//CHECK#6
if (.6E+1 !== 6.0) {
  throw '#6: .6E+1 === 6';
}

//CHECK#7
if (.7E+1 !== 7.0) {
  throw '#7: .7E+1 === 7';
}

//CHECK#8
if (.8E+1 !== 8.0) {
  throw '#8: .8E+1 === 8';
}

//CHECK#9
if (.9E+1 !== 9.0) {
  throw '#9: .9E+1 === 9';
}


//? test: S7.8.3_A2.2_T7
//? description: "ExponentPart :: e 0"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.0e0 !== 0.0) {
  throw '#0: .0e0 === 0.0';
}

//CHECK#1
if (.1e0 !== 0.1) {
  throw '#1: .1e0 === 0.1';
}

//CHECK#2
if (.2e0 !== 0.2) {
  throw '#2: .2e0 === 0.2';
}

//CHECK#3
if (.3e0 !== 0.3) {
  throw '#3: .3e0 === 0.3';
}

//CHECK#4
if (.4e0 !== 0.4) {
  throw '#4: .4e0 === 0.4';
}

//CHECK#5
if (.5e0 !== 0.5) {
  throw '#5: .5e0 === 0.5';
}

//CHECK#6
if (.6e0 !== 0.6) {
  throw '#6: .6e0 === 0.6';
}

//CHECK#7
if (.7e0 !== 0.7) {
  throw '#7: .7e0 === 0.7';
}

//CHECK#8
if (.8e0 !== 0.8) {
  throw '#8: .8e0 === 0.8';
}

//CHECK#9
if (.9e0 !== 0.9) {
  throw '#9: .9e0 === 0.9';
}


//? test: S7.8.3_A2.2_T8
//? description: "ExponentPart :: E 0"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (.0E0 !== 0.0) {
  throw '#0: .0E0 === 0.0';
}

//CHECK#1
if (.1E0 !== 0.1) {
  throw '#1: .1E0 === 0.1';
}

//CHECK#2
if (.2E0 !== 0.2) {
  throw '#2: .2E0 === 0.2';
}

//CHECK#3
if (.3E0 !== 0.3) {
  throw '#3: .3E0 === 0.3';
}

//CHECK#4
if (.4E0 !== 0.4) {
  throw '#4: .4E0 === 0.4';
}

//CHECK#5
if (.5E0 !== 0.5) {
  throw '#5: .5E0 === 0.5';
}

//CHECK#6
if (.6E0 !== 0.6) {
  throw '#6: .6E0 === 0.6';
}

//CHECK#7
if (.7E0 !== 0.7) {
  throw '#7: .7E0 === 0.7';
}

//CHECK#8
if (.8E0 !== 0.8) {
  throw '#8: .8E0 === 0.8';
}

//CHECK#9
if (.9E0 !== 0.9) {
  throw '#9: .9E0 === 0.9';
}


//? test: S7.8.3_A3.1_T1
//? description: "DecimalIntegerLiteral :: 0, NoNZeroDigit"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or not?

//CHECK#0
if (0. !== 0.0) {
  throw '#0: 0. === 0';
}

//CHECK#1
if (1. !== 1.0) {
  throw '#1: 1. === 1';
}

//CHECK#2
if (2. !== 2.0) {
  throw '#2: 2. === 2';
}

//CHECK#3
if (3. !== 3.0) {
  throw '#3: 3. === 3';
}

//CHECK#4
if (4. !== 4.0) {
  throw '#4: 4. === 4';
}

//CHECK#5
if (5. !== 5.0) {
  throw '#5: 5. === 5';
}

//CHECK#6
if (6. !== 6.0) {
  throw '#6: 6. === 6';
}

//CHECK#7
if (7. !== 7.0) {
  throw '#7: 7. === 7';
}

//CHECK#8
if (8. !== 8.0) {
  throw '#8: 8. === 8';
}

//CHECK#9
if (9. !== 9.0) {
  throw '#9: 9. === 9';
}


//? test: S7.8.3_A3.1_T2
//? description: "DecimalIntegerLiteral :: NoNZeroDigit DecimalDigigts"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme implement these or no?

//CHECK#1
if (11. !== 11.0) {
  throw '#1: 11. === 11';
}

//CHECK#2
if (22. !== 22.0) {
  throw '#2: 22. === 22';
}

//CHECK#3
if (33. !== 33.0) {
  throw '#3: 33. === 33';
}

//CHECK#4
if (44. !== 44.0) {
  throw '#4: 44. === 44';
}

//CHECK#5
if (55. !== 55.0) {
  throw '#5: 55. === 55';
}

//CHECK#6
if (66. !== 66.0) {
  throw '#6: 66. === 66';
}

//CHECK#7
if (77. !== 77.0) {
  throw '#7: 77. === 77';
}

//CHECK#8
if (88. !== 88.0) {
  throw '#8: 88. === 88';
}

//CHECK#9
if (99. !== 99.0) {
  throw '#9: 99. === 99';
}


//? test: S7.8.3_A3.2_T1
//? description: After DecimalIntegerLiteral. used ZeroDigit
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.0 !== 0.0) {
  throw '#0: 0.0 === 0';
}

//CHECK#1
if (1.0 !== 1.0) {
  throw '#1: 1.0 === 1';
}

//CHECK#2
if (2.0 !== 2.0) {
  throw '#2: 2.0 === 2';
}

//CHECK#3
if (3.0 !== 3.0) {
  throw '#3: 3.0 === 3';
}

//CHECK#4
if (4.0 !== 4.0) {
  throw '#4: 4.0 === 4';
}

//CHECK#5
if (5.0 !== 5.0) {
  throw '#5: 5.0 === 5';
}

//CHECK#6
if (6.0 !== 6.0) {
  throw '#6: 6.0 === 6';
}

//CHECK#7
if (7.0 !== 7.0) {
  throw '#7: 7.0 === 7';
}

//CHECK#8
if (8.0 !== 8.0) {
  throw '#8: 8.0 === 8';
}

//CHECK#9
if (9.0 !== 9.0) {
  throw '#9: 9.0 === 9';
}


//? test: S7.8.3_A3.2_T2
//? description: After DecimalIntegerLiteral. used ZeroDigits
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.00 !== 0.0) {
  throw '#0: 0.00 === 0';
}

//CHECK#1
if (1.00 !== 1.0) {
  throw '#1: 1.00 === 1';
}

//CHECK#2
if (2.00 !== 2.0) {
  throw '#2: 2.00 === 2';
}

//CHECK#3
if (3.00 !== 3.0) {
  throw '#3: 3.00 === 3';
}

//CHECK#4
if (4.00 !== 4.0) {
  throw '#4: 4.00 === 4';
}

//CHECK#5
if (5.00 !== 5.0) {
  throw '#5: 5.00 === 5';
}

//CHECK#6
if (6.00 !== 6.0) {
  throw '#6: 6.00 === 6';
}

//CHECK#7
if (7.00 !== 7.0) {
  throw '#7: 7.00 === 7';
}

//CHECK#8
if (8.00 !== 8.0) {
  throw '#8: 8.00 === 8';
}

//CHECK#9
if (9.00 !== 9.0) {
  throw '#9: 9.00 === 9';
}


//? test: S7.8.3_A3.2_T3
//? description: After DecimalIntegerLiteral. used NoNZeroDigit
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.0 !== 0.0) {
  throw '#0: 0.0 === 0.0';
}

//CHECK#1
if (1.1 !== 1.1) {
  throw '#1: 1.1 === 1.1';
}

//CHECK#2
if (2.2 !== 2.2) {
  throw '#2: 2.2 === 2.2';
}

//CHECK#3
if (3.3 !== 3.3) {
  throw '#3: 3.3 === 3.3';
}

//CHECK#4
if (4.4 !== 4.4) {
  throw '#4: 4.4 === 4.4';
}

//CHECK#5
if (5.5 !== 5.5) {
  throw '#5: 5.5 === 5.5';
}

//CHECK#6
if (6.6 !== 6.6) {
  throw '#6: 6.6 === 6.6';
}

//CHECK#7
if (7.7 !== 7.7) {
  throw '#7: 7.7 === 7.7';
}

//CHECK#8
if (8.8 !== 8.8) {
  throw '#8: 8..8 === 8.8';
}

//CHECK#9
if (9.9 !== 9.9) {
  throw '#9: 9.9 === 9.9';
}


//? test: S7.8.3_A3.3_T1
//? description: "ExponentPart :: e DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should these be legal exponent expressions?

//CHECK#0
if (0.e1 !== 0.0) {
  throw '#0: 0.e1 === 0';
}

//CHECK#1
if (1.e1 !== 10.0) {
  throw '#1: 1.e1 === 10';
}

//CHECK#2
if (2.e1 !== 20.0) {
  throw '#2: 2.e1 === 20';
}

//CHECK#3
if (3.e1 !== 30.0) {
  throw '#3: 3.e1 === 30';
}

//CHECK#4
if (4.e1 !== 40.0) {
  throw '#4: 4.e1 === 40';
}

//CHECK#5
if (5.e1 !== 50.0) {
  throw '#5: 5.e1 === 50';
}

//CHECK#6
if (6.e1 !== 60.0) {
  throw '#6: 6.e1 === 60';
}

//CHECK#7
if (7.e1 !== 70.0) {
  throw '#7: 7.e1 === 70';
}

//CHECK#8
if (8.e1 !== 80.0) {
  throw '#8: 8.e1 === 80';
}

//CHECK#9
if (9.e1 !== 90.0) {
  throw '#9: 9.e1 === 90';
}


//? test: S7.8.3_A3.3_T2
//? description: "ExponentPart :: E DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should these be legal exponent expressions?

//CHECK#0
if (0.E1 !== 0.0) {
  throw '#0: 0.E1 === 0';
}

//CHECK#1
if (1.E1 !== 10.0) {
  throw '#1: 1.E1 === 10';
}

//CHECK#2
if (2.E1 !== 20.0) {
  throw '#2: 2.E1 === 20';
}

//CHECK#3
if (3.E1 !== 30.0) {
  throw '#3: 3.E1 === 30';
}

//CHECK#4
if (4.E1 !== 40.0) {
  throw '#4: 4.E1 === 40';
}

//CHECK#5
if (5.E1 !== 50.0) {
  throw '#5: 5.E1 === 50';
}

//CHECK#6
if (6.E1 !== 60.0) {
  throw '#6: 6.E1 === 60';
}

//CHECK#7
if (7.E1 !== 70.0) {
  throw '#7: 7.E1 === 70';
}

//CHECK#8
if (8.E1 !== 80.0) {
  throw '#8: 8.E1 === 80';
}

//CHECK#9
if (9.E1 !== 90.0) {
  throw '#9: 9.E1 === 90';
}


//? test: S7.8.3_A3.3_T3
//? description: "ExponentPart :: e -DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should these be legal exponent expressions?

//CHECK#0
if (0.e-1 !== 0.0) {
  throw '#0: 0.e-1 === 0';
}

//CHECK#1
if (1.e-1 !== 0.1) {
  throw '#1: 1.e-1 === 0.1';
}

//CHECK#2
if (2.e-1 !== 0.2) {
  throw '#2: 2.e-1 === 0.2';
}

//CHECK#3
if (3.e-1 !== 0.3) {
  throw '#3: 3.e-1 === 0.3';
}

//CHECK#4
if (4.e-1 !== 0.4) {
  throw '#4: 4.e-1 === 0.4';
}

//CHECK#5
if (5.e-1 !== 0.5) {
  throw '#5: 5.e-1 === 0.5';
}

//CHECK#6
if (6.e-1 !== 0.6) {
  throw '#6: 6.e-1 === 0.6';
}

//CHECK#7
if (7.e-1 !== 0.7) {
  throw '#7: 7.e-1 === 0.7';
}

//CHECK#8
if (8.e-1 !== 0.8) {
  throw '#8: 8.e-1 === 0.8';
}

//CHECK#9
if (9.e-1 !== 0.9) {
  throw '#9: 9.e-1 === 0.9';
}


//? test: S7.8.3_A3.3_T4
//? description: "ExponentPart :: E -DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should these be legal exponent expressions?

//CHECK#0
if (0.E-1 !== 0.0) {
  throw '#0: 0.E-1 === 0';
}

//CHECK#1
if (1.E-1 !== 0.1) {
  throw '#1: 1.E-1 === 0.1';
}

//CHECK#2
if (2.E-1 !== 0.2) {
  throw '#2: 2.E-1 === 0.2';
}

//CHECK#3
if (3.E-1 !== 0.3) {
  throw '#3: 3.E-1 === 0.3';
}

//CHECK#4
if (4.E-1 !== 0.4) {
  throw '#4: 4.E-1 === 0.4';
}

//CHECK#5
if (5.E-1 !== 0.5) {
  throw '#5: 5.E-1 === 0.5';
}

//CHECK#6
if (6.E-1 !== 0.6) {
  throw '#6: 6.E-1 === 0.6';
}

//CHECK#7
if (7.E-1 !== 0.7) {
  throw '#7: 7.E-1 === 0.7';
}

//CHECK#8
if (8.E-1 !== 0.8) {
  throw '#8: 8.E-1 === 0.8';
}

//CHECK#9
if (9.E-1 !== 0.9) {
  throw '#9: 9.E-1 === 0.9';
}


//? test: S7.8.3_A3.3_T5
//? description: "ExponentPart :: e +DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should these be legal exponent expressions?

//CHECK#0
if (0.e+1 !== 0.0) {
  throw '#0: 0.e+1 === 0';
}

//CHECK#1
if (1.e+1 !== 10.0) {
  throw '#1: 1.e+1 === 10';
}

//CHECK#2
if (2.e+1 !== 20.0) {
  throw '#2: 2.e+1 === 20';
}

//CHECK#3
if (3.e+1 !== 30.0) {
  throw '#3: 3.e+1 === 30';
}

//CHECK#4
if (4.e+1 !== 40.0) {
  throw '#4: 4.e+1 === 40';
}

//CHECK#5
if (5.e+1 !== 50.0) {
  throw '#5: 5.e+1 === 50';
}

//CHECK#6
if (6.e+1 !== 60.0) {
  throw '#6: 6.e+1 === 60';
}

//CHECK#7
if (7.e+1 !== 70.0) {
  throw '#7: 7.e+1 === 70';
}

//CHECK#8
if (8.e+1 !== 80.0) {
  throw '#8: 8.e+1 === 80';
}

//CHECK#9
if (9.e+1 !== 90.0) {
  throw '#9: 9.e+1 === 90';
}


//? test: S7.8.3_A3.3_T6
//? description: "ExponentPart :: E +DecimalDigits"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should these be legal exponent expressions?

//CHECK#0
if (0.E+1 !== 0.0) {
  throw '#0: 0.E+1 === 0';
}

//CHECK#1
if (1.E+1 !== 10.0) {
  throw '#1: 1.E+1 === 10';
}

//CHECK#2
if (2.E+1 !== 20.0) {
  throw '#2: 2.E+1 === 20';
}

//CHECK#3
if (3.E+1 !== 30.0) {
  throw '#3: 3.E+1 === 30';
}

//CHECK#4
if (4.E+1 !== 40.0) {
  throw '#4: 4.E+1 === 40';
}

//CHECK#5
if (5.E+1 !== 50.0) {
  throw '#5: 5.E+1 === 50';
}

//CHECK#6
if (6.E+1 !== 60.0) {
  throw '#6: 6.E+1 === 60';
}

//CHECK#7
if (7.E+1 !== 70.0) {
  throw '#7: 7.E+1 === 70';
}

//CHECK#8
if (8.E+1 !== 80.0) {
  throw '#8: 8.E+1 === 80';
}

//CHECK#9
if (9.E+1 !== 90.0) {
  throw '#9: 9.E+1 === 90';
}


//? test: S7.8.3_A3.3_T7
//? description: "ExponentPart :: e 0"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should these be legal exponent expressions?

//CHECK#0
if (0.e0 !== 0.0) {
  throw '#0: 0.e0 === 0';
}

//CHECK#1
if (1.e0 !== 1.0) {
  throw '#1: 1.e0 === 1';
}

//CHECK#2
if (2.e0 !== 2.0) {
  throw '#2: 2.e0 === 2';
}

//CHECK#3
if (3.e0 !== 3.0) {
  throw '#3: 3.e0 === 3';
}

//CHECK#4
if (4.e0 !== 4.0) {
  throw '#4: 4.e0 === 4';
}

//CHECK#5
if (5.e0 !== 5.0) {
  throw '#5: 5.e0 === 5';
}

//CHECK#6
if (6.e0 !== 6.0) {
  throw '#6: 6.e0 === 6';
}

//CHECK#7
if (7.e0 !== 7.0) {
  throw '#7: 7.e0 === 7';
}

//CHECK#8
if (8.e0 !== 8.0) {
  throw '#8: 8.e0 === 8';
}

//CHECK#9
if (9.e0 !== 9.0) {
  throw '#9: 9.e0 === 9';
}


//? test: S7.8.3_A3.3_T8
//? description: "ExponentPart :: E 0"
//? expect: null
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should these be legal exponent expressions?

//CHECK#0
if (0.E0 !== 0.0) {
  throw '#0: 0.E0 === 0';
}

//CHECK#1
if (1.E0 !== 1.0) {
  throw '#1: 1.E0 === 1';
}

//CHECK#2
if (2.E0 !== 2.0) {
  throw '#2: 2.E0 === 2';
}

//CHECK#3
if (3.E0 !== 3.0) {
  throw '#3: 3.E0 === 3';
}

//CHECK#4
if (4.E0 !== 4.0) {
  throw '#4: 4.E0 === 4';
}

//CHECK#5
if (5.E0 !== 5.0) {
  throw '#5: 5.E0 === 5';
}

//CHECK#6
if (6.E0 !== 6.0) {
  throw '#6: 6.E0 === 6';
}

//CHECK#7
if (7.E0 !== 7.0) {
  throw '#7: 7.E0 === 7';
}

//CHECK#8
if (8.E0 !== 8.0) {
  throw '#8: 8.E0 === 8';
}

//CHECK#9
if (9.E0 !== 9.0) {
  throw '#9: 9.E0 === 9';
}


//? test: S7.8.3_A3.4_T1
//? description: "ExponentPart :: e DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.0e1 !== 0.0) {
  throw '#0: 0.0e1 === 0';
}

//CHECK#1
if (1.1e1 !== 11.0) {
  throw '#1: 1.1e1 === 11';
}

//CHECK#2
if (2.2e1 !== 22.0) {
  throw '#2: 2.2e1 === 22';
}

//CHECK#3
if (3.3e1 !== 33.0) {
  throw '#3: 3.3e1 === 33';
}

//CHECK#4
if (4.4e1 !== 44.0) {
  throw '#4: 4.4e1 === 44';
}

//CHECK#5
if (5.5e1 !== 55.0) {
  throw '#5: 5.5e1 === 55';
}

//CHECK#6
if (6.6e1 !== 66.0) {
  throw '#6: 6.e1 === 66';
}

//CHECK#7
if (7.7e1 !== 77.0) {
  throw '#7: 7.7e1 === 77';
}

//CHECK#8
if (8.8e1 !== 88.0) {
  throw '#8: 8.8e1 === 88';
}

//CHECK#9
if (9.9e1 !== 99.0) {
  throw '#9: 9.9e1 === 99';
}


//? test: S7.8.3_A3.4_T2
//? description: "ExponentPart :: E DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.0E1 !== 0.0) {
  throw '#0: 0.0E1 === 0';
}

//CHECK#1
if (1.1E1 !== 11.0) {
  throw '#1: 1.1E1 === 11';
}

//CHECK#2
if (2.2E1 !== 22.0) {
  throw '#2: 2.2E1 === 22';
}

//CHECK#3
if (3.3E1 !== 33.0) {
  throw '#3: 3.3E1 === 33';
}

//CHECK#4
if (4.4E1 !== 44.0) {
  throw '#4: 4.4E1 === 44';
}

//CHECK#5
if (5.5E1 !== 55.0) {
  throw '#5: 5.5E1 === 55';
}

//CHECK#6
if (6.6E1 !== 66.0) {
  throw '#6: 6.E1 === 66';
}

//CHECK#7
if (7.7E1 !== 77.0) {
  throw '#7: 7.7E1 === 77';
}

//CHECK#8
if (8.8E1 !== 88.0) {
  throw '#8: 8.8E1 === 88';
}

//CHECK#9
if (9.9E1 !== 99.0) {
  throw '#9: 9.9E1 === 99';
}


//? test: S7.8.3_A3.4_T3
//? description: "ExponentPart :: e -DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.0e-1 !== 0.0) {
  throw '#0: 0.0e-1 === 0';
}

//CHECK#1
if (1.1e-1 !== 0.11) {
  throw '#1: 1.1e-1 === 0.11';
}

//CHECK#2
if (2.2e-1 !== 0.22) {
  throw '#2: 2.2e-1 === 0.22';
}

//CHECK#3
if (3.3e-1 !== 0.33) {
  throw '#3: 3.3e-1 === 0.33';
}

//CHECK#4
if (4.4e-1 !== 0.44) {
  throw '#4: 4.4e-1 === 0.44';
}

//CHECK#5
if (5.5e-1 !== 0.55) {
  throw '#5: 5.5e-1 === 0.55';
}

//CHECK#6
if (6.6e-1 !== 0.66) {
  throw '#6: 6.e-1 === 0.66';
}

//CHECK#7
if (7.7e-1 !== 0.77) {
  throw '#7: 7.7e-1 === 0.77';
}

//CHECK#8
if (8.8e-1 !== 0.88) {
  throw '#8: 8.8e-1 === 0.88';
}

//CHECK#9
if (9.9e-1 !== 0.99) {
  throw '#9: 9.9e-1 === 0.99';
}


//? test: S7.8.3_A3.4_T4
//? description: "ExponentPart :: E -DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.0E-1 !== 0.0) {
  throw '#0: 0.0E-1 === 0';
}

//CHECK#1
if (1.1E-1 !== 0.11) {
  throw '#1: 1.1E-1 === 0.11';
}

//CHECK#2
if (2.2E-1 !== 0.22) {
  throw '#2: 2.2E-1 === 0.22';
}

//CHECK#3
if (3.3E-1 !== 0.33) {
  throw '#3: 3.3E-1 === 0.33';
}

//CHECK#4
if (4.4E-1 !== 0.44) {
  throw '#4: 4.4E-1 === 0.44';
}

//CHECK#5
if (5.5E-1 !== 0.55) {
  throw '#5: 5.5E-1 === 0.55';
}

//CHECK#6
if (6.6E-1 !== 0.66) {
  throw '#6: 6.E-1 === 0.66';
}

//CHECK#7
if (7.7E-1 !== 0.77) {
  throw '#7: 7.7E-1 === 0.77';
}

//CHECK#8
if (8.8E-1 !== 0.88) {
  throw '#8: 8.8E-1 === 0.88';
}

//CHECK#9
if (9.9E-1 !== 0.99) {
  throw '#9: 9.9E-1 === 0.99';
}


//? test: S7.8.3_A3.4_T5
//? description: "ExponentPart :: e +DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.0e+1 !== 0.0) {
  throw '#0: 0.0e+1 === 0';
}

//CHECK#1
if (1.1e+1 !== 11.0) {
  throw '#1: 1.1e+1 === 11';
}

//CHECK#2
if (2.2e+1 !== 22.0) {
  throw '#2: 2.2e+1 === 22';
}

//CHECK#3
if (3.3e+1 !== 33.0) {
  throw '#3: 3.3e+1 === 33';
}

//CHECK#4
if (4.4e+1 !== 44.0) {
  throw '#4: 4.4e+1 === 44';
}

//CHECK#5
if (5.5e+1 !== 55.0) {
  throw '#5: 5.5e+1 === 55';
}

//CHECK#6
if (6.6e+1 !== 66.0) {
  throw '#6: 6.e+1 === 66';
}

//CHECK#7
if (7.7e+1 !== 77.0) {
  throw '#7: 7.7e+1 === 77';
}

//CHECK#8
if (8.8e+1 !== 88.0) {
  throw '#8: 8.8e+1 === 88';
}

//CHECK#9
if (9.9e+1 !== 99.0) {
  throw '#9: 9.9e+1 === 99';
}


//? test: S7.8.3_A3.4_T6
//? description: "ExponentPart :: E +DecimalDigits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.0E+1 !== 0.0) {
  throw '#0: 0.0E+1 === 0';
}

//CHECK#1
if (1.1E+1 !== 11.0) {
  throw '#1: 1.1E+1 === 11';
}

//CHECK#2
if (2.2E+1 !== 22.0) {
  throw '#2: 2.2E+1 === 22';
}

//CHECK#3
if (3.3E+1 !== 33.0) {
  throw '#3: 3.3E+1 === 33';
}

//CHECK#4
if (4.4E+1 !== 44.0) {
  throw '#4: 4.4E+1 === 44';
}

//CHECK#5
if (5.5E+1 !== 55.0) {
  throw '#5: 5.5E+1 === 55';
}

//CHECK#6
if (6.6E+1 !== 66.0) {
  throw '#6: 6.E+1 === 66';
}

//CHECK#7
if (7.7E+1 !== 77.0) {
  throw '#7: 7.7E+1 === 77';
}

//CHECK#8
if (8.8E+1 !== 88.0) {
  throw '#8: 8.8E+1 === 88';
}

//CHECK#9
if (9.9E+1 !== 99.0) {
  throw '#9: 9.9E+1 === 99';
}


//? test: S7.8.3_A3.4_T7
//? description: "ExponentPart :: e 0"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.0e0 !== 0.0) {
  throw '#0: 0.0e0 === 0.0';
}

//CHECK#1
if (1.1e0 !== 1.1) {
  throw '#1: 1.1e0 === 1.1';
}

//CHECK#2
if (2.2e0 !== 2.2) {
  throw '#2: 2.2e0 === 2.2';
}

//CHECK#3
if (3.3e0 !== 3.3) {
  throw '#3: 3.3e0 === 3.3';
}

//CHECK#4
if (4.4e0 !== 4.4) {
  throw '#4: 4.4e0 === 4.4';
}

//CHECK#5
if (5.5e0 !== 5.5) {
  throw '#5: 5.5e0 === 5.5';
}

//CHECK#6
if (6.6e0 !== 6.6) {
  throw '#6: 6.e0 === 6.6';
}

//CHECK#7
if (7.7e0 !== 7.7) {
  throw '#7: 7.7e0 === 7.7';
}

//CHECK#8
if (8.8e0 !== 8.8) {
  throw '#8: 8.8e0 === 8.8';
}

//CHECK#9
if (9.9e0 !== 9.9) {
  throw '#9: 9.9e0 === 9.9';
}


//? test: S7.8.3_A3.4_T8
//? description: "ExponentPart :: E 0"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0.0E0 !== 0.0) {
  throw '#0: 0.0E0 === 0.0';
}

//CHECK#1
if (1.1E0 !== 1.1) {
  throw '#1: 1.1E0 === 1.1';
}

//CHECK#2
if (2.2E0 !== 2.2) {
  throw '#2: 2.2E0 === 2.2';
}

//CHECK#3
if (3.3E0 !== 3.3) {
  throw '#3: 3.3E0 === 3.3';
}

//CHECK#4
if (4.4E0 !== 4.4) {
  throw '#4: 4.4E0 === 4.4';
}

//CHECK#5
if (5.5E0 !== 5.5) {
  throw '#5: 5.5E0 === 5.5';
}

//CHECK#6
if (6.6E0 !== 6.6) {
  throw '#6: 6.E0 === 6.6';
}

//CHECK#7
if (7.7E0 !== 7.7) {
  throw '#7: 7.7E0 === 7.7';
}

//CHECK#8
if (8.8E0 !== 8.8) {
  throw '#8: 8.8E0 === 8.8';
}

//CHECK#9
if (9.9E0 !== 9.9) {
  throw '#9: 9.9E0 === 9.9';
}


//? test: S7.8.3_A4.1_T1
//? description: "ExponentPart :: e DecimalDigits"
//? expect: error
//? source: ...
#!/usr/bin/env afw

e1


//? test: S7.8.3_A4.1_T2
//? description: "ExponentPart :: E DecimalDigits"
//? expect: error
//? source: ...
#!/usr/bin/env afw

E1


//? test: S7.8.3_A4.1_T3
//? description: "ExponentPart :: e DecimalDigits"
//? expect: error
//? source: ...
#!/usr/bin/env afw

e-1


//? test: S7.8.3_A4.1_T4
//? description: "ExponentPart :: E DecimalDigits"
//? expect: error
//? source: ...
#!/usr/bin/env afw

E-1


//? test: S7.8.3_A4.1_T5
//? description: "ExponentPart :: e DecimalDigits"
//? expect: error
//? source: ...
#!/usr/bin/env afw

e+1


//? test: S7.8.3_A4.1_T6
//? description: "ExponentPart :: E DecimalDigits"
//? expect: error
//? source: ...
#!/usr/bin/env afw

E+1


//? test: S7.8.3_A4.1_T7
//? description: "ExponentPart :: e 0"
//? expect: error
//? source: ...
#!/usr/bin/env afw

e0


//? test: S7.8.3_A4.1_T8
//? description: "ExponentPart :: E 0"
//? expect: error
//? source: ...
#!/usr/bin/env afw

E0


//? test: S7.8.3_A4.2_T1
//? description: "ExponentIndicator :: e"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0e01 !== 0.0) {
  throw '#0: 0e01 === 0';
}

//CHECK#1
if (1e01 !== 10.0) {
  throw '#1: 1e01 === 10';
}

//CHECK#2
if (2e01 !== 20.0) {
  throw '#2: 2e01 === 20';
}

//CHECK#3
if (3e01 !== 30.0) {
  throw '#3: 3e01 === 30';
}

//CHECK#4
if (4e01 !== 40.0) {
  throw '#4: 4e01 === 40';
}

//CHECK#5
if (5e01 !== 50.0) {
  throw '#5: 5e01 === 50';
}

//CHECK#6
if (6e01 !== 60.0) {
  throw '#6: 6e01 === 60';
}

//CHECK#7
if (7e01 !== 70.0) {
  throw '#7: 7e01 === 70';
}

//CHECK#8
if (8e01 !== 80.0) {
  throw '#8: 8e01 === 80';
}

//CHECK#9
if (9e01 !== 90.0) {
  throw '#9: 9e01 === 90';
}


//? test: S7.8.3_A4.2_T2
//? description: "ExponentIndicator :: E"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0E01 !== 0.0) {
  throw '#0: 0E01 === 0';
}

//CHECK#1
if (1E01 !== 10.0) {
  throw '#1: 1E01 === 10';
}

//CHECK#2
if (2E01 !== 20.0) {
  throw '#2: 2E01 === 20';
}

//CHECK#3
if (3E01 !== 30.0) {
  throw '#3: 3E01 === 30';
}

//CHECK#4
if (4E01 !== 40.0) {
  throw '#4: 4E01 === 40';
}

//CHECK#5
if (5E01 !== 50.0) {
  throw '#5: 5E01 === 50';
}

//CHECK#6
if (6E01 !== 60.0) {
  throw '#6: 6E01 === 60';
}

//CHECK#7
if (7E01 !== 70.0) {
  throw '#7: 7E01 === 70';
}

//CHECK#8
if (8E01 !== 80.0) {
  throw '#8: 8E01 === 80';
}

//CHECK#9
if (9E01 !== 90.0) {
  throw '#9: 9E01 === 90';
}


//? test: S7.8.3_A4.2_T3
//? description: "ExponentIndicator :: e"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0e-01 !== 0.0) {
  throw '#0: 0e-01 === 0';
}

//CHECK#1
if (1e-01 !== 0.1) {
  throw '#1: 1e-01 === 0.1';
}

//CHECK#2
if (2e-01 !== 0.2) {
  throw '#2: 2e-01 === 0.2';
}

//CHECK#3
if (3e-01 !== 0.3) {
  throw '#3: 3e-01 === 0.3';
}

//CHECK#4
if (4e-01 !== 0.4) {
  throw '#4: 4e-01 === 0.4';
}

//CHECK#5
if (5e-01 !== 0.5) {
  throw '#5: 5e-01 === 0.5';
}

//CHECK#6
if (6e-01 !== 0.6) {
  throw '#6: 6e-01 === 0.6';
}

//CHECK#7
if (7e-01 !== 0.7) {
  throw '#7: 7e-01 === 0.7';
}

//CHECK#8
if (8e-01 !== 0.8) {
  throw '#8: 8e-01 === 0.8';
}

//CHECK#9
if (9e-01 !== 0.9) {
  throw '#9: 9e-01 === 0.9';
}


//? test: S7.8.3_A4.2_T4
//? description: "ExponentIndicator :: E"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0E-01 !== 0.0) {
  throw '#0: 0E-01 === 0';
}

//CHECK#1
if (1E-01 !== 0.1) {
  throw '#1: 1E-01 === 0.1';
}

//CHECK#2
if (2E-01 !== 0.2) {
  throw '#2: 2E-01 === 0.2';
}

//CHECK#3
if (3E-01 !== 0.3) {
  throw '#3: 3E-01 === 0.3';
}

//CHECK#4
if (4E-01 !== 0.4) {
  throw '#4: 4E-01 === 0.4';
}

//CHECK#5
if (5E-01 !== 0.5) {
  throw '#5: 5E-01 === 0.5';
}

//CHECK#6
if (6E-01 !== 0.6) {
  throw '#6: 6E-01 === 0.6';
}

//CHECK#7
if (7E-01 !== 0.7) {
  throw '#7: 7E-01 === 0.7';
}

//CHECK#8
if (8E-01 !== 0.8) {
  throw '#8: 8E-01 === 0.8';
}

//CHECK#9
if (9E-01 !== 0.9) {
  throw '#9: 9E-01 === 0.9';
}


//? test: S7.8.3_A4.2_T5
//? description: "ExponentIndicator :: e"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0e+01 !== 0.0) {
  throw '#0: 0e+01 === 0';
}

//CHECK#1
if (1e+01 !== 10.0) {
  throw '#1: 1e+01 === 10';
}

//CHECK#2
if (2e+01 !== 20.0) {
  throw '#2: 2e+01 === 20';
}

//CHECK#3
if (3e+01 !== 30.0) {
  throw '#3: 3e+01 === 30';
}

//CHECK#4
if (4e+01 !== 40.0) {
  throw '#4: 4e+01 === 40';
}

//CHECK#5
if (5e+01 !== 50.0) {
  throw '#5: 5e+01 === 50';
}

//CHECK#6
if (6e+01 !== 60.0) {
  throw '#6: 6e+01 === 60';
}

//CHECK#7
if (7e+01 !== 70.0) {
  throw '#7: 7e+01 === 70';
}

//CHECK#8
if (8e+01 !== 80.0) {
  throw '#8: 8e+01 === 80';
}

//CHECK#9
if (9e+01 !== 90.0) {
  throw '#9: 9e+01 === 90';
}


//? test: S7.8.3_A4.2_T6
//? description: "ExponentIndicator :: E"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0E+01 !== 0.0) {
  throw '#0: 0E+01 === 0';
}

//CHECK#1
if (1E+01 !== 10.0) {
  throw '#1: 1E+01 === 10';
}

//CHECK#2
if (2E+01 !== 20.0) {
  throw '#2: 2E+01 === 20';
}

//CHECK#3
if (3E+01 !== 30.0) {
  throw '#3: 3E+01 === 30';
}

//CHECK#4
if (4E+01 !== 40.0) {
  throw '#4: 4E+01 === 40';
}

//CHECK#5
if (5E+01 !== 50.0) {
  throw '#5: 5E+01 === 50';
}

//CHECK#6
if (6E+01 !== 60.0) {
  throw '#6: 6E+01 === 60';
}

//CHECK#7
if (7E+01 !== 70.0) {
  throw '#7: 7E+01 === 70';
}

//CHECK#8
if (8E+01 !== 80.0) {
  throw '#8: 8E+01 === 80';
}

//CHECK#9
if (9E+01 !== 90.0) {
  throw '#9: 9E+01 === 90';
}


//? test: S7.8.3_A4.2_T7
//? description: "ExponentIndicator :: e"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0e00 !== 0.0) {
  throw '#0: 0e00 === 0';
}

//CHECK#1
if (1e00 !== 1.0) {
  throw '#1: 1e00 === 1';
}

//CHECK#2
if (2e00 !== 2.0) {
  throw '#2: 2e00 === 2';
}

//CHECK#3
if (3e00 !== 3.0) {
  throw '#3: 3e00 === 3';
}

//CHECK#4
if (4e00 !== 4.0) {
  throw '#4: 4e00 === 4';
}

//CHECK#5
if (5e00 !== 5.0) {
  throw '#5: 5e00 === 5';
}

//CHECK#6
if (6e00 !== 6.0) {
  throw '#6: 6e00 === 6';
}

//CHECK#7
if (7e00 !== 7.0) {
  throw '#7: 7e00 === 7';
}

//CHECK#8
if (8e00 !== 8.0) {
  throw '#8: 8e00 === 8';
}

//CHECK#9
if (9e00 !== 9.0) {
  throw '#9: 9e00 === 9';
}


//? test: S7.8.3_A4.2_T8
//? description: "ExponentIndicator :: E"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0E00 !== 0.0) {
  throw '#0: 0E00 === 0';
}

//CHECK#1
if (1E00 !== 1.0) {
  throw '#1: 1E00 === 1';
}

//CHECK#2
if (2E00 !== 2.0) {
  throw '#2: 2E00 === 2';
}

//CHECK#3
if (3E00 !== 3.0) {
  throw '#3: 3E00 === 3';
}

//CHECK#4
if (4E00 !== 4.0) {
  throw '#4: 4E00 === 4';
}

//CHECK#5
if (5E00 !== 5.0) {
  throw '#5: 5E00 === 5';
}

//CHECK#6
if (6E00 !== 6.0) {
  throw '#6: 6E00 === 6';
}

//CHECK#7
if (7E00 !== 7.0) {
  throw '#7: 7E00 === 7';
}

//CHECK#8
if (8E00 !== 8.0) {
  throw '#8: 8E00 === 8';
}

//CHECK#9
if (9E00 !== 9.0) {
  throw '#9: 9E00 === 9';
}


//? test: S7.8.3_A5.1_T1
//? description: "HexIntegerLiteral :: 0x Digit"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0x0 !== 0) {
  throw '#0: 0x0 === 0';
}

//CHECK#1
if (0x1 !== 1) {
  throw '#1: 0x1 === 1';
}

//CHECK#2
if (0x2 !== 2) {
  throw '#2: 0x2 === 2';
}

//CHECK#3
if (0x3 !== 3) {
  throw '#3: 0x3 === 3';
}

//CHECK#4
if (0x4 !== 4) {
  throw '#4: 0x4 === 4';
}

//CHECK#5
if (0x5 !== 5) {
  throw '#5: 0x5 === 5';
}

//CHECK#6
if (0x6 !== 6) {
  throw '#6: 0x6 === 6';
}

//CHECK#7
if (0x7 !== 7) {
  throw '#7: 0x7 === 7';
}

//CHECK#8
if (0x8 !== 8) {
  throw '#8: 0x8 === 8';
}

//CHECK#9
if (0x9 !== 9) {
  throw '#9: 0x9 === 9';
}

//CHECK#A
if (0xA !== 10) {
  throw '#A: 0xA === 10';
}

//CHECK#B
if (0xB !== 11) {
  throw '#B: 0xB === 11';
}

//CHECK#C
if (0xC !== 12) {
  throw '#C: 0xC === 12';
}

//CHECK#D
if (0xD !== 13) {
  throw '#D: 0xD === 13';
}

//CHECK#E
if (0xE !== 14) {
  throw '#E: 0xE === 14';
}

//CHECK#F
if (0xF !== 15) {
  throw '#F: 0xF === 15';
}


//? test: S7.8.3_A5.1_T2
//? description: "HexIntegerLiteral :: 0X Digit"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0X0 !== 0) {
  throw '#0: 0X0 === 0';
}

//CHECK#1
if (0X1 !== 1) {
  throw '#1: 0X1 === 1';
}

//CHECK#2
if (0X2 !== 2) {
  throw '#2: 0X2 === 2';
}

//CHECK#3
if (0X3 !== 3) {
  throw '#3: 0X3 === 3';
}

//CHECK#4
if (0X4 !== 4) {
  throw '#4: 0X4 === 4';
}

//CHECK#5
if (0X5 !== 5) {
  throw '#5: 0X5 === 5';
}

//CHECK#6
if (0X6 !== 6) {
  throw '#6: 0X6 === 6';
}

//CHECK#7
if (0X7 !== 7) {
  throw '#7: 0X7 === 7';
}

//CHECK#8
if (0X8 !== 8) {
  throw '#8: 0X8 === 8';
}

//CHECK#9
if (0X9 !== 9) {
  throw '#9: 0X9 === 9';
}

//CHECK#A
if (0XA !== 10) {
  throw '#A: 0XA === 10';
}

//CHECK#B
if (0XB !== 11) {
  throw '#B: 0XB === 11';
}

//CHECK#C
if (0XC !== 12) {
  throw '#C: 0XC === 12';
}

//CHECK#D
if (0XD !== 13) {
  throw '#D: 0XD === 13';
}

//CHECK#E
if (0XE !== 14) {
  throw '#E: 0XE === 14';
}

//CHECK#F
if (0XF !== 15) {
  throw '#F: 0XF === 15';
}


//? test: S7.8.3_A5.1_T3
//? description: "HexIntegerLiteral :: 0x NonZeroDigit Digits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0x0 !== 0) {
  throw '#0: 0x0 === 0';
}

//CHECK#1
if (0x1 !== 1) {
  throw '#1: 0x1 === 1';
}

//CHECK#2
if (0x10 !== 16) {
  throw '#2: 0x10 === 16';
}

//CHECK3
if (0x100 !== 256) {
  throw '3: 0x100 === 256';
}

//CHECK#4
if (0x1000 !== 4096) {
  throw '#4: 0x1000 === 4096';
}

//CHECK#5
if (0x10000 !== 65536) {
  throw '#5: 0x10000 === 65536';
}

//CHECK#6
if (0x100000 !== 1048576) {
  throw '#6: 0x100000 === 1048576';
}

//CHECK#7
if (0x1000000 !== 16777216) {
  throw '#7: 0x1000000 === 16777216';
}

//CHECK#8
if (0x10000000 !== 268435456) {
  throw '#8: 0x10000000 === 268435456';
}


//? test: S7.8.3_A5.1_T4
//? description: "HexIntegerLiteral :: 0X NonZeroDigit Digits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0X0 !== 0) {
  throw '#0: 0X0 === 0';
}

//CHECK#1
if (0X1 !== 1) {
  throw '#1: 0X1 === 1';
}

//CHECK#2
if (0X10 !== 16) {
  throw '#2: 0X10 === 16';
}

//CHECK3
if (0X100 !== 256) {
  throw '3: 0X100 === 256';
}

//CHECK#4
if (0X1000 !== 4096) {
  throw '#4: 0X1000 === 4096';
}

//CHECK#5
if (0X10000 !== 65536) {
  throw '#5: 0X10000 === 65536';
}

//CHECK#6
if (0X100000 !== 1048576) {
  throw '#6: 0X100000 === 1048576';
}

//CHECK#7
if (0X1000000 !== 16777216) {
  throw '#7: 0X1000000 === 16777216';
}

//CHECK#8
if (0X10000000 !== 268435456) {
  throw '#8: 0X10000000 === 268435456';
}


//? test: S7.8.3_A5.1_T5
//? description: "HexIntegerLiteral :: 0x0 Digits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0x00 !== 0) {
  throw '#0: 0x00 === 0';
}

//CHECK#1
if (0x01 !== 1) {
  throw '#1: 0x01 === 1';
}

//CHECK#2
if (0x010 !== 16) {
  throw '#2: 0x010 === 16';
}

//CHECK3
if (0x0100 !== 256) {
  throw '3: 0x0100 === 256';
}

//CHECK#4
if (0x01000 !== 4096) {
  throw '#4: 0x01000 === 4096';
}

//CHECK#5
if (0x010000 !== 65536) {
  throw '#5: 0x010000 === 65536';
}

//CHECK#6
if (0x0100000 !== 1048576) {
  throw '#6: 0x0100000 === 1048576';
}

//CHECK#7
if (0x01000000 !== 16777216) {
  throw '#7: 0x01000000 === 16777216';
}

//CHECK#8
if (0x010000000 !== 268435456) {
  throw '#8: 0x010000000 === 268435456';
}


//? test: S7.8.3_A5.1_T6
//? description: "HexIntegerLiteral :: 0X0 Digits"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if (0X00 !== 0) {
  throw '#0: 0X00 === 0';
}

//CHECK#1
if (0X01 !== 1) {
  throw '#1: 0X01 === 1';
}

//CHECK#2
if (0X010 !== 16) {
  throw '#2: 0X010 === 16';
}

//CHECK3
if (0X0100 !== 256) {
  throw '3: 0X0100 === 256';
}

//CHECK#4
if (0X01000 !== 4096) {
  throw '#4: 0X01000 === 4096';
}

//CHECK#5
if (0X010000 !== 65536) {
  throw '#5: 0X010000 === 65536';
}

//CHECK#6
if (0X0100000 !== 1048576) {
  throw '#6: 0X0100000 === 1048576';
}

//CHECK#7
if (0X01000000 !== 16777216) {
  throw '#7: 0X01000000 === 16777216';
}

//CHECK#8
if (0X010000000 !== 268435456) {
  throw '#8: 0X010000000 === 268435456';
}


//? test: S7.8.3_A5.1_T7
//? description: "HexIntegerLiteral :: 0x one of a, b, c, d, e, f"
//? expect: null
//? source: ...
#!/usr/bin/env afw


//CHECK#a
if (0xa !== 10) {
  throw '#a: 0xa === 10';
}

//CHECK#b
if (0xb !== 11) {
  throw '#b: 0xb === 11';
}

//CHECK#c
if (0xc !== 12) {
  throw '#c: 0xc === 12';
}

//CHECK#d
if (0xd !== 13) {
  throw '#d: 0xd === 13';
}

//CHECK#e
if (0xe !== 14) {
  throw '#e: 0xe === 14';
}

//CHECK#f
if (0xf !== 15) {
  throw '#f: 0xf === 15';
}


//? test: S7.8.3_A5.1_T8
//? description: "HexIntegerLiteral :: 0X one of a, b, c, d, e, f"
//? expect: null
//? source: ...
#!/usr/bin/env afw

//CHECK#a
if (0Xa !== 10) {
  throw '#a: 0Xa === 10';
}

//CHECK#b
if (0Xb !== 11) {
  throw '#b: 0Xb === 11';
}

//CHECK#c
if (0Xc !== 12) {
  throw '#c: 0Xc === 12';
}

//CHECK#d
if (0Xd !== 13) {
  throw '#d: 0Xd === 13';
}

//CHECK#e
if (0Xe !== 14) {
  throw '#e: 0Xe === 14';
}

//CHECK#f
if (0Xf !== 15) {
  throw '#f: 0Xf === 15';
}


//? test: S7.8.3_A6.1_T1
//? description: Checking if execution of "0x" passes
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
0x


//? test: S7.8.3_A6.1_T2
//? description: Checking if execution of "0X" passes
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
0X


//? test: S7.8.3_A6.2_T1
//? description: Checking if execution of "0xG" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
0xG


//? test: S7.8.3_A6.2_T2
//? description: Checking if execution of "0xg" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
0xg


