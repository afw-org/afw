package afw::model;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    model_default_add_object_action 
    model_default_delete_object_action 
    model_default_modify_object_action 
    model_default_replace_object_action 
    model_mapBackObject_signature 
    model_mapObject_signature 
    model_returnObject_signature 
);

=head1 NAME

afw::model - Perl module for model functions

=head2 Functions

The following functions are exported by default

=head3 model_default_add_object_action

This function returns a _AdaptiveAction_ object for the default action
performed if a onAdd parameter is not specified on the associated
_AdaptiveModelObjectType_. The action is not performed, but be aware that
functions called while producing the _AdaptiveAction_ object may cause side
effects. This function can be called as part of a onAdd expression or for
testing.
Produce a default _AdaptiveAction_ object for an add object request to a model adaptor.

=head4 Parameters

    $adaptorId

This is the adaptorId of a model adaptor. Variable custom::adaptorId can be
used to access this value in model expressions.

    $objectType

This is the adaptive object type of object being added. Variable
custom::objectType can be used to access this value in model expressions.

    $object

This is the object to add. Variable custom::object can be used to access this
value in model expressions.

    $objectId

This is the optional preferred objectId of object to add. The adaptor may
ignore this. Variable custom::objectId can be used to access this value in
model expressions.

    $modelId

This specifics a modelId of model to use for producing results. If not
specified, the adaptor's current model will be used.

    $context

This specifies additional context information available to model expressions.

=head3 model_default_delete_object_action

This function returns a _AdaptiveAction_ object for the default action
performed if a onDeleteObject parameter is not specified on the associated
_AdaptiveModelObjectType_. The action is not performed, but be aware that
functions called while producing the _AdaptiveAction_ object may cause side
effects. This function can be called as part of a onDelete expression or for
testing.
Produce a default _AdaptiveAction_ object for an delete object request to a model adaptor.

=head4 Parameters

    $adaptorId

This is the adaptorId of a model adaptor. Variable custom::adaptorId can be
used to access this value in model expressions.

    $objectType

This is the adaptive object type of object being deleted. Variable
custom::objectType can be used to access this value in model expressions.

    $objectId

This is the objectId of object to delete. Variable custom::object can be used
to access this value in model expressions.

    $modelId

This specifics a modelId of model to use for producing results. If not
specified, the adaptor's current model will be used.

    $context

This specifies additional context information available to model expressions.

=head3 model_default_modify_object_action

This function returns a _AdaptiveAction_ object for the default action
performed if a onModifyObject parameter is not specified on the associated
_AdaptiveModelObjectType_. The action is not performed, but be aware that
functions called while producing the _AdaptiveAction_ object may cause side
effects. This function can be called as part of a onModify expression or for
testing.
Produce a default _AdaptiveAction_ object for an modify object request to a model adaptor.

=head4 Parameters

    $adaptorId

This is the adaptorId of a model adaptor. Variable custom::adaptorId can be
used to access this value in model expressions.

    $objectType

This is the adaptive object type of object being modified. Variable
custom::objectType can be used to access this value in model expressions.

    $objectId

This is the objectId of object to modify. Variable custom::objectId can be
used to access this value in model expressions.

    $entries

This is an array of modifications. Variable custom::actions can be used to
access this value in model expressions. Entries are of the form:

    [
        'add_value',
        'property name' or ['property name', ... ],
        value
    ]

    [
        'remove_property',
        'property name' or ['property name', ... ]
    ]

    [
        'remove_value',
        'property name' or ['property name', ... ],
        value
    ]

    [
        'set_property',
        'property name' or ['property name', ... ],
        value
    ]

    $modelId

This specifics a modelId of model to use for producing results. If not
specified, the adaptor's current model will be used.

    $context

This specifies additional context information available to model expressions.

=head3 model_default_replace_object_action

This function returns a _AdaptiveAction_ object for the default action
performed if a onReplaceObject parameter is not specified on the associated
_AdaptiveModelObjectType_. The action is not performed, but be aware that
functions called while producing the _AdaptiveAction_ object may cause side
effects. This function can be called as part of a onReplace expression or for
testing.
Produce a default _AdaptiveAction_ object for an replace object request to a model adaptor.

