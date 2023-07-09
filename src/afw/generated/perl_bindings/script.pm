package afw::script;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    assign 
    bag_script 
    bag_size_script 
    break 
    compile_script 
    const 
    continue 
    do_while 
    eq_script 
    eqx_script 
    evaluate_script 
    for 
    foreach 
    ge_script 
    gt_script 
    if 
    is_script 
    le_script 
    loc 
    lt_script 
    ne_script 
    nex_script 
    rethrow 
    return 
    script 
    throw 
    try 
    while 
);

=head1 NAME

afw::script - Perl module for script functions

=head2 Functions

The following functions are exported by default

=head3 assign

Assign a value to the innermost structured block definition of a variable. If
the variable is not defined, the variable is defined in the innermost
structured block. An error is thrown if not called from a list of values
(statements) in a structured function.
Assign a value to a defined variable

=head4 Parameters

    $name

Variable name

    $value

This is the value to assign to the variable.

=head3 bag_script

Takes any number of script values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 bag_size_script

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 break

This is a special function that can be called to break out of the body of a
loop. If called outside of a loop body, an error is thrown.
Break out of a block

=head4 Parameters

    $value

The value to evaluate that the enclosing loop will return. If not specified,
the last evaluated value or a null value will be returned.

=head3 compile_script

Compile script value and return either an unevaluated adaptive value or a
string containing the compiler listing.
Compile script value

=head4 Parameters

    $source

script string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
expression value.

This parameter can be an integer between 0 and 10 of a string that is used
for indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 const

Define one or more statically scoped constants local to the current script
block with a permanent value. These constants can be accessed from the
current block and inner blocks, but can not be assigned a different value.
Define one or more constants in the current block

=head4 Parameters

    $name

The name of one or more constants to defined in the current block.

    $value

This is the value of the constant(s).

    $type

The type of the constant(s).

=head3 continue

This is a special function that can be called in the body of a loop function
to test the condition and, if true, start evaluating the body again. If
called outside of a loop body, an error is thrown.
Continue at beginning of a loop

=head4 Parameters

=head3 do_while

This creates a new structured block with a new nested variable scope.

This function will evaluate a list of values (statements) at least once while
a condition is true. See the related functions "break", "continue", "return"
and "throw".
Evaluate a list of values (statements) at least once while a condition is true

=head4 Parameters

    $condition

While this condition is true, the loop will continue. This is evaluated in
the loop's scope.

    $body

This is a list of values (statements) that are evaluated for each iteration
of the loop. Each value in body is evaluated in order until the end of the
list or until a "break", "continue", "return" or "throw" function is
encountered.

=head3 eq_script

Determine if script arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use "eqx" ("===") instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_script

Determine if for script arg1 is equal to the value and data type of arg2 then
return the boolean result. Use "eq" ("==") instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 evaluate_script

Compile and evaluate script value.
Evaluate script value

=head4 Parameters

    $source

script string to compile and evaluate

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 for

This creates a new structured block with a new nested variable scope.

This function loops while condition is true. If the condition is false for
the first iteration, the loop returns a null value.
Evaluate a list of values (statements) while a condition is true with a list of initial and increment values

=head4 Parameters

    $initial

This is a list of values (statements) to evaluate before the loop starts. The
values will normally be a call to the "assign" function.

    $condition

While this condition is true, the loop will continue.

    $increment

This is a list of values (statements) to evaluate after each iteration of the
loop. The values will normally be a call to the "assign" function.

    $body

This is a list of values (statements) that are evaluated for each iteration
of the loop. Each value in body is evaluated in order until the end of the
list or until a "break", "continue", "return" or "throw" function is
encountered.

=head3 foreach

This creates a new structured block with a new nested variable scope.

This function will evaluate a list of values (statements) while a condition
is true with initial and increment values. The condition is tested at the
beginning of the loop. If the condition is false for the first iteration, the
loop returns a null value.
Evaluate a list of values (statements) while a condition is true with a list of initial and increment values

=head4 Parameters

    $name

Variable name(s).

    $value

