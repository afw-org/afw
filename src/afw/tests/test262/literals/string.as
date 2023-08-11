#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: string.as
//? customPurpose: Part of test262
//? description: literals/string
//? sourceType: script
//?
//? test: legacy-non-octal-escape-sequence-8-non-strict
//? description: LegacyOctalEscapeSequence is not available in non-strict code - 8
//? expect: error
//? source: ...
#!/usr/bin/env afw


assert('\8' ===  '8');


//? test: legacy-non-octal-escape-sequence-8-strict
//? description: LegacyOctalEscapeSequence is not enabled in strict mode code - 8
//? expect: error
//? source: ...
#!/usr/bin/env afw




'\8';


//? test: legacy-non-octal-escape-sequence-9-non-strict
//? description: LegacyOctalEscapeSequence is not available in non-strict code - 9
//? expect: error
//? source: ...
#!/usr/bin/env afw


assert('\9' ===  '9');


//? test: legacy-non-octal-escape-sequence-9-strict
//? description: LegacyOctalEscapeSequence is not enabled in strict mode code - 9
//? expect: error
//? source: ...
#!/usr/bin/env afw




'\9';


//? test: legacy-non-octal-escape-sequence-strict
//? description: LegacyOctalEscapeSequence is not available in template literals
//? expect: error
//? source: ...
#!/usr/bin/env afw




'\08';


//? test: legacy-octal-escape-sequence
//? description: String value for LegacyOctalEscapeSequence
//? expect: error
//? source: ...
#!/usr/bin/env afw


// LegacyOctalEscapeSequence ::
//   OctalDigit [lookahead ∉ OctalDigit]
assert('\0', '\x00' ===  '\\0');
assert('\1', '\x01' ===  '\\1');
assert('\2', '\x02' ===  '\\2');
assert('\3', '\x03' ===  '\\3');
assert('\4', '\x04' ===  '\\4');
assert('\5', '\x05' ===  '\\5');
assert('\6', '\x06' ===  '\\6');
assert('\7', '\x07' ===  '\\7');

assert('\08', '\x008' ===  '\\08');
assert('\18', '\x018' ===  '\\18');
assert('\28', '\x028' ===  '\\28');
assert('\38', '\x038' ===  '\\38');
assert('\48', '\x048' ===  '\\48');
assert('\58', '\x058' ===  '\\58');
assert('\68', '\x068' ===  '\\68');
assert('\78', '\x078' ===  '\\78');
assert('\08', '\x008' ===  '\\08');

// LegacyOctalEscapeSequence ::
//   ZeroToThree OctalDigit [lookahead ∉ OctalDigit]
assert('\00', '\x00' ===  '\\00');
assert('\01', '\x01' ===  '\\01');
assert('\06', '\x06' ===  '\\06');
assert('\07', '\x07' ===  '\\07');
assert('\10', '\x08' ===  '\\10');
assert('\11', '\x09' ===  '\\11');
assert('\16', '\x0e' ===  '\\16');
assert('\17', '\x0f' ===  '\\17');
assert('\20', '\x10' ===  '\\20');
assert('\21', '\x11' ===  '\\21');
assert('\26', '\x16' ===  '\\26');
assert('\27', '\x17' ===  '\\27');
assert('\30', '\x18' ===  '\\30');
assert('\31', '\x19' ===  '\\31');
assert('\36', '\x1e' ===  '\\36');
assert('\37', '\x1f' ===  '\\37');
assert('\008', '\x008' ===  '\\008');
assert('\018', '\x018' ===  '\\018');
assert('\068', '\x068' ===  '\\068');
assert('\078', '\x078' ===  '\\078');
assert('\108', '\x088' ===  '\\108');
assert('\118', '\x098' ===  '\\118');
assert('\168', '\x0e8' ===  '\\168');
assert('\178', '\x0f8' ===  '\\178');
assert('\208', '\x108' ===  '\\208');
assert('\218', '\x118' ===  '\\218');
assert('\268', '\x168' ===  '\\268');
assert('\278', '\x178' ===  '\\278');
assert('\308', '\x188' ===  '\\308');
assert('\318', '\x198' ===  '\\318');
assert('\368', '\x1e8' ===  '\\368');
assert('\378', '\x1f8' ===  '\\378');

// LegacyOctalEscapeSequence ::
//   FourToSeven OctalDigit
assert('\40', '\x20' ===  '\\40');
assert('\41', '\x21' ===  '\\41');
assert('\46', '\x26' ===  '\\46');
assert('\47', '\x27' ===  '\\47');
assert('\50', '\x28' ===  '\\50');
assert('\51', '\x29' ===  '\\51');
assert('\56', '\x2e' ===  '\\56');
assert('\57', '\x2f' ===  '\\57');
assert('\60', '\x30' ===  '\\60');
assert('\61', '\x31' ===  '\\61');
assert('\66', '\x36' ===  '\\66');
assert('\67', '\x37' ===  '\\67');
assert('\70', '\x38' ===  '\\70');
assert('\71', '\x39' ===  '\\71');
assert('\76', '\x3e' ===  '\\76');
assert('\77', '\x3f' ===  '\\77');
assert('\400', '\x200' ===  '\\400');
assert('\410', '\x210' ===  '\\410');
assert('\460', '\x260' ===  '\\460');
assert('\470', '\x270' ===  '\\470');
assert('\500', '\x280' ===  '\\500');
assert('\510', '\x290' ===  '\\510');
assert('\560', '\x2e0' ===  '\\560');
assert('\570', '\x2f0' ===  '\\570');
assert('\600', '\x300' ===  '\\600');
assert('\610', '\x310' ===  '\\610');
assert('\660', '\x360' ===  '\\660');
assert('\670', '\x370' ===  '\\670');
assert('\700', '\x380' ===  '\\700');
assert('\710', '\x390' ===  '\\710');
assert('\760', '\x3e0' ===  '\\760');
assert('\770', '\x3f0' ===  '\\770');

