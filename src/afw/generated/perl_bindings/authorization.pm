package afw::authorization;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    authorization_check 
);

=head1 NAME

afw::authorization - Perl module for authorization functions

=head2 Functions

The following functions are exported by default

=head3 authorization_check

This function can be used to check if the current subject has a given access
to the specified resource in the current environment. The returned object is
object type '_AdaptiveAuthorizationResult_'.
Perform authorization check

=head4 Parameters

    $requestId

This is a request id to associate with the authorization request. Variable
current::requestId can be used to access this value in authorization
policies.

    $resourceId

This is the URI of the resource being checked. Variable current::resourceId
can be used to access this value in authorization policies.

    $actionId

This is the action that on the resource that's being checked.  Variable
current::actionId can be used to access this value in authorization policies.

    $object

Variable current::object can be used to access this value in authorization
policies.

    $enforce

This specifies whether an error is thrown if access is denied. If false or
not specified, an error is not thrown and the decision can be checked in the
returned object.

=cut

sub authorization_check {
    my ($requestId, $resourceId, $actionId, $object, $enforce) = @_;

    my $request = $session->request()

    $request->set("function" => "authorization_check");
    $request->set("requestId", $requestId);
    $request->set("resourceId", $resourceId);
    $request->set("actionId", $actionId);

    if (defined $object)
        $request->set("object", $object);

    if (defined $enforce)
        $request->set("enforce", $enforce);

    return $request->getResult();
}

42;
