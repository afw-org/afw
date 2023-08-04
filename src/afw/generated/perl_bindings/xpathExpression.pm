package afw::xpathExpression;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    bag_size_xpathExpression 
    bag_xpathExpression 
    compile_xpathExpression 
    eq_xpathExpression 
    eqx_xpathExpression 
    evaluate_xpathExpression 
    ge_xpathExpression 
    gt_xpathExpression 
    is_xpathExpression 
    le_xpathExpression 
    lt_xpathExpression 
    ne_xpathExpression 
    nex_xpathExpression 
    to_string_xpathExpression 
    xpathExpression 
    xpath_node_count 
    xpath_node_eq 
    xpath_node_match 
);

=head1 NAME

afw::xpathExpression - Perl module for xpathExpression functions

=head2 Functions

The following functions are exported by default

=head3 bag_size_xpathExpression

This returns the integer number of values in list.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 bag_xpathExpression

Takes any number of xpathExpression values and returns a list of list.
Makes a list from values

=head4 Parameters

    $values


=head3 compile_xpathExpression

Compile xpathExpression value and return either an unevaluated adaptive value
or a string containing the compiler listing.
Compile xpathExpression value

=head4 Parameters

    $source

xpathExpression string to compile

    $listing

If specified, a compiler listing is produced instead of an unevaluated
expression value.

This parameter can be an integer between 0 and 10 of a string that is used
for indentation. If 0 is specified, no whitespace is added to the resulting
string. If 1 through 10 is specified, that number of spaces is used.

=head3 eq_xpathExpression

Determine if xpathExpression arg1 is equal to the value of arg2 converted to
the data type of arg1 then return the boolean result. Use "eqx" ("===")
instead if you want false to be returned if arg1 and arg2's data type don't
match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_xpathExpression

Determine if for xpathExpression arg1 is equal to the value and data type of
arg2 then return the boolean result. Use "eq" ("==") instead if you want arg2
to be converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 evaluate_xpathExpression

Compile and evaluate xpathExpression value.
Evaluate xpathExpression value

=head4 Parameters

    $source

xpathExpression string to compile and evaluate

    $additionalUntrustedQualifiedVariables

This parameter supplies additional qualified variables that can be accessed
during evaluation. These variables will not be used by anything that needs to
ensure its qualified variables must come from a trusted source, such as
authorization. This parameter is intended to be used for testing only and
should not be used for anything running in production.

=head3 ge_xpathExpression

Checks for xpathExpression arg1 is greater than or equal to xpathExpression
arg2 and return the boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_xpathExpression

Checks for xpathExpression arg1 is greater than xpathExpression arg2 and
return the boolean result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_xpathExpression

Checks whether value is dataType xpathExpression and return the boolean
result.
Checks whether value is dataType xpathExpression

=head4 Parameters

    $value

Value to check

=head3 le_xpathExpression

Checks for xpathExpression arg1 is less than or equal to xpathExpression arg2
and return the boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 lt_xpathExpression

Checks for xpathExpression arg1 is less that xpathExpression arg2 and return
the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_xpathExpression

Determine if xpathExpression arg1 is not equal to the value of arg2 converted
to the data type of arg1 then return the boolean result. Use "nex" ("!==")
instead if you want true to be returned if arg1 and arg2's data type don't
match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_xpathExpression

Determine if for xpathExpression arg1 is not equal to the value or data type
of arg2 then return the boolean result. Use "ne" ("!=") instead if you want
arg2 to be converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 to_string_xpathExpression

Converts xpathExpression value to string. For list values, the to_string()
value for each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A xpathExpression value.

=head3 xpathExpression

Converts value to data type xpathExpression returning xpathExpression result.
Convert to data type xpathExpression

=head4 Parameters

    $value

Value to convert

=head3 xpath_node_count

The number of nodes in a node-set.
Number of nodes in node-set

=head4 Parameters

    $nodeset


=head3 xpath_node_eq

Checks for xpathExpression arg1 is equal to xpathExpression arg2 and return
the boolean result.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 xpath_node_match

Returns true if any of the nodes matched by nodeset1 are equal to any of the
nodes matched by nodeset2 or their corresponding children.
Checks whether two node-sets or their children have an equal node

=head4 Parameters

    $nodeset2


    $arg2


=cut

sub bag_size_xpathExpression {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<xpathExpression>");
    $request->set("value", $value);

    return $request->getResult();
}

sub bag_xpathExpression {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<xpathExpression>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub compile_xpathExpression {
    my ($source, $listing) = @_;

    my $request = $session->request()

    $request->set("function" => "compile<xpathExpression>");
    $request->set("source", $source);

    if (defined $listing)
        $request->set("listing", $listing);

    return $request->getResult();
}

sub eq_xpathExpression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<xpathExpression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_xpathExpression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<xpathExpression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub evaluate_xpathExpression {
    my ($source, $additionalUntrustedQualifiedVariables) = @_;

    my $request = $session->request()

    $request->set("function" => "evaluate<xpathExpression>");
    $request->set("source", $source);

    if (defined $additionalUntrustedQualifiedVariables)
        $request->set("additionalUntrustedQualifiedVariables", $additionalUntrustedQualifiedVariables);

    return $request->getResult();
}

sub ge_xpathExpression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<xpathExpression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_xpathExpression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<xpathExpression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_xpathExpression {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<xpathExpression>");
    $request->set("value", $value);

    return $request->getResult();
}

sub le_xpathExpression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<xpathExpression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub lt_xpathExpression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<xpathExpression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_xpathExpression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<xpathExpression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_xpathExpression {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<xpathExpression>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub to_string_xpathExpression {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<xpathExpression>");
    $request->set("value", $value);

    return $request->getResult();
}

sub xpathExpression {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "xpathExpression");
    $request->set("value", $value);

    return $request->getResult();
}

sub xpath_node_count {
    my ($nodeset) = @_;

    my $request = $session->request()

    $request->set("function" => "xpath_node_count");
    $request->set("nodeset", $nodeset);

    return $request->getResult();
}

sub xpath_node_eq {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "xpath_node_eq");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub xpath_node_match {
    my ($nodeset2, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "xpath_node_match");
    $request->set("nodeset2", $nodeset2);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

42;
