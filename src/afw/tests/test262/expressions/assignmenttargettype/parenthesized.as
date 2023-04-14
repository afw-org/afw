#!/usr/bin/env -S afw --syntax test_script
//?
//? testScript: parenthesized.as
//? customPurpose: Part of test262
//? description: expressions/assignmenttargettype/parenthesized
//? sourceType: script
//?
//? test: parenthesized-additiveexpression-minus-multiplicativeexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x - y) = 1;


//? test: parenthesized-additiveexpression-minus-multiplicativeexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 - 2) = 1;


//? test: parenthesized-additiveexpression-minus-multiplicativeexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true - false) = 1;


//? test: parenthesized-additiveexpression-plus-multiplicativeexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x + y) = 1;


//? test: parenthesized-additiveexpression-plus-multiplicativeexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 + 2) = 1;


//? test: parenthesized-additiveexpression-plus-multiplicativeexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true + false) = 1;


//? test: parenthesized-arrowfunction-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(() => {}) = 1;


//? test: parenthesized-arrowfunction-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(() => ({})) = 1;


//? test: parenthesized-arrowfunction-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x => x)) = 1;


//? test: parenthesized-arrowfunction-3
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(((x) => x)) = 1;


//? test: parenthesized-arrowfunction-4
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((() => 1)) = 1;


//? test: parenthesized-arrowfunction-5
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((() => true)) = 1;


//? test: parenthesized-arrowfunction-6
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((() => 1)) = 1;


//? test: parenthesized-bitwiseandexpression-bitwise-and-equalityexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x & y) = 1;


//? test: parenthesized-bitwiseandexpression-bitwise-and-equalityexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 & 2) = 1;


//? test: parenthesized-bitwiseandexpression-bitwise-and-equalityexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true & false) = 1;


//? test: parenthesized-bitwiseorexpression-bitwise-or-bitwisexorexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x | y) = 1;


//? test: parenthesized-bitwiseorexpression-bitwise-or-bitwisexorexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 | 2) = 1;


//? test: parenthesized-bitwiseorexpression-bitwise-or-bitwisexorexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true | false) = 1;


//? test: parenthesized-bitwisexorexpression-bitwise-xor-bitwiseandexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x ^ y) = 1;


//? test: parenthesized-bitwisexorexpression-bitwise-xor-bitwiseandexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 ^ 2) = 1;


//? test: parenthesized-bitwisexorexpression-bitwise-xor-bitwiseandexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true ^ false) = 1;


//? test: parenthesized-callexpression-arguments
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(f()) = 1;


//? test: parenthesized-callexpression-templateliteral
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(f()``) = 1;


//? test: parenthesized-coalesceexpressionhead-coalesce-bitwiseorexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x ?? y) = 1;


//? test: parenthesized-coalesceexpressionhead-coalesce-bitwiseorexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 ?? 2) = 1;


//? test: parenthesized-coalesceexpressionhead-coalesce-bitwiseorexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true ?? false) = 1;


//? test: parenthesized-equalityexpression-abstract-equal-relationalexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x == y) = 1;


//? test: parenthesized-equalityexpression-abstract-equal-relationalexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 == 2) = 1;


//? test: parenthesized-equalityexpression-abstract-equal-relationalexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true == false) = 1;


//? test: parenthesized-equalityexpression-abstract-not-equal-relationalexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x != y) = 1;


//? test: parenthesized-equalityexpression-abstract-not-equal-relationalexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 != 2) = 1;


//? test: parenthesized-equalityexpression-abstract-not-equal-relationalexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true != false) = 1;


//? test: parenthesized-equalityexpression-strict-equal-relationalexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x == y) = 1;


//? test: parenthesized-equalityexpression-strict-equal-relationalexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 == 2) = 1;


//? test: parenthesized-equalityexpression-strict-equal-relationalexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true == false) = 1;


//? test: parenthesized-equalityexpression-strict-not-equal-relationalexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x !== y) = 1;


//? test: parenthesized-equalityexpression-strict-not-equal-relationalexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 !== 2) = 1;


//? test: parenthesized-equalityexpression-strict-not-equal-relationalexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true !== false) = 1;


//? test: parenthesized-expression-comma-assignmentexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x, y = z)) = 1;


//? test: parenthesized-expression-comma-assignmentexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x, y = 1)) = 1;


//? test: parenthesized-expression-comma-assignmentexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x, y = true)) = 1;