Any list, object or single value.

    $body

This is a list of values (statements) that are evaluated for each iteration
of the loop. Each value in body is evaluated in order until the end of the
list or until a "break", "continue", "return" or "throw" function is
encountered.

=head3 ge_script

Checks for script arg1 is greater than or equal to script arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_script

Checks for script arg1 is greater than script arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 if

Evaluate one of two different values depending on test condition.
If/then/else function

=head4 Parameters

    $condition

If true, parameter "then" is evaluated for result. If false, parameter "else"
is evaluated.

    $then

This is the body of a structured block that is evaluated if "condition" is
true. See the "body" parameter of the "block" function for information on how
the body is processed.

    $else

This is the body of a structured block that is evaluated if "condition" is
false. If not specified and condition is false, a null value is returned. See
the "body" parameter of the "block" function for information on how the body
is processed.

=head3 is_script

Checks whether value is dataType script and return the boolean result.
Checks whether value is dataType script

=head4 Parameters

    $value

Value to check

=head3 le_script

Checks for script arg1 is less than or equal to script arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 loc

Declare one or more statically scoped variable locations local to the current
script block and optionally assign them an initial value. These variables can
be accessed and assigned different values from the current block and inner
blocks.
Declare one or more variables in the current block

=head4 Parameters

    $name

The name of one or more variables to declared in the current block.

    $value

This is the initial value of the variable(s). If not specified, the variable
will have a value of undefined.

    $type

The type of the variable(s).

=head3 lt_script

Checks for script arg1 is less that script arg2 and return the boolean
result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_script

Determine if script arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use "nex" ("!==") instead
if you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_script

Determine if for script arg1 is not equal to the value or data type of arg2
then return the boolean result. Use "ne" ("!=") instead if you want arg2 to
be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 rethrow

This is a special function that can be called to rethrow an error inside of a
catch block. If called outside of a catch body, an error is thrown.
rethrow error from inside case block

=head4 Parameters

=head3 return

Return from the outermost structured block. If the expression of a lambda
function is a block function, this will effectively return from the lambda
function. If called outside of a structured block, an error is thrown.
Return from the outermost structured block

=head4 Parameters

    $value

The value to evaluate that the outermost block will return. If not specified,
the last evaluated value or a null value will be returned.

=head3 script

Converts value to data type script returning script result.
Convert to data type script

=head4 Parameters

    $value

Value to convert

=head3 throw

This throws an error that can be caught by a try/catch block. An error object
of object type _AdaptiveResponseError_ will be available in the catch block.
Its "errorCodeId" property will be set to "throw". The other properties set
based on the parameters specified and where this function is called.
Throws an error

=head4 Parameters

    $message

This is the message that will be included in the _AdaptiveResponseError_
error object available in the catch block.

    $additional

Optional additional information that will be available as a "additional"
property in the error object.

=head3 try

This creates a new structured block with a new nested variable scope.

This function will evaluate the body statements. If an error is thrown and
there is an optional catch, the error will be "caught" and the associated
statements will be evaluated. The optional finally statements are always
evaluated after the body and catch statements. See the related functions
"break", "continue", "return" and "throw".
Evaluate a list of values (statements) as a try block with optional catch and finally statements

=head4 Parameters

    $body

This is a list of values (statements) that are evaluated. Each value in body
is evaluated in order until the end of the list or until a "break",
"continue", "return" or "throw" function is encountered.

    $finally

This is a list of values (statements) that are evaluated after the try and
catch statements even if an error occurs. Each value in body is evaluated in
order until the end of the list or until a "break", "continue", "return" or
"throw" function is encountered.

    $catch

This is a list of values (statements) that are evaluated when an error is
thrown while evaluating the body. Each value in body is evaluated in order
until the end of the list or until a "break", "continue", "return" or "throw"
function is encountered.

    $error

The error object thrown. This is only available in the catch block. See
adaptive object type _AdaptiveObjectType_ for details.

=head3 while

This creates a new structured block with a new nested variable scope.