=head4 Parameters

    $adaptorId

This is the adaptorId of a model adaptor. Variable custom::adaptorId can be
used to access this value in model expressions.

    $objectType

This is the adaptive object type of object being replaced. Variable
custom::objectType can be used to access this value in model expressions.

    $objectId

The objectId of object to replace. Variable custom::objectId can be used to
access this value in model expressions.

    $object

This is the object to replace. Variable custom::object can be used to access
this value in model expressions.

    $modelId

This specifics a modelId of model to use for producing results. If not
specified, the adaptor's current model will be used.

    $context

This specifies additional context information available to model expressions.

=head3 model_mapBackObject_signature

This is the function signature for current:mapBackObject used by onGetObjects
and onRetrieveObjects. Calling this directly will throw a 'Do not call
directly' error.
Signature used for current:mapBackObject()

=head4 Parameters

    $mappedObject

This is the object to map back.

=head3 model_mapObject_signature

This is the function signature for current:mapObject used by onAddObject.
onDeleteObject, onModifyObject, and onReplaceObject. Calling this directly
will throw a 'Do not call directly' error.
Signature used for current:mapObject()

=head4 Parameters

    $object

This is the object to map.

=head3 model_returnObject_signature

This is the function signature for current:returnObject used by onGetObjects
and onRetrieveObjects. Calling this directly will throw a 'Do not call
directly' error.
Signature used for current:returnObject()

=head4 Parameters

    $object

This is the object to return.

    $userData

If this is present and true, the object will be mapped its mapped adaptor's
object type to the model adaptor's object type.

=cut

sub model_default_add_object_action {
    my ($adaptorId, $objectType, $object, $objectId, $modelId, $context) = @_;

    my $request = $session->request()

    $request->set("function" => "model_default_add_object_action");
    $request->set("objectType", $objectType);
    $request->set("object", $object);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $objectId)
        $request->set("objectId", $objectId);

    if (defined $modelId)
        $request->set("modelId", $modelId);

    if (defined $context)
        $request->set("context", $context);

    return $request->getResult();
}

sub model_default_delete_object_action {
    my ($adaptorId, $objectType, $objectId, $modelId, $context) = @_;

    my $request = $session->request()

    $request->set("function" => "model_default_delete_object_action");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $modelId)
        $request->set("modelId", $modelId);

    if (defined $context)
        $request->set("context", $context);

    return $request->getResult();
}

sub model_default_modify_object_action {
    my ($adaptorId, $objectType, $objectId, $entries, $modelId, $context) = @_;

    my $request = $session->request()

    $request->set("function" => "model_default_modify_object_action");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);
    $request->set("entries", $entries);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $modelId)
        $request->set("modelId", $modelId);

    if (defined $context)
        $request->set("context", $context);

    return $request->getResult();
}

sub model_default_replace_object_action {
    my ($adaptorId, $objectType, $objectId, $object, $modelId, $context) = @_;

    my $request = $session->request()

    $request->set("function" => "model_default_replace_object_action");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);
    $request->set("object", $object);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $modelId)
        $request->set("modelId", $modelId);

    if (defined $context)
        $request->set("context", $context);

    return $request->getResult();
}

sub model_mapBackObject_signature {
    my ($mappedObject) = @_;

    my $request = $session->request()

    $request->set("function" => "model_mapBackObject_signature");
    $request->set("mappedObject", $mappedObject);

    return $request->getResult();
}

sub model_mapObject_signature {
    my ($object) = @_;

    my $request = $session->request()

    $request->set("function" => "model_mapObject_signature");
    $request->set("object", $object);

    return $request->getResult();
}

sub model_returnObject_signature {
    my ($object, $userData) = @_;

    my $request = $session->request()

    $request->set("function" => "model_returnObject_signature");
    $request->set("object", $object);

    if (defined $userData)
        $request->set("userData", $userData);

    return $request->getResult();
}

42;