// LegacyOctalEscapeSequence ::
//   ZeroToThree OctalDigit OctalDigit
assert('\000', '\x00' ===  '\\000');
assert('\001', '\x01' ===  '\\001');
assert('\010', '\x08' ===  '\\010');
assert('\006', '\x06' ===  '\\006');
assert('\060', '\x30' ===  '\\060');
assert('\007', '\x07' ===  '\\007');
assert('\070', '\x38' ===  '\\070');
assert('\077', '\x3f' ===  '\\077');
assert('\100', '\x40' ===  '\\100');
assert('\101', '\x41' ===  '\\101');
assert('\110', '\x48' ===  '\\110');
assert('\106', '\x46' ===  '\\106');
assert('\160', '\x70' ===  '\\160');
assert('\107', '\x47' ===  '\\107');
assert('\170', '\x78' ===  '\\170');
assert('\177', '\x7f' ===  '\\177');
assert('\200', '\x80' ===  '\\200');
assert('\201', '\x81' ===  '\\201');
assert('\210', '\x88' ===  '\\210');
assert('\206', '\x86' ===  '\\206');
assert('\260', '\xb0' ===  '\\260');
assert('\207', '\x87' ===  '\\207');
assert('\270', '\xb8' ===  '\\270');
assert('\277', '\xbf' ===  '\\277');
assert('\300', '\xc0' ===  '\\300');
assert('\301', '\xc1' ===  '\\301');
assert('\310', '\xc8' ===  '\\310');
assert('\306', '\xc6' ===  '\\306');
assert('\360', '\xf0' ===  '\\360');
assert('\307', '\xc7' ===  '\\307');
assert('\370', '\xf8' ===  '\\370');
assert('\377', '\xff' ===  '\\377');


//? test: legacy-octal-escape-sequence-strict
//? description: LegacyOctalEscapeSequence is not available in template literals
//? expect: error
//? source: ...
#!/usr/bin/env afw


'\1';


//? test: line-continuation-double
//? description: line-continuation-double
//? expect: error:Parse error at offset 87 around line 6 column 8: Invalid escape code
//? source: ...
#!/usr/bin/env afw

// \fixme support these?

// LineTerminatorSequence :: <LF>
assert("\
" === "");

// LineTerminatorSequence :: <CR> [lookahead ≠ <LF>]
assert("\
" === "");

// LineTerminatorSequence :: <LS>
// <LS> is U+2028 LINE SEPARATOR; UTF8(0x2028) = 0xE2 0x80 0xA8
assert("\ " === "");

// LineTerminatorSequence :: <PS>
// <PS> is U+2029 PARAGRAPH SEPARATOR; UTF8(0x2029) = 0xE2 0x80 0xA9
assert("\ " === "");

// LineTerminatorSequence :: <CR> <LF>
assert("\
" === "");


//? test: line-continuation-single
//? description: line-continuation-single
//? expect: error:Parse error at offset 87 around line 6 column 8: Invalid escape code
//? source: ...
#!/usr/bin/env afw

// \fixme support these?

// LineTerminatorSequence :: <LF>
assert('\
' === '');

// LineTerminatorSequence :: <CR> [lookahead ≠ <LF>]
assert('\
' === '');

// LineTerminatorSequence :: <LS>
// <LS> is U+2028 LINE SEPARATOR; UTF8(0x2028) = 0xE2 0x80 0xA8
assert('\ ' === '');

// LineTerminatorSequence :: <PS>
// <PS> is U+2029 PARAGRAPH SEPARATOR; UTF8(0x2029) = 0xE2 0x80 0xA9
assert('\ ' === '');

// LineTerminatorSequence :: <CR> <LF>
assert('\
' === '');


//? test: line-separator-eval
//? description: line-separator-eval
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


assert(evaluate(script("'\u2028'")) ===  "\u2028");


//? test: line-separator
//? description: line-separator
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// U+2028 in strings; UTF8(0x2028) = 0xE2 0x80 0xA8
assert(" " === "\u2028");


//? test: mongolian-vowel-separator-eval
//? description: mongolian-vowel-separator-eval
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


assert(evaluate(script("'\u180E'")) ===  "\u180E");


//? test: mongolian-vowel-separator
//? description:mongolian-vowel-separator
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// U+180E in strings; UTF8(0x180E) = 0xE1 0xA0 0x8E
assert("᠎" ===  "\u180E");


//? test: paragraph-separator-eval
//? description: paragraph-separator-eval
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


assert(evaluate(script("'\u2029'")) ===  "\u2029");


//? test: paragraph-separator
//? description: paragraph-separator
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


// U+2029 in strings; UTF8(0x2029) = 0xE2 0x80 0xA9
assert(" " === "\u2029");


//? test: S7.8.4_A1.1_T1
//? description:...
    DoubleStringCharacter :: SourceCharacter but not double-quote " or
    LineTerminator
//? expect: error
//? source: ...
#!/usr/bin/env afw


