package afw::compiler;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    assert 
    compile_json 
    compile_relaxed_json 
    convert_syntax_hybrid_to_expression 
    decompile 
    evaluate_value 
    evaluate_with_retry 
    qualifier 
    qualifiers 
    safe_evaluate 
    stringify 
    test_expression 
    test_hybrid 
    test_script 
    test_template 
    test_value 
);

=head1 NAME

afw::compiler - Perl module for compiler functions

=head2 Functions

The following functions are exported by default

=head3 assert

Assert that a value is true. If not, an assertion_failed error is thrown.
Assert a value

=head4 Parameters

    $assertion

This is the assertion to make. If not true, an assertion_failed error is
thrown.

    $reason

This is an optional reason to include in the assertion_failed message.

=head3 compile_json

Compile a string containing adaptive JSON syntax and return either an
unevaluated JSON adaptive value or a string containing the compiler listing.
Compile JSON

=head4 Parameters

    $json

JSON string to compile.

    $listing

If specified, a compiler listing is produced instead of an unevaluated JSON
value.

This parameter can be an integer between 0 and 10 of a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 compile_relaxed_json

Compile an adaptive relaxed JSON syntax string and return an adaptive value.
Compile relaxed JSON

=head4 Parameters

    $json

Adaptive relaxed JSON syntax string to compile.

    $listing

If specified, a compiler listing is produced instead of an unevaluated relaxed
JSON value.

This parameter can be an integer between 0 and 10 of a string that is used for
indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 convert_syntax_hybrid_to_expression

Convert a string containing adaptive hybrid syntax, which can be an adaptive
template or adaptive expression, to adaptive expression syntax.
Convert a hybrid to a expression.

=head4 Parameters

    $hybrid

The hybrid to convert.

    $whitespace

Add whitespace for readability if present and not 0. This parameter can be an
integer between 0 and 10 or a string that is used for indentation. If 0 is
specified, no whitespace is added to the resulting string. If 1 through 10 is
specified, that number of spaces is used.

=head3 decompile

Decompile an adaptive value to string.
Decompile value

=head4 Parameters

    $value

Value to decompile.

    $whitespace

Add whitespace for readability if present and not 0. This parameter can be an
integer between 0 and 10 or a string that is used for indentation. If 0 is
specified, no whitespace is added to the resulting string. If 1 through 10 is
specified, that number of spaces is used.

=head3 evaluate_value

Evaluate an adaptive value.
Evaluate value

=head4 Parameters

    $value


    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 evaluate_with_retry

Evaluate a value and retry up to a limit if an exception occurs.
Evaluate value with retry

=head4 Parameters

    $value

Value to evaluated

    $limit

Maximum number to retry if an exception occurs.

=head3 qualifier

This function allows the active variables for a qualifier to be accessed as
the properties of an object.
Access variables of a qualifier as an object

=head4 Parameters

    $qualifier

This is the qualifier whose variables are to be accessed as properties of the
returned object.

    $forTesting

If specified and true, the object returned will be suitable to pass as the
additionalUntrustedQualifiedVariables parameter of evaluate*() functions. This
is intended for testing purposes and should not be used in production.

=head3 qualifiers

This function allows the active qualifiers to be accessed as properties of an
object. The value of each of these properties is an object whose properties
are the variables for the corresponding qualifier.
Access qualifiers as an object

=head4 Parameters

    $forTesting

If specified and true, the object returned will be suitable to pass as the
additionalUntrustedQualifiedVariables parameter of evaluate*() functions. This
is intended for testing purposes and should not be used in production.

=head3 safe_evaluate

Return the evaluated adaptive value. If an exception occurs, return evaluated
error instead.
Safe evaluate

=head4 Parameters

    $value

Value to evaluated

    $error

Value to evaluate and return if exception occurs. If an error occurs
evaluating this value, the exception will continue.

=head3 stringify

Evaluate and decompile an adaptive value to string. For most values this has
the effect of producing a string containing json.
Evaluate and decompile a value

=head4 Parameters

    $value

Value to stringify.

    $replacer

Optional replacer function.

    $whitespace

Add whitespace for readability if present and not 0. This parameter can be an
integer between 0 and 10 or a string that is used for indentation. If 0 is
specified, no whitespace is added to the resulting string. If 1 through 10 is
specified, that number of spaces is used.

=head3 test_expression

Compile and evaluate an adaptive expression and compare the results to an
expected value. Return object with the test's results.
Test expression

=head4 Parameters

    $id

Id of test

    $description

Description of test

    $expression

Expression to compile and evaluate.

    $expected

Expected result.

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 test_hybrid

