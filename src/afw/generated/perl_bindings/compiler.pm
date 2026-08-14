#!/usr/bin/env perl

#**
# @file compiler.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the compiler category.
#*

package afw::compiler;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    assert 
    compile_from_file 
    decompile 
    eval_from_file 
    evaluate 
    evaluate_with_retry 
    qualifier 
    qualifiers 
    safe_evaluate 
    stringify 
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

=head3 compile_from_file

Load an external adaptive script, json, or template to be compiled and
returned.
Compile an external file

=head4 Parameters

    $file

The path of the file to include, resolved using rootFilePaths (longest
matching prefix; host path must remain under that root).

    $compileType

The compile type, used by the parser to determine how to compile the data.
For example, 'json', 'relaxed_json', 'script', 'template'

=head3 decompile

Decompile an adaptive value to Adaptive text that represents the compiled form
(functional forms and #implementation_id(...) pragmas such as
#script_function, #block, #assignment_target). This is not original source
recovery and is not pure JSON — use stringify() for JSON of evaluated data,
and compile(..., listing) for a human compiler listing with symbol tables.
Many decompile forms recompile to the same compiled value; #closure_binding
and #function_thunk are known rejects (runtime-only / C-side). Optional
whitespace matches stringify/listing style (integer 0-10 or indent string).
Decompile a value to Adaptive compiled form

=head4 Parameters

    $value

Value to decompile (may be unevaluated, such as a compiled script root).

    $whitespace

Add whitespace for readability if present and not 0. This parameter can be an
integer between 0 and 10 or a string that is used for indentation. If 0 is
specified, no whitespace is added to the resulting string. If 1 through 10 is
specified, that number of spaces is used.

=head3 eval_from_file

Load an external adaptive script, json, or template to be compiled and
evaluate.
Compile and evaluate an external file

=head4 Parameters

    $file

The path of the file to include, resolved using rootFilePaths (longest
matching prefix; host path must remain under that root).

    $compileType

The compile type, used by the parser to determine how to compile the data.
For example, 'json', 'relaxed_json', 'script', 'template'

=head3 evaluate

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

Returns a new memory object whose properties are the active variables for the
given qualifier (issue #9). Built from the current xctx qualifier stack via
contribute callbacks; not a live view. Each call creates a fresh object.
Intended for debugging, tooling, and tests — not for hot production paths that
only need qualifier::name access.

Warning: snapshots can be large. Qualifiers such as environment:: or request::
may contribute many properties (and some values can themselves be large
objects). qualifiers() nests a full snapshot per active qualifier name and
multiplies that cost. Prefer qualifier::name for normal work; use these
functions sparingly and avoid holding or repeatedly rebuilding large snapshots
in long-running scripts.

All matching visible stack entries for the qualifier name contribute into one
object (most recent first; later entries only fill property names not already
set). Get (qualifier::name) uses the same first-defining-frame rule per name
(newest → older; first non-null get_cb wins, including present undefined/null
values). Default visibility matches normal qualifier::name access right now.
Optional includeUntrusted is only meaningful while the xctx is secure: set
true so the snapshot includes the same frames you would see with :: if you
were less secure (trusted and untrusted). When already not secure, the flag
changes nothing.
Snapshot of variables for a qualifier as an object

=head4 Parameters

    $qualifier

This is the qualifier whose variables are to be accessed as properties of the
returned object.

    $includeUntrusted

Default false: snapshot matches what qualifier::name can access in the current
xctx (while secure, untrusted stack frames with secure=false are omitted). Set
true while secure to use the same visibility as running less secure — trusted
and untrusted frames (not untrusted-only). When the xctx is not secure, true
and false are the same because :: already sees untrusted frames. Does not
change hot-path get; only this snapshot. Useful for debugging secure
evaluation and for building objects to re-inject as evaluate()'s
additionalUntrustedQualifiedVariables.

=head3 qualifiers

Returns a new memory object whose properties are active qualifier names; each
value is an object of that qualifier's variables (issue #9). Built from the
current xctx qualifier stack; each call creates a fresh object. Intended for
debugging, tooling, and tests — not for hot production paths that only need
qualifier::name access.

Warning: the result can be very large. Each property is a full snapshot of
that qualifier (see qualifier()), so environment, request, application,
current, and others can all appear as nested objects with many properties.
Prefer qualifier::name or qualifier(name) when you need one bag; avoid
repeated qualifiers() calls or retaining the result in long-running work.

Each nested variables object is the multi-entry snapshot for that name (all
matching visible stack entries contribute; most recent wins per property). A
qualifier name is omitted if it is not active (same as qualifier(name) being
nullish); never invent an empty nested object for an inactive name. Default
visibility matches normal qualifier::name access right now. Optional
includeUntrusted is only meaningful while the xctx is secure: set true so each
nested snapshot uses the same frame visibility as running less secure (trusted
and untrusted). When already not secure, the flag changes nothing.
Snapshot of active qualifiers as an object

=head4 Parameters

    $includeUntrusted

Default false: only qualifiers/frames visible to qualifier::name in the
current xctx. Set true while secure to match less-secure :: visibility
(include untrusted frames). When not secure, true and false are the same. Does
not change hot-path get. The result shape (qualifier → variables object) is
suitable to pass as evaluate()'s additionalUntrustedQualifiedVariables when
that is the intent.

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

Evaluate value and serialize it as pure JSON text. Adaptive data types use
their jsonPrimitive (for example base64Binary and date become JSON strings).
The value is fully evaluated before serialization (not Adaptive compiled
form). For Adaptive compiled form as text use decompile(). For binary octets
as UTF-8 text use decode_to_string(); string(binary) is base64 printable text,
not UTF-8. Optional replacer is a function (key, value) that returns the value
to serialize, or an array of property names to include when serializing
objects. Optional whitespace matches decompile/listing style.
Serialize an evaluated value as JSON text

=head4 Parameters

    $value

Evaluated value to serialize as JSON.

    $replacer

Optional replacer: a function (key: string, value: any): any called for the
root (key is empty string) and each object property or array element; return
undefined to omit an object property (array elements become null). Or an array
of string property names to keep when serializing objects. Omit or null for no
replacer.

    $whitespace

Add whitespace for readability if present and not 0. This parameter can be an
integer between 0 and 10 or a string that is used for indentation. If 0 is
specified, no whitespace is added to the resulting string. If 1 through 10 is
specified, that number of spaces is used.

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

sub compile_from_file {
    my ($file, $compileType) = @_;

    my $request = $session->request()

    $request->set("function" => "compile_from_file");
    $request->set("file", $file);

    if (defined $compileType)
        $request->set("compileType", $compileType);

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

sub eval_from_file {
    my ($file, $compileType) = @_;

    my $request = $session->request()

    $request->set("function" => "eval_from_file");
    $request->set("file", $file);

    if (defined $compileType)
        $request->set("compileType", $compileType);

    return $request->getResult();
}

sub evaluate {
    my ($value, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "evaluate");
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
    my ($qualifier, $includeUntrusted) = @_;

    my $request = $session->request()

    $request->set("function" => "qualifier");
    $request->set("qualifier", $qualifier);

    if (defined $includeUntrusted)
        $request->set("includeUntrusted", $includeUntrusted);

    return $request->getResult();
}

sub qualifiers {
    my ($includeUntrusted) = @_;

    my $request = $session->request()

    $request->set("function" => "qualifiers");

    if (defined $includeUntrusted)
        $request->set("includeUntrusted", $includeUntrusted);

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