//? test: parenthesized-import.meta
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(import.meta) = 1;


//? test: parenthesized-lefthandsideexpression-assignment-assignmentexpression-0
//? description: description: If LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral, the following Early Error rule is applied, It is a Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x = y)) = 1;


//? test: parenthesized-lefthandsideexpression-assignment-assignmentexpression-1
//? description: description: If LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral, the following Early Error rule is applied, It is a Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x = 1)) = 1;


//? test: parenthesized-lefthandsideexpression-assignment-assignmentexpression-2
//? description: description: If LeftHandSideExpression is neither an ObjectLiteral nor an ArrayLiteral, the following Early Error rule is applied, It is a Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x = true)) = 1;


//? test: parenthesized-lefthandsideexpression-coalesce-assignment-assignmentexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x ??= y)) = 1;


//? test: parenthesized-lefthandsideexpression-coalesce-assignment-assignmentexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x ??= 1)) = 1;


//? test: parenthesized-lefthandsideexpression-coalesce-assignment-assignmentexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x ??= true)) = 1;


//? test: parenthesized-lefthandsideexpression-logical-and-assignment-assignmentexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x &&= y)) = 1;


//? test: parenthesized-lefthandsideexpression-logical-and-assignment-assignmentexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x &&= 1)) = 1;


//? test: parenthesized-lefthandsideexpression-logical-and-assignment-assignmentexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x &&= true)) = 1;


//? test: parenthesized-lefthandsideexpression-logical-or-assignment-assignmentexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x ||= y)) = 1;


//? test: parenthesized-lefthandsideexpression-logical-or-assignment-assignmentexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x ||= 1)) = 1;


//? test: parenthesized-lefthandsideexpression-logical-or-assignment-assignmentexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x ||= true)) = 1;


//? test: parenthesized-lefthandsideexpression-minus-minus
//? description: description: It is an early Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x--) = 1;


//? test: parenthesized-lefthandsideexpression-plus-plus
//? description: description: It is an early Syntax Error if AssignmentTargetType of LeftHandSideExpression is not simple. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x++) = 1;


//? test: parenthesized-logicalandexpression-logical-and-bitwiseorexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x && y) = 1;


//? test: parenthesized-logicalandexpression-logical-and-bitwiseorexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 && 2) = 1;


//? test: parenthesized-logicalandexpression-logical-and-bitwiseorexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true && false) = 1;


//? test: parenthesized-logicalorexpression-logical-or-logicalandexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x || y) = 1;


//? test: parenthesized-logicalorexpression-logical-or-logicalandexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 || 2) = 1;


//? test: parenthesized-logicalorexpression-logical-or-logicalandexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true || false) = 1;


//? test: parenthesized-memberexpression-templateliteral
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(o.f()``) = 1;


//? test: parenthesized-minus-minus-unaryexpression
//? description: description: It is an early Syntax Error if AssignmentTargetType of UnaryExpression is not simple. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(--x) = 1;


//? test: parenthesized-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x * y) = 1;


//? test: parenthesized-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 * 2) = 1;


//? test: parenthesized-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true * false) = 1;


//? test: parenthesized-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-3
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x / y) = 1;


//? test: parenthesized-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-4
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 / 2) = 1;


//? test: parenthesized-multiplicativeexpression-multiplicativeoperator-exponentiationexpression-5
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true / false) = 1;


//? test: parenthesized-optionalexpression
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x?.y) = 1;


//? test: parenthesized-plus-plus-unaryexpression
//? description: description: It is an early Syntax Error if AssignmentTargetType of UnaryExpression is not simple. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(++x) = 1;



//? test: parenthesized-primaryexpression-functionexpression
//? description: description: PrimaryExpression FunctionExpression, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(function() {}) = 1;


//? test: parenthesized-primaryexpression-literal-boolean
//? description: description: PrimaryExpression Literal BooleanLiteral; Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true) = 1;


//? test: parenthesized-primaryexpression-literal-null
//? description: description: PrimaryExpression Literal NullLiteral; Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(null) = 1;


//? test: parenthesized-primaryexpression-literal-numeric
//? description: description: PrimaryExpression Literal NumericLiteral; Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(0) = 1;


//? test: parenthesized-primaryexpression-literal-string
//? description: description: PrimaryExpression Literal StringLiteral; Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

('') = 1;


//? test: parenthesized-primaryexpression-objectliteral
//? description: description: PrimaryExpression ObjectLiteral; Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