//CHECK#1
"""


//? test: S7.8.4_A1.1_T2
//? description:...
    DoubleStringCharacter :: SourceCharacter but not double-quote " or
    LineTerminator
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should we indicate syntax errors on these?

//CHECK#1
"
"


//? test: S7.8.4_A1.2_T1
//? description:...
    SingleStringCharacter :: SourceCharacter but not single-quote ' or
    LineTerminator
//? expect: error
//? source: ...
#!/usr/bin/env afw


//CHECK#1
'''


//? test: S7.8.4_A1.2_T2
//? description:...
    SingleStringCharacter :: SourceCharacter but not single-quote ' or
    LineTerminator
//? expect: error
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should we indicate syntax errors on these?

//CHECK#1
'
'


//? test: S7.8.4_A2.1_T1
//? description: Check ENGLISH CAPITAL ALPHABET
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#A-Z
let unicode = ["\u0041", "\u0042", "\u0043", "\u0044", "\u0045", "\u0046", "\u0047", "\u0048", "\u0049", "\u004A", "\u004B", "\u004C", "\u004D", "\u004E", "\u004F", "\u0050", "\u0051", "\u0052", "\u0053", "\u0054", "\u0055", "\u0056", "\u0057", "\u0058", "\u0059", "\u005A"];
let character = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"];
for (let index = 0; index <= 25; index = index + 1) {
  if (unicode[index] !== character[index]) {
    throw '#' + character[index] + ' ';
  }
}


//? test: S7.8.4_A2.1_T2
//? description: Check ENGLISH SMALL ALPHABET
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#a-z
let hex = ["\u0061", "\u0062", "\u0063", "\u0064", "\u0065", "\u0066", "\u0067", "\u0068", "\u0069", "\u006A", "\u006B", "\u006C", "\u006D", "\u006E", "\u006F", "\u0070", "\u0071", "\u0072", "\u0073", "\u0074", "\u0075", "\u0076", "\u0077", "\u0078", "\u0079", "\u007A"];
let character = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"];
for (let index = 0; index <= 25; index = index + 1) {
  if (hex[index] !== character[index]) {
    throw '#' + character[index] + ' ';
  }
}


//? test: S7.8.4_A2.2_T1
//? description: Check RUSSIAN CAPITAL ALPHABET
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#А-Я
let unicode = ["\u0410", "\u0411", "\u0412", "\u0413", "\u0414", "\u0415", "\u0416", "\u0417", "\u0418", "\u0419", "\u041A", "\u041B", "\u041C", "\u041D", "\u041E", "\u041F", "\u0420", "\u0421", "\u0422", "\u0423", "\u0424", "\u0425", "\u0426", "\u0427", "\u0428", "\u0429", "\u042A", "\u042B", "\u042C", "\u042D", "\u042E", "\u042F", "\u0401"];
let character = ["А", "Б", "В", "Г", "Д", "Е", "Ж", "З", "И", "Й", "К", "Л", "М", "Н", "О", "П", "Р", "С", "Т", "У", "Ф", "Х", "Ц", "Ч", "Ш", "Щ", "Ъ", "Ы", "Ь", "Э", "Ю", "Я", "Ё"];
for (let index = 0; index <= 32; index = index + 1) {
  if (unicode[index] !== character[index]) {
    throw '#' + character[index] + ' ';
  }
}


//? test: S7.8.4_A2.2_T2
//? description: Check RUSSIAN SMALL ALPHABET
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#а-я
let unicode = ["\u0430", "\u0431", "\u0432", "\u0433", "\u0434", "\u0435", "\u0436", "\u0437", "\u0438", "\u0439", "\u043A", "\u043B", "\u043C", "\u043D", "\u043E", "\u043F", "\u0440", "\u0441", "\u0442", "\u0443", "\u0444", "\u0445", "\u0446", "\u0447", "\u0448", "\u0449", "\u044A", "\u044B", "\u044C", "\u044D", "\u044E", "\u044F", "\u0451"];
let character = ["а", "б", "в", "г", "д", "е", "ж", "з", "и", "й", "к", "л", "м", "н", "о", "п", "р", "с", "т", "у", "ф", "х", "ц", "ч", "ш", "щ", "ъ", "ы", "ь", "э", "ю", "я", "ё"];
for (let index = 0; index <= 32; index = index + 1) {
  if (unicode[index] !== character[index]) {
    throw '#' + character[index] + ' ';
  }
}


//? test: S7.8.4_A2.3_T1
//? description: Check DIGITS
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#0-9
let unicode = ["\u0030", "\u0031", "\u0032", "\u0033", "\u0034", "\u0035", "\u0036", "\u0037", "\u0038", "\u0039"];
let character = ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"];
for (let index = 0; index <= 9; index = index + 1) {
  if (unicode[index] !== character[index]) {
    throw '#' + character[index] + ' ';
  }
}


//? test: S7.8.4_A3.1_T1
//? description: Checking if execution of "\" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
"\"


//? test: S7.8.4_A3.1_T2
//? description: Checking if execution of "'\'" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
'\'


//? test: S7.8.4_A3.2_T1
//? description: Checking if execution of "\\\" fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
"\\\"


//? test: S7.8.4_A3.2_T2
//? description: Checking if execution of '\\\' fails
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
'\\\'


//? test: S7.8.4_A4.1_T1
//? description: "SingleEscapeSequence :: one of b f n r t v"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if (String.fromCharCode(0x0008) !== "\b") {
  throw '#1: String.fromCharCode(0x0008) === "\\b"';
}

//CHECK#2
if (String.fromCharCode(0x0009) !== "\t") {
  throw '#2: String.fromCharCode(0x0009) === "\\t"';
}

//CHECK#3
if (String.fromCharCode(0x000A) !== "\n") {
  throw '#3: String.fromCharCode(0x000A) === "\\n"';
}

//CHECK#4
if (String.fromCharCode(0x000B) !== "\v") {
  throw '#4: String.fromCharCode(0x000B) === "\\v"';
}

//CHECK#5
if (String.fromCharCode(0x000C) !== "\f") {
  throw '#5: String.fromCharCode(0x000C) === "\\f"';
}

//CHECK#6
if (String.fromCharCode(0x000D) !== "\r") {
  throw '#6: String.fromCharCode(0x000D) === "\\r"';
}


//? test: S7.8.4_A4.1_T2
//? description: "SingleEscapeSequence :: one of ' \" \\"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme we have no fromCharCode function

//CHECK#1
if (String.fromCharCode(0x0027) !== "\'") {
  throw '#1: String.fromCharCode(0x0027) === "\\\'"';
}

//CHECK#2
if (String.fromCharCode(0x0022) !== '\"') {
  throw '#2: String.fromCharCode(0x0027) === \'\\\"\'';
}

//CHECK#3
if (String.fromCharCode(0x005C) !== "\\") {
  throw '#3: String.fromCharCode(0x005C) === "\\\"';
}

//CHECK#4
if ("\'" !== "'") {
  throw '#4: "\'" === "\\\'"';
}

//CHECK#5
if ('\"' !== '"') {
  throw '#5: \'\"\' === \'\\\"\'';
}


//? test: S7.8.4_A4.2_T1
//? description: "NonEscapeSequence :: ENGLISH CAPITAL ALPHABET"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#A-Z
let CharacterCode = [0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A];
let NonEscapeCharacter = ["\A", "\B", "\C", "\D", "\E", "\F", "\G", "\H", "\I", "\J", "\K", "\L", "\M", "\N", "\O", "\P", "\Q", "\R", "\S", "\T", "\U", "\V", "\W", "\X", "\Y", "\Z"];
for (let index = 0; index <= 25; index = index + 1) {
  if (String.fromCharCode(CharacterCode[index]) !== NonEscapeCharacter[index]) {
    throw '#' + NonEscapeCharacter[index] + ' ';
  }
}


//? test: S7.8.4_A4.2_T2
//? description: "NonEscapeSequence :: ENGLISH CAPITAL ALPHABET"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should we recognize these escape sequences as valid?

//CHECK#A-Z
if ("A" !== "\A") {
  throw '#A';
}

if ("B" !== "\B") {
  throw '#B';
}

if ("C" !== "\C") {
  throw '#C';
}

if ("D" !== "\D") {
  throw '#D';
}

if ("E" !== "\E") {
  throw '#E';
}

if ("F" !== "\F") {
  throw '#F';
}

if ("G" !== "\G") {
  throw '#G';
}

if ("H" !== "\H") {
  throw '#H';
}

if ("I" !== "\I") {
  throw '#I';
}

if ("J" !== "\J") {
  throw '#J';
}

if ("K" !== "\K") {
  throw '#K';
}

if ("L" !== "\L") {
  throw '#L';
}

if ("M" !== "\M") {
  throw '#M';
}

if ("N" !== "\N") {
  throw '#N';
}

if ("O" !== "\O") {
  throw '#O';
}

if ("P" !== "\P") {
  throw '#P';
}

if ("Q" !== "\Q") {
  throw '#Q';
}

if ("R" !== "\R") {
  throw '#R';
}

if ("S" !== "\S") {
  throw '#S';
}

if ("T" !== "\T") {
  throw '#T';
}

if ("U" !== "\U") {
  throw '#U';
}

if ("V" !== "\V") {
  throw '#V';
}

if ("W" !== "\W") {
  throw '#W';
}

if ("X" !== "\X") {
  throw '#X';
}

if ("Y" !== "\Y") {
  throw '#Y';
}

if ("Z" !== "\Z") {
  throw '#Z';
}


//? test: S7.8.4_A4.2_T3
//? description: "NonEscapeSequence :: ENGLISH SMALL ALPHABET"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#a-z without b, f, n, r, t, v, x, u
let CharacterCode = [0x0061, 0x0063, 0x0064, 0x0065, 0x0067, 0x0068, 0x0069, 0x006A, 0x006B, 0x006C, 0x006D, 0x006F, 0x0070, 0x0071, 0x0073, 0x0077, 0x0079, 0x007A];
let NonEscapeCharacter = ["\a", "\c", "\d", "\e", "\g", "\h", "\i", "\j", "\k", "\l", "\m", "\o", "\p", "\q", "\s", "\w", "\y", "\z"];
for (let index = 0; index <= 17; index = index + 1) {
  if (String.fromCharCode(CharacterCode[index]) !== NonEscapeCharacter[index]) {
    throw '#' + NonEscapeCharacter[index] + ' ';
  }
}


//? test: S7.8.4_A4.2_T4
//? description: "NonEscapeSequence :: ENGLISH SMALL ALPHABET"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should we recognize these escape sequences as valid?

//CHECK#a-z without b, f, n, r, t, v, x, u

if ("a" !== "\a") {
  throw '#a';
}

if ("c" !== "\c") {
  throw '#c';
}

if ("d" !== "\d") {
  throw '#d';
}

if ("e" !== "\e") {
  throw '#e';
}

if ("g" !== "\g") {
  throw '#g';
}

if ("h" !== "\h") {
  throw '#h';
}

if ("i" !== "\i") {
  throw '#i';
}

if ("j" !== "\j") {
  throw '#j';
}

if ("k" !== "\k") {
  throw '#k';
}

if ("l" !== "\l") {
  throw '#l';
}

if ("m" !== "\m") {
  throw '#m';
}


if ("o" !== "\o") {
  throw '#o';
}

if ("p" !== "\p") {
  throw '#p';
}

if ("q" !== "\q") {
  throw '#q';
}

if ("s" !== "\s") {
  throw '#s';
}

if ("w" !== "\w") {
  throw '#w';
}

if ("y" !== "\y") {
  throw '#y';
}

if ("z" !== "\z") {
  throw '#z';
}


//? test: S7.8.4_A4.2_T5
//? description: "NonEscapeSequence :: RUSSIAN CAPITAL ALPHABET"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#А-Я
let CharacterCode = [0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F, 0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F, 0x0401];
let NonEscapeCharacter = ["\А", "\Б", "\В", "\Г", "\Д", "\Е", "\Ж", "\З", "\И", "\Й", "\К", "\Л", "\М", "\Н", "\О", "\П", "\Р", "\С", "\Т", "\У", "\Ф", "\Х", "\Ц", "\Ч", "\Ш", "\Щ", "\Ъ", "\Ы", "\Ь", "\Э", "\Ю", "\Я", "\Ё"];
for (let index = 0; index <= 32; index = index + 1) {
  if (String.fromCharCode(CharacterCode[index]) !== NonEscapeCharacter[index]) {
    throw '#' + NonEscapeCharacter[index] + ' ';
  }
}


//? test: S7.8.4_A4.2_T6
//? description: "NonEscapeSequence :: RUSSIAN CAPITAL ALPHABET"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should we recognize these escape sequences as valid?

//CHECK#А-Я

if ("А" !== "\А") {
  throw '#А';
}

if ("Б" !== "\Б") {
  throw '#Б';
}

if ("В" !== "\В") {
  throw '#В';
}

if ("Г" !== "\Г") {
  throw '#Г';
}

if ("Д" !== "\Д") {
  throw '#Д';
}

if ("Е" !== "\Е") {
  throw '#Е';
}

if ("Ж" !== "\Ж") {
  throw '#Ж';
}

if ("З" !== "\З") {
  throw '#З';
}

if ("И" !== "\И") {
  throw '#И';
}

if ("Й" !== "\Й") {
  throw '#Й';
}

if ("К" !== "\К") {
  throw '#К';
}

if ("Л" !== "\Л") {
  throw '#Л';
}

if ("М" !== "\М") {
  throw '#М';
}

if ("Н" !== "\Н") {
  throw '#Н';
}

if ("О" !== "\О") {
  throw '#О';
}

if ("П" !== "\П") {
  throw '#П';
}

if ("Р" !== "\Р") {
  throw '#Р';
}

if ("С" !== "\С") {
  throw '#С';
}

if ("Т" !== "\Т") {
  throw '#Т';
}

if ("У" !== "\У") {
  throw '#У';
}

if ("Ф" !== "\Ф") {
  throw '#Ф';
}

if ("Х" !== "\Х") {
  throw '#Х';
}

if ("Ц" !== "\Ц") {
  throw '#Ц';
}

if ("Ч" !== "\Ч") {
  throw '#Ч';
}

if ("Ш" !== "\Ш") {
  throw '#Ш';
}

if ("Щ" !== "\Щ") {
  throw '#Щ';
}

if ("Ъ" !== "\Ъ") {
  throw '#Ъ';
}

if ("Ы" !== "\Ы") {
  throw '#Ы';
}

if ("Ь" !== "\Ь") {
  throw '#Ь';
}

if ("Э" !== "\Э") {
  throw '#Э';
}

if ("Ю" !== "\Ю") {
  throw '#Ю';
}

if ("Я" !== "\Я") {
  throw '#Я';
}

if ("Ё" !== "\Ё") {
  throw '#Ё';
}


//? test: S7.8.4_A4.2_T7
//? description: "NonEscapeSequence :: RUSSIAN SMALL ALPHABET"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#а-я
let CharacterCode = [0x0430, 0x0431, 0x0432, 0x0433, 0x0434, 0x0435, 0x0436, 0x0437, 0x0438, 0x0439, 0x043A, 0x043B, 0x043C, 0x043D, 0x043E, 0x043F, 0x0440, 0x0441, 0x0442, 0x0443, 0x0444, 0x0445, 0x0446, 0x0447, 0x0448, 0x0449, 0x044A, 0x044B, 0x044C, 0x044D, 0x044E, 0x044F, 0x0451];
let NonEscapeCharacter = ["\а", "\б", "\в", "\г", "\д", "\е", "\ж", "\з", "\и", "\й", "\к", "\л", "\м", "\н", "\о", "\п", "\р", "\с", "\т", "\у", "\ф", "\х", "\ц", "\ч", "\ш", "\щ", "\ъ", "\ы", "\ь", "\э", "\ю", "\я", "\ё"];
for (let index = 0; index <= 32; index = index + 1) {
  if (String.fromCharCode(CharacterCode[index]) !== NonEscapeCharacter[index]) {
    throw '#' + NonEscapeCharacter[index] + ' ';
  }
}


//? test: S7.8.4_A4.2_T8
//? description: "NonEscapeSequence :: RUSSIAN SMALL ALPHABET"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should we recognize these escape sequences as valid?


//CHECK#а-я

if ("а" !== "\а") {
  throw '#а';
}

if ("б" !== "\б") {
  throw '#б';
}

if ("в" !== "\в") {
  throw '#в';
}

if ("г" !== "\г") {
  throw '#г';
}

if ("д" !== "\д") {
  throw '#д';
}

if ("е" !== "\е") {
  throw '#е';
}

if ("ж" !== "\ж") {
  throw '#ж';
}

if ("з" !== "\з") {
  throw '#з';
}

if ("и" !== "\и") {
  throw '#и';
}

if ("й" !== "\й") {
  throw '#й';
}

if ("к" !== "\к") {
  throw '#к';
}

if ("л" !== "\л") {
  throw '#л';
}

if ("м" !== "\м") {
  throw '#м';
}

if ("н" !== "\н") {
  throw '#н';
}

if ("о" !== "\о") {
  throw '#о';
}

if ("п" !== "\п") {
  throw '#п';
}

if ("р" !== "\р") {
  throw '#р';
}

if ("с" !== "\с") {
  throw '#с';
}

if ("т" !== "\т") {
  throw '#т';
}

if ("у" !== "\у") {
  throw '#у';
}

if ("ф" !== "\ф") {
  throw '#ф';
}

if ("х" !== "\х") {
  throw '#х';
}

if ("ц" !== "\ц") {
  throw '#ц';
}

if ("ч" !== "\ч") {
  throw '#ч';
}

if ("ш" !== "\ш") {
  throw '#ш';
}

if ("щ" !== "\щ") {
  throw '#щ';
}

if ("ъ" !== "\ъ") {
  throw '#ъ';
}

if ("ы" !== "\ы") {
  throw '#ы';
}

if ("ь" !== "\ь") {
  throw '#ь';
}

if ("э" !== "\э") {
  throw '#э';
}

if ("ю" !== "\ю") {
  throw '#ю';
}

if ("я" !== "\я") {
  throw '#я';
}

if ("ё" !== "\ё") {
  throw '#ё';
}


//? test: S7.8.4_A4.3_T1
//? description: "EscapeCharacter :: DecimalDigits :: 1"
//? expect: error
//? source: ...
#!/usr/bin/env afw




"\1"


//? test: S7.8.4_A4.3_T2
//? description: "EscapeCharacter :: DecimalDigits :: 7"
//? expect: error
//? source: ...
#!/usr/bin/env afw




"\7"


//? test: S7.8.4_A4.3_T7
//? description: "EscapeCharacter :: SingleEscapeCharacter :: one of b f n r t v"
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#bfnrtv
if ("b" === "\b") {
  throw '#b';
}

if ("f" === "\f") {
  throw '#f';
}

if ("n" === "\n") {
  throw '#n';
}

if ("r" === "\r") {
  throw '#r';
}

if ("t" === "\t") {
  throw '#t';
}

if ("v" === "\v") {
  throw '#v';
}


//? test: S7.8.4_A5.1_T1
//? description: String.fromCharCode(0x0000)
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme we have no fromCharCode function

//CHECK#1
if (String.fromCharCode(0x0000) !== "\0") {
  throw '#1: String.fromCharCode(0x0000) === "\\0"';
}


//? test: S7.8.4_A5.1_T2
//? description: "\"\\u0000\""
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should we recognize these escape sequences as valid?

//CHECK#1
if ("\u0000" !== "\0") {
  throw '#1: "\\u0000" === "\\0"';
}


//? test: S7.8.4_A5.1_T3
//? description: "\"\\x00\""
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should we recognize these escape sequences as valid?

//CHECK#1
if ("\x00" !== "\0") {
  throw '#1: "\\x00" === "\\0"';
}


//? test: S7.8.4_A6.1_T1
//? description: "HexEscapeSequence ::  HexDigit"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if ("\x00" !== String.fromCharCode("0")) {
  throw '#0: "\\x00" === String.fromCharCode("0")';
}

//CHECK#1
if ("\x01" !== String.fromCharCode("1")) {
  throw '#1: "\\x01" === String.fromCharCode("1")';
}

//CHECK#2
if ("\x02" !== String.fromCharCode("2")) {
  throw '#2: "\\x02" === String.fromCharCode("2")';
}

//CHECK#3
if ("\x03" !== String.fromCharCode("3")) {
  throw '#3: "\\x03" === String.fromCharCode("3")';
}

//CHECK#4
if ("\x04" !== String.fromCharCode("4")) {
  throw '#4: "\\x04" === String.fromCharCode("4")';
}

//CHECK#5
if ("\x05" !== String.fromCharCode("5")) {
  throw '#5: "\\x05" === String.fromCharCode("5")';
}

//CHECK#6
if ("\x06" !== String.fromCharCode("6")) {
  throw '#6: "\\x06" === String.fromCharCode("6")';
}

//CHECK#7
if ("\x07" !== String.fromCharCode("7")) {
  throw '#7: "\\x07" === String.fromCharCode("7")';
}

//CHECK#8
if ("\x08" !== String.fromCharCode("8")) {
  throw '#8: "\\x08" === String.fromCharCode("8")';
}

//CHECK#9
if ("\x09" !== String.fromCharCode("9")) {
  throw '#9: "\\x09" === String.fromCharCode("9")';
}

//CHECK#A
if ("\x0A" !== String.fromCharCode("10")) {
  throw '#A: "\\x0A" === String.fromCharCode("10")';
}

//CHECK#B
if ("\x0B" !== String.fromCharCode("11")) {
  throw '#B: "\\x0B" === String.fromCharCode("11")';
}

//CHECK#C
if ("\x0C" !== String.fromCharCode("12")) {
  throw '#C: "\\x0C" === String.fromCharCode("12")';
}

//CHECK#D
if ("\x0D" !== String.fromCharCode("13")) {
  throw '#D: "\\x0D" === String.fromCharCode("13")';
}

//CHECK#E
if ("\x0E" !== String.fromCharCode("14")) {
  throw '#E: "\\x0E" === String.fromCharCode("14")';
}

//CHECK#F
if ("\x0F" !== String.fromCharCode("15")) {
  throw '#F: "\\x0F" === String.fromCharCode("15")';
}


//? test: S7.8.4_A6.1_T2
//? description: "HexEscapeSequence ::  ENGLISH CAPITAL ALPHABET"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should we recognize these escape sequences as valid?

//CHECK#A-Z
let hex = ["\x41", "\x42", "\x43", "\x44", "\x45", "\x46", "\x47", "\x48", "\x49", "\x4A", "\x4B", "\x4C", "\x4D", "\x4E", "\x4F", "\x50", "\x51", "\x52", "\x53", "\x54", "\x55", "\x56", "\x57", "\x58", "\x59", "\x5A"];
let character = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"];
for (let index = 0; index <= 25; index = index + 1) {
  if (hex[index] !== character[index]) {
    throw '#' + character[index] + ' ';
  }
}


//? test: S7.8.4_A6.1_T3
//? description: "HexEscapeSequence ::  ENGLISH SMALL ALPHABET"
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw

// \fixme should we recognize these escape sequences as valid?

//CHECK#a-z
let hex = ["\x61", "\x62", "\x63", "\x64", "\x65", "\x66", "\x67", "\x68", "\x69", "\x6A", "\x6B", "\x6C", "\x6D", "\x6E", "\x6F", "\x70", "\x71", "\x72", "\x73", "\x74", "\x75", "\x76", "\x77", "\x78", "\x79", "\x7A"];
let character = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"];
for (let index = 0; index <= 25; index = index + 1) {
  if (hex[index] !== character[index]) {
    throw '#' + character[index] + ' ';
  }
}


//? test: S7.8.4_A6.3_T1
//? description: Check similar to ('\x01F' === String.fromCharCode('1') + 'F')
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ('\x01F' !== String.fromCharCode('1') + 'F') {
  throw "#1: '\x01F' === String.fromCharCode('1') + 'F'";
}

//CHECK#2
if ('\x02E' !== String.fromCharCode('2') + 'E') {
  throw "#2: '\x02E' === String.fromCharCode('2') + 'E'";
}

//CHECK#3
if ('\x03D' !== String.fromCharCode('3') + 'D') {
  throw "#3: '\x03D' === String.fromCharCode('3') + 'D'";
}

//CHECK#4
if ('\x04C' !== String.fromCharCode('4') + 'C') {
  throw "#4: '\x04C' === String.fromCharCode('4') + 'C'";
}

//CHECK#5
if ('\x05B' !== String.fromCharCode('5') + 'B') {
  throw "#5: '\x05B' === String.fromCharCode('5') + 'B'";
}

//CHECK#6
if ('\x06A' !== String.fromCharCode('6') + 'A') {
  throw "#6: '\x06A' === String.fromCharCode('6') + 'A'";
}

//CHECK#7
if ('\x079' !== String.fromCharCode('7') + '9') {
  throw "#7: '\x079' === String.fromCharCode('7') + '9'";
}

//CHECK#8
if ('\x088' !== String.fromCharCode('8') + '8') {
  throw "#8: '\x088' === String.fromCharCode('8') + '8'";
}

//CHECK#9
if ('\x097' !== String.fromCharCode('9') + '7') {
  throw "#9: '\x097' === String.fromCharCode('9') + '7'";
}

//CHECK#A
if ('\x0A6' !== String.fromCharCode('10') + '6') {
  throw "#A: '\x0A6' === String.fromCharCode('10') + '6'";
}

//CHECK#B
if ('\x0B5' !== String.fromCharCode('11') + '5') {
  throw "#B: '\x0B5' === String.fromCharCode('11') + '5'";
}

//CHECK#C
if ('\x0C4' !== String.fromCharCode('12') + '4') {
  throw "#C: '\x0C4' === String.fromCharCode('12') + '4'";
}

//CHECK#D
if ('\x0D3' !== String.fromCharCode('13') + '3') {
  throw "#D: '\x0D3' === String.fromCharCode('13') + '3'";
}

//CHECK#E
if ('\x0E2' !== String.fromCharCode('14') + '2') {
  throw "#E: '\x0E2' === String.fromCharCode('14') + '2'";
}

//CHECK#F
if ('\x0F1' !== String.fromCharCode('15') + '1') {
  throw "#F: '\x0F1' === String.fromCharCode('15') + '1'";
}


//? test: S7.8.4_A7.1_T1
//? description: Check similar to ("\u0000" === String.fromCharCode("0"))
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#0
if ("\u0000" !== String.fromCharCode("0")) {
  throw '#0: "\\u0000" === String.fromCharCode("0")';
}

//CHECK#1
if ("\u0001" !== String.fromCharCode("1")) {
  throw '#1: "\\u0001" === String.fromCharCode("1")';
}

//CHECK#2
if ("\u0002" !== String.fromCharCode("2")) {
  throw '#2: "\\u0002" === String.fromCharCode("2")';
}

//CHECK#3
if ("\u0003" !== String.fromCharCode("3")) {
  throw '#3: "\\u0003" === String.fromCharCode("3")';
}

//CHECK#4
if ("\u0004" !== String.fromCharCode("4")) {
  throw '#4: "\\u0004" === String.fromCharCode("4")';
}

//CHECK#5
if ("\u0005" !== String.fromCharCode("5")) {
  throw '#5: "\\u0005" === String.fromCharCode("5")';
}

//CHECK#6
if ("\u0006" !== String.fromCharCode("6")) {
  throw '#6: "\\u0006" === String.fromCharCode("6")';
}

//CHECK#7
if ("\u0007" !== String.fromCharCode("7")) {
  throw '#7: "\\u0007" === String.fromCharCode("7")';
}

//CHECK#8
if ("\u0008" !== String.fromCharCode("8")) {
  throw '#8: "\\u0008" === String.fromCharCode("8")';
}

//CHECK#9
if ("\u0009" !== String.fromCharCode("9")) {
  throw '#9: "\\u0009" === String.fromCharCode("9")';
}

//CHECK#A
if ("\u000A" !== String.fromCharCode("10")) {
  throw '#A: "\\u000A" === String.fromCharCode("10")';
}

//CHECK#B
if ("\u000B" !== String.fromCharCode("11")) {
  throw '#B: "\\u000B" === String.fromCharCode("11")';
}

//CHECK#C
if ("\u000C" !== String.fromCharCode("12")) {
  throw '#C: "\\u000C" === String.fromCharCode("12")';
}

//CHECK#D
if ("\u000D" !== String.fromCharCode("13")) {
  throw '#D: "\\u000D" === String.fromCharCode("13")';
}

//CHECK#E
if ("\u000E" !== String.fromCharCode("14")) {
  throw '#E: "\\u000E" === String.fromCharCode("14")';
}

//CHECK#F
if ("\u000F" !== String.fromCharCode("15")) {
  throw '#F: "\\u000F" === String.fromCharCode("15")';
}


//? test: S7.8.4_A7.1_T2
//? description: "UnicodeEscapeSequence ::  ENGLISH CAPITAL ALPHABET"
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#A-Z
let unicode = ["\u0041", "\u0042", "\u0043", "\u0044", "\u0045", "\u0046", "\u0047", "\u0048", "\u0049", "\u004A", "\u004B", "\u004C", "\u004D", "\u004E", "\u004F", "\u0050", "\u0051", "\u0052", "\u0053", "\u0054", "\u0055", "\u0056", "\u0057", "\u0058", "\u0059", "\u005A"];
let character = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"];
for (let index = 0; index <= 25; index = index + 1) {
  if (unicode[index] !== character[index]) {
    throw '#' + character[index] + ' ';
  }
}


//? test: S7.8.4_A7.1_T3
//? description: "UnicodeEscapeSequence ::  ENGLISH SMALL ALPHABET"
//? expect: undefined
//? source: ...
#!/usr/bin/env afw


//CHECK#a-z
let unicode = ["\u0061", "\u0062", "\u0063", "\u0064", "\u0065", "\u0066", "\u0067", "\u0068", "\u0069", "\u006A", "\u006B", "\u006C", "\u006D", "\u006E", "\u006F", "\u0070", "\u0071", "\u0072", "\u0073", "\u0074", "\u0075", "\u0076", "\u0077", "\u0078", "\u0079", "\u007A"];
let character = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"];
for (let index = 0; index <= 25; index = index + 1) {
  if (unicode[index] !== character[index]) {
    throw '#' + character[index] + ' ';
  }
}


//? test: S7.8.4_A7.1_T4
//? description: "UnicodeEscapeSequence :: u000G is incorrect"
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#
"\u000G"


//? test: S7.8.4_A7.2_T1
//? description: ":: HexDigit :: 1"
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
"\u1"


//? test: S7.8.4_A7.2_T2
//? description: ":: HexDigit :: A"
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
"\uA"


//? test: S7.8.4_A7.2_T3
//? description: ":: HexDigit :: 1"
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
"\u11"


//? test: S7.8.4_A7.2_T4
//? description: ":: HexDigit :: A"
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
"\uAA"


//? test: S7.8.4_A7.2_T5
//? description: ":: HexDigit :: 1"
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
"\u111"


//? test: S7.8.4_A7.2_T6
//? description: ":: HexDigit :: A"
//? expect: error
//? source: ...
#!/usr/bin/env afw




//CHECK#1
"\uAAA"


//? test: S7.8.4_A7.3_T1
//? description: Check similar to ("\u0001F" === String.fromCharCode("1") + "F")
//? expect: undefined
//? skip: true
//? source: ...
#!/usr/bin/env afw


//CHECK#1
if ("\u0001F" !== String.fromCharCode("1") + "F") {
  throw '#1: "\\u0001F" === String.fromCharCode("1") + "F"';
}

//CHECK#2
if ("\u0002E" !== String.fromCharCode("2") + "E") {
  throw '#2: "\\u0002E" === String.fromCharCode("2") + "E"';
}

//CHECK#3
if ("\u0003D" !== String.fromCharCode("3") + "D") {
  throw '#3: "\\u0003D" === String.fromCharCode("3") + "D"';
}

//CHECK#4
if ("\u0004C" !== String.fromCharCode("4") + "C") {
  throw '#4: "\\u0004C" === String.fromCharCode("4") + "C"';
}

//CHECK#5
if ("\u0005B" !== String.fromCharCode("5") + "B") {
  throw '#5: "\\u0005B" === String.fromCharCode("5") + "B"';
}

//CHECK#6
if ("\u0006A" !== String.fromCharCode("6") + "A") {
  throw '#6: "\\u0006A" === String.fromCharCode("6") + "A"';
}

//CHECK#7
if ("\u00079" !== String.fromCharCode("7") + "9") {
  throw '#7: "\\u00079" === String.fromCharCode("7") + "9"';
}

//CHECK#8
if ("\u00088" !== String.fromCharCode("8") + "8") {
  throw '#8: "\\u00088" === String.fromCharCode("8") + "8"';
}

//CHECK#9
if ("\u00097" !== String.fromCharCode("9") + "7") {
  throw '#9: "\\u00097" === String.fromCharCode("9") + "7"';
}

//CHECK#A
if ("\u000A6" !== String.fromCharCode("10") + "6") {
  throw '#A: "\\u000A6" === String.fromCharCode("10") + "6"';
}

//CHECK#B
if ("\u000B5" !== String.fromCharCode("11") + "5") {
  throw '#B: "\\u000B5" === String.fromCharCode("11") + "5"';
}

//CHECK#C
if ("\u000C4" !== String.fromCharCode("12") + "4") {
  throw '#C: "\\u000C4" === String.fromCharCode("12") + "4"';
}

//CHECK#D
if ("\u000D3" !== String.fromCharCode("13") + "3") {
  throw '#D: "\\u000D3" === String.fromCharCode("13") + "3"';
}

//CHECK#E
if ("\u000E2" !== String.fromCharCode("14") + "2") {
  throw '#E: "\\u000E2" === String.fromCharCode("14") + "2"';
}

//CHECK#F
if ("\u000F1" !== String.fromCharCode("15") + "1") {
  throw '#F: "\\u000F1" === String.fromCharCode("15") + "1"';
}


//? test: unicode-escape-nls-err-double
//? description: NumericLiteralSeperator disallowed in unicode CodePoint sequence (DoubleStringCharacters)
//? expect: error
//? source: ...
#!/usr/bin/env afw


"\u{1F_639}"


//? test: unicode-escape-nls-err-single
//? description: NumericLiteralSeperator disallowed in unicode CodePoint sequence (DoubleStringCharacters)
//? expect: error
//? source: ...
#!/usr/bin/env afw


'\u{1F_639}';


//? test: unicode-escape-no-hex-err-double
//? description: unicode-escape-no-hex-err-double
//? expect: error
//? source: ...
#!/usr/bin/env afw


"\u"


//? test: unicode-escape-no-hex-err-single
//? description: unicode-escape-no-hex-err-single
//? expect: error
//? source: ...
#!/usr/bin/env afw


'\u'