This function will evaluate a list of values (statements) while a condition
is true. The condition is tested at the beginning of the loop. If the
condition is false for the first iteration, the loop returns a null value.
See the related functions "break", "continue", "return" and "throw".
Evaluate a list of values (statements) while a condition is true

=head4 Parameters

    $condition

While this condition is true, the loop will continue. This is evaluated in
the loop's scope.

    $body

This is a list of values (statements) that are evaluated for each iteration
of the loop. Each value in body is evaluated in order until the end of the
list or until a "break", "continue", "return" or "throw" function is
encountered.

=cut

sub assign {
    my ($name, $value) = @_;

    my $request = $session->request()

    $request->set("function" => "assign");
    $request->set("name", $name);
    $request->set("value", $value);

    return $request->getResult();
}

sub bag_script {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<script>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_script {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<script>");
    $request->set("value", $value);

    return $request->getResult();
}

sub break_ {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "break");

    if (defined $value)
        $request->set("value", $value);

    return $request->getResult();
}

sub compile_script {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<script>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub const {
    my ($name, $value, $type) = @_;

    my $request = $session->request()

    $request->set("function" => "const");
    $request->set("name", $name);
    $request->set("value", $value);

    if (defined $type)
        $request->set("type", $type);

    return $request->getResult();
}

sub continue_ {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "continue");

    return $request->getResult();
}

sub do_while {
    my ($condition, $body) = @_;

    my $request = $session->request()

    $request->set("function" => "do_while");
    $request->set("condition", $condition);
    $request->set("body", $body);

    return $request->getResult();
}

sub eq_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub evaluate_script {
    my ($source, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "evaluate<script>");
    $request->set("source", $source);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub for_ {
    my ($initial, $condition, $increment, $body) = @_;

    my $request = $session->request()

    $request->set("function" => "for");

    if (defined $initial)
        $request->set("initial", $initial);

    if (defined $condition)
        $request->set("condition", $condition);

    if (defined $increment)
        $request->set("increment", $increment);

    if (defined $body)
        $request->set("body", $body);

    return $request->getResult();
}

sub foreach {
    my ($name, $value, $body) = @_;

    my $request = $session->request()

    $request->set("function" => "foreach");
    $request->set("name", $name);
    $request->set("value", $value);

    if (defined $body)
        $request->set("body", $body);

    return $request->getResult();
}

sub ge_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub if_ {
    my ($condition, $then, $else) = @_;

    my $request = $session->request()

    $request->set("function" => "if");
    $request->set("condition", $condition);
    $request->set("then", $then);

    if (defined $else)
        $request->set("else", $else);

    return $request->getResult();
}

sub is_script {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<script>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub loc {
    my ($name, $value, $type) = @_;

    my $request = $session->request()

    $request->set("function" => "loc");
    $request->set("name", $name);

    if (defined $value)
        $request->set("value", $value);

    if (defined $type)
        $request->set("type", $type);

    return $request->getResult();
}

sub lt_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_script {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<script>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub rethrow {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "rethrow");

    return $request->getResult();
}

sub return_ {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "return");

    if (defined $value)
        $request->set("value", $value);

    return $request->getResult();
}

sub script {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "script");
    $request->set("value", $value);

    return $request->getResult();
}

sub throw {
    my ($message, $additional) = @_;

    my $request = $session->request()

    $request->set("function" => "throw");
    $request->set("message", $message);

    if (defined $additional)
        $request->set("additional", $additional);

    return $request->getResult();
}

sub try_ {
    my ($body, $finally, $catch, $error) = @_;

    my $request = $session->request()

    $request->set("function" => "try");
    $request->set("body", $body);

    if (defined $finally)
        $request->set("finally", $finally);

    if (defined $catch)
        $request->set("catch", $catch);

    if (defined $error)
        $request->set("error", $error);

    return $request->getResult();
}

sub while_ {
    my ($condition, $body) = @_;

    my $request = $session->request()

    $request->set("function" => "while");
    $request->set("condition", $condition);
    $request->set("body", $body);

    return $request->getResult();
}

42;