Compile and evaluate a string containing adaptive hybrid syntax which can be
an adaptive template or adaptive expression and then compare the results to an
expected value. Return object with the test's results.
Test hybrid

=head4 Parameters

    $id

Id of test

    $description

Description of test

    $hybrid

Hybrid to compile and evaluate.

    $expected

Expected evaluated result.

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 test_script

Compile and evaluate an adaptive script and compare the results to an expected
value. Return object with the test's results.
Test an adaptive script

=head4 Parameters

    $id

Id of test

    $description

Description of test

    $script

Script to compile and evaluate.

    $expected

Expected result.

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 test_template

Compile and evaluate an adaptive template and compare the results to an
expected value. Return object with the test's results.
Test template

=head4 Parameters

    $id

Id of test

    $description

Description of test

    $template

Template to compile and evaluate.

    $expected

Expected evaluated result.

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 test_value

Evaluate an adaptive value and compare it to an expected value. Return object
with the test's results.
Test value

=head4 Parameters

    $id

Id of test

    $description

Description of test

    $value

Value to evaluate

    $expected

Expected result.

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=cut

sub assert_ {
    my ($assertion, $reason) = @_;

    my $request = $session->request()

    $request->set("function" => "assert");
    $request->set("assertion", $assertion);

    if (defined $reason)
        $request->set("reason", $reason);

    return $request->getResult();
}

sub compile_json {
    my ($json, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile_json");
    $request->set("json", $json);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub compile_relaxed_json {
    my ($json, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile_relaxed_json");
    $request->set("json", $json);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub convert_syntax_hybrid_to_expression {
    my ($hybrid, $whitespace) = @_;

    my $request = $session->request()

    $request->set("function" => "convert_syntax_hybrid_to_expression");
    $request->set("hybrid", $hybrid);

    if (defined $whitespace)
        $request->set("whitespace", $whitespace);

    return $request->getResult();
}

sub decompile {
    my ($value, $whitespace) = @_;

    my $request = $session->request()

    $request->set("function" => "decompile");
    $request->set("value", $value);

    if (defined $whitespace)
        $request->set("whitespace", $whitespace);

    return $request->getResult();
}

sub evaluate_value {
    my ($value, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "evaluate_value");
    $request->set("value", $value);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub evaluate_with_retry {
    my ($value, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "evaluate_with_retry");
    $request->set("value", $value);
    $request->set("limit", $limit);

    return $request->getResult();
}

sub qualifier {
    my ($qualifier, $forTesting) = @_;

    my $request = $session->request()

    $request->set("function" => "qualifier");
    $request->set("qualifier", $qualifier);

    if (defined $forTesting)
        $request->set("forTesting", $forTesting);

    return $request->getResult();
}

sub qualifiers {
    my ($forTesting) = @_;

    my $request = $session->request()

    $request->set("function" => "qualifiers");

    if (defined $forTesting)
        $request->set("forTesting", $forTesting);

    return $request->getResult();
}

sub safe_evaluate {
    my ($value, $error) = @_;

    my $request = $session->request()

    $request->set("function" => "safe_evaluate");
    $request->set("value", $value);
    $request->set("error", $error);

    return $request->getResult();
}

sub stringify {
    my ($value, $replacer, $whitespace) = @_;

    my $request = $session->request()

    $request->set("function" => "stringify");
    $request->set("value", $value);

    if (defined $replacer)
        $request->set("replacer", $replacer);

    if (defined $whitespace)
        $request->set("whitespace", $whitespace);

    return $request->getResult();
}

sub test_expression {
    my ($id, $description, $expression, $expected, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "test_expression");
    $request->set("id", $id);
    $request->set("description", $description);
    $request->set("expression", $expression);

    if (defined $expected)
        $request->set("expected", $expected);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub test_hybrid {
    my ($id, $description, $hybrid, $expected, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "test_hybrid");
    $request->set("id", $id);
    $request->set("description", $description);
    $request->set("hybrid", $hybrid);

    if (defined $expected)
        $request->set("expected", $expected);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub test_script {
    my ($id, $description, $script, $expected, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "test_script");
    $request->set("id", $id);
    $request->set("description", $description);
    $request->set("script", $script);

    if (defined $expected)
        $request->set("expected", $expected);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub test_template {
    my ($id, $description, $template, $expected, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "test_template");
    $request->set("id", $id);
    $request->set("description", $description);
    $request->set("template", $template);

    if (defined $expected)
        $request->set("expected", $expected);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub test_value {
    my ($id, $description, $value, $expected, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "test_value");
    $request->set("id", $id);
    $request->set("description", $description);
    $request->set("value", $value);

    if (defined $expected)
        $request->set("expected", $expected);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

42;
