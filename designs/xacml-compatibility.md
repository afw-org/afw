# XACML / XML Schema compatibility notes

**Audience:** maintainers working on a policy (XACML) handler or anything that must agree with XACML 3.0 or XML Schema (XSD) data types. Not user docs; `whats-new.md` and the handbook describe AFW behavior without comparing it to XACML.

AFW's data types and many built-ins came from XACML 3.0 core (Appendix A) and XSD 1.1 Part 2. Since #580 (strict types) AFW follows its own rules: strict like XACML in what it compares, ECMAScript-like in names and argument order where a function has an ECMAScript name (#599). This pad lists where AFW now **differs** from the specs, so a policy handler can bridge the gap (a translation layer, a XACML-mode function, or a decision to accept the difference). It covers differences that exist today, not plans.

When a change in `src/afw` makes AFW differ from XACML or XSD, add a row here in the same PR.

## Reading text (lexical space)

Two rules, kept apart on purpose (#543):

1. **A value never changes data type on its own.** An integer given to a double parameter, typed slot, operator, or compare is an error (#583); `1 == 1.0` is an error. Nothing here loosens that.
2. **Text is read by the data type's XSD lexical form**, and only where text is being read: `to_T(string)`, a typed literal (`"…"dateTime`; numbers have none), or a parser reading text declared as `T`. The caller named `T`, so text in `T`'s lexical space is `T`. `to_double("5")` is `5.0` because `"5"` is `xs:double` text; no integer value exists.

Do not read rule 2 as permission to coerce values elsewhere. Whitespace collapse is a facet, not the lexical form, and stays out (row below).

| Data type | XSD / XACML | AFW | Since |
|---|---|---|---|
| boolean | XSD lexical `true`, `false`, `1`, `0` (lower case only) | `string_to_boolean` also reads any case and `t`/`T`/`f`/`F` | #588 |
| double, integer | XSD lexical: double includes integer text (`"5"`), `.5`, `5.`, leading zeros; integer allows leading zeros (`"007"`) | Same, since #543 (#588 had rejected integer text for double) | #543 |
| all with whiteSpace `collapse` (boolean, numbers, dates, durations, anyURI, …) | Leading/trailing spaces are removed before reading | No whitespace processing: `to_integer(" 12")` is an error | #588 |
| anyURI | Any text (XSD 1.1 3.3.17) | Same | #593 |
| rfc822Name | No read check required (XACML A.2) | Requires local part, `@`, domain part | #593 |
| x500Name | No read check required | Must parse as an RFC 4514 / 2253 DN | #593 |
| ipAddress, dnsName | XACML A.2 gives a syntax but no read check | Same: any text | #593 |
| dateTime, time (fractional seconds) | XSD: any number of digits (a processor may limit precision, at least milliseconds) | Microseconds; more digits only if they are `0`, otherwise an error (never rounded) | #593 |
| date, dateTime, time, durations | XSD 1.1 lexical forms | Same, since #593: `hh:mm:ss` required, `±hh:mm` zones, 4+ digit years, `24:00:00`, duration leading zeros and `1.` / `.5` seconds | #593 |

## Equality

| Data type | XACML | AFW | Since |
|---|---|---|---|
| rfc822Name | `rfc822Name-equal`: domain part normalized to lower case | Same; lower case is ICU `u_tolower` per code point, not ASCII only | #593 |
| x500Name | `x500Name-equal`: RFC 2253 normalize, sorted multi-valued RDNs, RFC 3280 compare rules | Types ignore case with a small name↔OID table (cn sn c l st street o ou title uid dc); values lower case + space collapse (RFC 4518 caseIgnoreMatch **without** full stringprep or Unicode normalization); `#` hex values compare as octets and are **not** BER-decoded, so `cn=#0c03616263` ≠ `cn=abc` | #593 |
| ipAddress, dnsName | **No** equality function in XACML 3.0 A.3.1 | Text ignoring case (DNS names, IPv6 hex digits); `::1` and `0:0:0:0:0:0:0:1` are not equal | #593 |
| date, dateTime, time without a time zone | Implicit time zone (XACML A.3 / XPath) | Same since #593: the local time zone when the xctx was created; dates compare by the instant they start | #593 |
| integer vs double | Distinct types; mixed calls are a type error | Same: `1 == 1.0` is an error (*Data types do not match*) | #580 |

## Ordering

XACML defines `-greater-than` / `-less-than` only for integer, double, string, time, date, dateTime (A.3.6–A.3.9). XSD marks anyURI, boolean, the binaries, and QName as unordered, and durations as partially ordered.

| Data type | XACML / XSD | AFW | Since |
|---|---|---|---|
| dnsName, ipAddress, rfc822Name, x500Name | No ordering | No ordering (`<` throws) | #593 |
| anyURI, base64Binary, hexBinary, boolean, ia5String, durations | No ordering in XACML | Ordered (`lt<T>` exists): text, octets, `false < true`, durations by value | — |
| array, object, function | n/a (XACML bags have no order) | `lt<array>` element by element; `lt<object>` and `lt<function>` exist but are not an order (object compare returns -1 whenever objects differ; function compares addresses). Metadata `relationalCompares` says false for these and for boolean; see the #593 scope comment | — |

## Functions

| XACML | AFW | Since |
|---|---|---|
| `type-bag`, `type-bag-size` | `array<T>(…)`, `length(a)` | #583 |
| `type-one-and-only(bag)` | `one_and_only<T>(array)`; a single value is an error, not a bag of one | #583 |
| A single value where a bag is expected is a bag of one (in some implementations) | Error | #583 |
| `type-from-string`, `string-from-type` | `to_T(s)`, `to_string(v)`; generic `T(x)` conversions are gone | #588, #589 |
| `rfc822Name-match(string, rfc822Name)` | `rfc822Name_match(pattern, value)`, same order and cases (A.3.14) | #593 |
| `x500Name-match(x500Name, x500Name)` | `x500Name_match(pattern, value)`, same (terminal RDNs) | #593 |
| Functions that share a name with ECMAScript | ECMAScript argument order and behavior; functions with XACML-only names keep XACML order | #599 |
| `type-regexp-match(regexp, value)` (A.3.13: XPath `fn:matches`, arguments reversed) | `test(regexp, value)`: the same order (the regexp is ECMAScript's receiver, #617; it was `regexp_match<T>(value, regexp)`), one function for every text type, and the pattern is a `regexp` value, not a string (`to_regexp(s)` converts a pattern held as a string). Matches anywhere unless `^`/`$` anchor it, as `fn:matches` does; the libxml2 XML Schema regexes it used before always matched the whole value | #585 |
| Regular expression syntax and meaning: XPath F&O 7.6.1 (XML Schema regex plus `^`, `$`, lazy quantifiers, back-references; flags `s`, `m`, `i`, `x`) | ICU, which is neither. Same: search semantics, `^` / `$` anchors, the four flags. Different: `$` without `m` also matches before a final line terminator (XPath: end of string only; write `\z`); `\w` and `\s` have other definitions; `.` and `m` use all ICU line terminators (XPath: LF, CR); `i` is full case folding; `\i` / `\c` and `[a-z-[aeiou]]` subtraction are not ICU syntax (ICU: `[a-z--[aeiou]]`); ICU also accepts lookaround, named groups, `\b`. Public list: handbook *Lexical*, *Regular Expressions* (names XPath `fn:matches`, not XACML) | #585 |
| `ipAddress` / `dnsName` matching or parts | Not provided (XACML 3.0 core has none beyond conversion either) | — |

Spec texts: [XACML 3.0 core](https://docs.oasis-open.org/xacml/3.0/xacml-3.0-core-spec-os-en.html) Appendix A; [XSD 1.1 Part 2](https://www.w3.org/TR/xmlschema11-2/).

## Regular expressions in a policy handler (#585, #617)

A handler that maps XACML `*-regexp-match` onto AFW should:

- map every `type-regexp-match` to `test(regexp, value)`, the same argument order (#617);
- convert each pattern once when the policy loads (`to_regexp`, or `compile<regexp>` of a `regexp`), so a bad pattern is found at load and each decision matches with the compiled form;
- make a regexp error Indeterminate, never Permit or Deny: `conversion_error` (bad pattern), `payload_too_large` (the match limit, about 0.1 s of backtracking). The ICU 74.2 bug where a lazy loop over a group that can match empty (`(b|)*?^(?=a)`) never ends also lands here, through the limit;
- expect the ICU differences in the table above. Exact XPath semantics would need XPath-to-ICU translation in XACML-only functions; not done.
