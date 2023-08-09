package afw::compiler_test_script;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    test_script_runtime_support 
);

=head1 NAME

afw::compiler_test_script - Perl module for compiler_test_script functions

=head2 Functions

The following functions are exported by default

=head3 test_script_runtime_support

This is a function called internally as the result of a test_script compile.
This function is not intended to be called directly.
Internal test script runtime support

=head4 Parameters

    $testScriptObject

A test script results object with the required evaluation result properties
missing. The sources will be evaluated and the corresponding test result
properties will be set.

=cut

sub test_script_runtime_support {
    my ($testScriptObject) = @_;

    my $request = $session->request()

    $request->set("function" => "test_script_runtime_support");
    $request->set("testScriptObject", $testScriptObject);

    return $request->getResult();
}

42;