({}) = 1;


//? test: parenthesized-primaryexpression-regularexpressionliteral
//? description: description: PrimaryExpression RegularExpressionLiteral; Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(/1/) = 1;


//? test: parenthesized-primaryexpression-templateliteral
//? description: description: PrimaryExpression TemplateLiteral; Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(``) = 1;


//? test: parenthesized-primaryexpression-this
//? description: description: PrimaryExpression this; Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(this) = 1;


//? test: parenthesized-relationalexpression-greater-than-or-equal-to-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x >= y) = 1;


//? test: parenthesized-relationalexpression-greater-than-or-equal-to-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 >= 2) = 1;


//? test: parenthesized-relationalexpression-greater-than-or-equal-to-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true >= false) = 1;


//? test: parenthesized-relationalexpression-greater-than-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x > y) = 1;


//? test: parenthesized-relationalexpression-greater-than-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 > 2) = 1;


//? test: parenthesized-relationalexpression-greater-than-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true > false) = 1;


//? test: parenthesized-relationalexpression-in-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x in y) = 1;


//? test: parenthesized-relationalexpression-in-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 in 2) = 1;


//? test: parenthesized-relationalexpression-in-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true in false) = 1;


//? test: parenthesized-relationalexpression-instanceof-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x instanceof y) = 1;


//? test: parenthesized-relationalexpression-instanceof-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 instanceof 2) = 1;


//? test: parenthesized-relationalexpression-instanceof-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true instanceof false) = 1;


//? test: parenthesized-relationalexpression-less-than-or-equal-to-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x <= y) = 1;


//? test: parenthesized-relationalexpression-less-than-or-equal-to-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 <= 2) = 1;


//? test: parenthesized-relationalexpression-less-than-or-equal-to-shiftexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true <= false) = 1;


//? test: parenthesized-relationalexpression-less-than-shiftexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x < y) = 1;


//? test: parenthesized-relationalexpression-less-than-shiftexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 < 2) = 1;


//? test: parenthesized-shortcircuitexpression-question-assignmentexpression-else-assignmentexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((x ? y : z)) = 1;


//? test: parenthesized-shortcircuitexpression-question-assignmentexpression-else-assignmentexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((1 ? 2 : 3)) = 1;


//? test: parenthesized-shortcircuitexpression-question-assignmentexpression-else-assignmentexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

((true ? false : true)) = 1;


//? test: parenthesized-unaryexpression-delete-unaryexpression
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(delete x.y) = 1;


//? test: parenthesized-unaryexpression-exclamation-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(!x) = 1;


//? test: parenthesized-unaryexpression-exclamation-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(!1) = 1;


//? test: parenthesized-unaryexpression-exclamation-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(!true) = 1;


//? test: parenthesized-unaryexpression-minus-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(-x) = 1;


//? test: parenthesized-unaryexpression-minus-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(-1) = 1;


//? test: parenthesized-unaryexpression-minus-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(-true) = 1;


//? test: parenthesized-unaryexpression-plus-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(+x) = 1;


//? test: parenthesized-unaryexpression-plus-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(+1) = 1;


//? test: parenthesized-unaryexpression-plus-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(+true) = 1;


//? test: parenthesized-unaryexpression-tilde-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(~x) = 1;


//? test: parenthesized-unaryexpression-tilde-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(~1) = 1;


//? test: parenthesized-unaryexpression-tilde-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(~true) = 1;


//? test: parenthesized-unaryexpression-typeof-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(typeof x) = 1;


//? test: parenthesized-unaryexpression-typeof-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(typeof 1) = 1;


//? test: parenthesized-unaryexpression-typeof-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(typeof true) = 1;


//? test: parenthesized-unaryexpression-void-unaryexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(void x) = 1;


//? test: parenthesized-unaryexpression-void-unaryexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(void 1) = 1;


//? test: parenthesized-unaryexpression-void-unaryexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(void true) = 1;


//? test: parenthesized-updateexpression-star-star-exponentiationexpression-0
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(x ** y) = 1;


//? test: parenthesized-updateexpression-star-star-exponentiationexpression-1
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(1 ** 2) = 1;


//? test: parenthesized-updateexpression-star-star-exponentiationexpression-2
//? description: description: Static Semantics AssignmentTargetType, Return invalid. (ParenthesizedExpression)
//? expect: error
//? source: ...
#!/usr/bin/env afw

(true ** false) = 1;