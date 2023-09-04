package afw::adaptor;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    adaptor_objectCallback_signature 
    add_object 
    add_object_with_uri 
    convert_AdaptiveQueryCriteria_to_query_string 
    convert_query_string_to_AdaptiveQueryCriteria 
    delete_object 
    delete_object_with_uri 
    get_object 
    get_object_with_uri 
    modify_object 
    modify_object_with_uri 
    reconcile_object 
    replace_object 
    replace_object_with_uri 
    retrieve_objects 
    retrieve_objects_to_callback 
    retrieve_objects_to_response 
    retrieve_objects_to_stream 
    retrieve_objects_with_uri 
    retrieve_objects_with_uri_to_callback 
    retrieve_objects_with_uri_to_response 
    retrieve_objects_with_uri_to_stream 
    update_object 
    update_object_with_uri 
);

=head1 NAME

afw::adaptor - Perl module for adaptor functions

=head2 Functions

The following functions are exported by default

=head3 adaptor_objectCallback_signature

This is the function signature for the objectCallback parameter in adaptor
functions. Calling this directly will throw a 'Do not call directly' error.
Signature used for adaptor objectCallback

=head4 Parameters

    $object

This is the object from adaptor or undefined if there are no more objects to
return.

    $userData

This is the value of the userData parameter when objectCallback was called.

=head3 add_object

Add an adaptive object to an adaptor, specified by the adaptorId, with a
specified objectType. You may supply an optional objectId, if the underlying
adaptorId allows it.
Add adaptive object

=head4 Parameters

    $adaptorId

Id of adaptor.

    $objectType

Id of adaptive object type of object being added.

    $object

Object to add.

    $objectId

Optional preferred objectId of object to add. The adaptor may ignore this.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_add_object

where ${adaptorType} is the adaptor type id.

=head3 add_object_with_uri

Add an adaptive object with a given URI.
Add adaptive object with URI

=head4 Parameters

    $uri

URI of object to add. If a URI begins with a single slash ('/'), it is the
local object path. In the case of a local path, an optional preferred objectId
of object can be including in URI which the adaptor may ignore.

    $object

Object to add.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_add_object

Where ${adaptorType} is the adaptor type id.

=head3 convert_AdaptiveQueryCriteria_to_query_string

This function converts an _AdaptiveQueryCriteria_ object to a query string.
Convert _AdaptiveQueryCriteria_ to RQL

=head4 Parameters

    $queryCriteria

An object type _AdaptiveQueryCriteria_ object.

    $adaptorId

This is the adaptorId of adaptor containing the objectType for validating and
mapping this query. If not specified, no validation or mapping will occur.

    $objectType

This is the objectType used in conjunction with the adaptorId parameter. This
parameter is only allowed and is required if the adaptorId parameter is
specified.

    $style

The style of the query string produced. All of the styles are appropriately
url encoded.

0 - 'name=op=value' where '=op=' will be '==', '!=', '=ge=', etc. with
conjunctions '&' (and) and '|' (or). Parentheses are placed around each group
of like conjunctions except for outer '&'. (default)

1 - the same as 1 with '=gte=' instead of '=ge=', '=lte=' instead of '=le=',
'=eq=' instead of '=', and '=ne=' instead of '!='.

2 - similar to 1 with ';' for '&' and ',' for '|'.

3 - comparisons 'op(name,value)' where 'op' will be 'eq', 'ne', 'ge', ...,
plus conjunctions 'and(list)' and 'or(list)' where 'array' is a comma
separated list of any comparison or conjunction.

=head3 convert_query_string_to_AdaptiveQueryCriteria

This function converts a query string to an _AdaptiveQueryCriteria_ object.
Convert query string to _AdaptiveQueryCriteria_

=head4 Parameters

    $queryString

The query string to convert.

    $adaptorId

This is the adaptorId of adaptor containing the objectType for validating and
mapping this query. If not specified, no validation or mapping will occur.

    $objectType

This is the objectType used in conjunction with the adaptorId parameter. This
parameter is only allowed and is required if the adaptorId parameter is
specified.

=head3 delete_object

Delete an adaptive object.
Delete adaptive object

=head4 Parameters

    $adaptorId

Id of adaptor containing object to delete.

    $objectType

Id of adaptive object type of object to delete.

    $objectId

Id of object to delete.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_delete_object

where ${adaptorType} is the adaptor type id.

=head3 delete_object_with_uri

Delete an adaptive object with a given URI.
Delete adaptive object with URI

=head4 Parameters

    $uri

URI of object to delete. If a URI begins with a single slash ('/'), it is the
local object path.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_delete_object

where ${adaptorType} is the adaptor type id.

=head3 get_object

Get an adaptive object from the adaptor, specified by adaptorId, objectType
and objectId. Optional view options and adaptor-specific options may be
supplied to influence how the object is returned.
Get adaptive object

=head4 Parameters

    $adaptorId

Id of adaptor containing object to retrieve.

    $objectType

Id of adaptive object type of object to retrieve.

    $objectId

Id of object to retrieve.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_get_object

Where ${adaptorType} is the adaptor type id.

=head3 get_object_with_uri

Get an object with a given URI.
Get adaptive object with URI

=head4 Parameters

    $uri

URI of object to retrieve. If a URI begins with a single slash ('/'), it is
the local object path.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_get_object

where ${adaptorType} is the adaptor type id.

=head3 modify_object

Modify an adaptive object.
Modify adaptive object

=head4 Parameters

    $adaptorId

Id of adaptor containing object to modify.

    $objectType

Id of adaptive object type of object to modify.

    $objectId

Id of object to modify.

    $entries

List of modifications. Entries are of the form:

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

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_modify_object

Where ${adaptorType} is the adaptor type id.

=head3 modify_object_with_uri

Modify an adaptive object with a given URI.
Modify adaptive object with URI

=head4 Parameters

    $uri

URI of object to modify. If a URI begins with a single slash ('/'), it is the
local object path.

    $entries

List of asserts and modifications. Entries are of the form:

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

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_modify_object

Where ${adaptorType} is the adaptor type id.

=head3 reconcile_object

Reconcile an adaptive object.
Reconcile adaptive object

=head4 Parameters

    $object

An object that has been retrieved with object options reconcilable, and whose
properties have been modified. A modify_object request will be produced
reflecting the difference between the original object and this unmodified
object.

    $checkOnly

Produce the modify_object request but don't execute it. The returned journal
enter will contain the modify_object request.

=head3 replace_object

Replace an adaptive object.
Replace adaptive object

=head4 Parameters

    $adaptorId

Id of adaptor containing object to replace.

    $objectType

Id of adaptive object type of object to replace.

    $objectId

Id of object to replace.

    $object

Replacement object.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_replace_object

Where ${adaptorType} is the adaptor type id.

=head3 replace_object_with_uri

Replace an adaptive object with a given URI.
Replace adaptive object with URI

=head4 Parameters

    $uri

URI of object to replace. If a URI begins with a single slash ('/'), it is the
local object path.

    $object

Replacement object.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_replace_object

Where ${adaptorType} is the adaptor type id.

=head3 retrieve_objects

This function retrieves adaptive objects from an adaptor, specified by
adaptorId, which match the type specified by objectType.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adaptorId, can be optionally supplied.
Retrieve adaptive objects

=head4 Parameters

    $adaptorId

Id of adaptor containing objects to retrieve.

    $objectType

Id of adaptive object type of objects to retrieve.

    $queryCriteria

This is the query criteria for objects to be retrieved. If not specified, all
objects will be retrieved.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects

Where ${adaptorType} is the adaptor type id.

=head3 retrieve_objects_to_callback

This function retrieves adaptive objects from an adaptor, specified by
adaptorId, which match the type specified by objectType.

Each object is passed to the callback as it's retrieved.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adaptorId, can be optionally supplied.
Retrieve adaptive objects

=head4 Parameters

    $objectCallback

If this is specified, this function is called once for each object retrieved
instead of adding the object to the return array. Parameter object will be an
object retrieved or undefined if there are no more objects. This function
should return true if it wants to abort the retrieve request.

    $userData

This value is passed to the objectCallback function in the userData parameter.

    $adaptorId

Id of adaptor containing objects to retrieve.

    $objectType

Id of adaptive object type of objects to retrieve.

    $queryCriteria

This is the query criteria for objects to be retrieved. If not specified, all
objects will be retrieved.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects_to_callback

Where ${adaptorType} is the adaptor type id.

=head3 retrieve_objects_to_response

This function retrieves adaptive objects from an adaptor, specified by
adaptorId, which match the type specified by objectType.

This function is only allowed when called during a request with content type
'application/x-afw'. An '_AdaptiveResponse_' object is written to the
request's response as each object is retrieved. This '_AdaptiveResponse_'
object has a 'result' property containing the retrieved object and a
'intermediate' property set to true.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adaptorId, can be optionally supplied.
Retrieve adaptive objects to a response

=head4 Parameters

    $adaptorId

Id of adaptor containing objects to retrieve.

    $objectType

Id of adaptive object type of objects to retrieve.

    $queryCriteria

This is the query criteria for objects to be retrieved. If not specified, all
objects will be retrieved.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects

Where ${adaptorType} is the adaptor type id.

=head3 retrieve_objects_to_stream

This function retrieves adaptive objects from an adaptor, specified by
adaptorId, which match the type specified by objectType.

Each object is written to the 'streamId' stream as it's retrieved.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adaptorId, can be optionally supplied.
Retrieve adaptive objects to a stream

=head4 Parameters

    $streamNumber

Stream number objects will be written to as they are retrieved.

    $adaptorId

Id of adaptor containing objects to retrieve.

    $objectType

Id of adaptive object type of objects to retrieve.

    $queryCriteria

This is the query criteria for objects to be retrieved. If not specified, all
objects will be retrieved.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects

Where ${adaptorType} is the adaptor type id.

=head3 retrieve_objects_with_uri

Retrieve adaptive objects with a given URI.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adaptorId, can be optionally supplied.
Retrieve adaptive object with URI

=head4 Parameters

    $uri

URI of objects to retrieve. If a URI begins with a single slash ('/'), it is
the local object path. A query string can be specified.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects

Where ${adaptorType} is the adaptor type id.

=head3 retrieve_objects_with_uri_to_callback

Retrieve adaptive objects with a given URI.

Each object is passed to the callback as it's retrieved.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adaptorId, can be optionally supplied.
Retrieve adaptive object with URI

=head4 Parameters

    $objectCallback

If this is specified, this function is called once for each object retrieved
instead of adding the object to the return array. Parameter object will be an
object retrieved or undefined if there are no more objects. This function
should return true if it wants to abort the retrieve request.

    $userData

This is the value passed to the objectCallback function in the userData
parameter.

    $uri

URI of objects to retrieve. If a URI begins with a single slash ('/'), it is
the local object path. A query string can be specified.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects

Where ${adaptorType} is the adaptor type id.

=head3 retrieve_objects_with_uri_to_response

This function retrieves adaptive objects with a given URI.

This function is only allowed when called during a request with content type
'application/x-afw'. An '_AdaptiveResponse_' object is written to the
request's response as each object is retrieved. This '_AdaptiveResponse_'
object has a 'result' property containing the retrieved object and a 'partial'
property set to true.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adaptorId, can be optionally supplied.
Retrieve adaptive object with URI to response

=head4 Parameters

    $uri

URI of objects to retrieve. If a URI begins with a single slash ('/'), it is
the local object path. A query string can be specified.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects

Where ${adaptorType} is the adaptor type id.

=head3 retrieve_objects_with_uri_to_stream

This function retrieves adaptive objects with a given URI.

Each object is written to the 'streamId' stream as it's retrieved.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adaptorId, can be optionally supplied.
Retrieve adaptive object with URI to stream

=head4 Parameters

    $streamNumber

Stream number objects will be written to as they are retrieved.

    $uri

URI of objects to retrieve. If a URI begins with a single slash ('/'), it is
the local object path. A query string can be specified.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_retrieve_objects

Where ${adaptorType} is the adaptor type id.

=head3 update_object

Update an adaptive object.
Update adaptive object

=head4 Parameters

    $adaptorId

Id of adaptor containing object to update.

    $objectType

Id of adaptive object type of object to update.

    $objectId

Id of object to update.

    $object

Object containing properties to replace in existing object. You can use
object_modify instead to update properties of embedded objects and arrays, as
well as to modify individual values of the object.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_modify_object

Where ${adaptorType} is the adaptor type id.

=head3 update_object_with_uri

Update an adaptive object with a given URI.
Update adaptive object with URI

=head4 Parameters

    $uri

URI of object to update. If a URI begins with a single slash ('/'), it is the
local object path.

    $object

Object containing properties to replace in existing object. You can use
object_modify instead to update properties of embedded objects and arrays, as
well as to modify individual values of the object.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adaptorTypeSpecific

This is an optional object parameter with an objectType determined by the
adaptorType associated with the adaptorId parameter. If the adaptorType
supports this parameter, adaptor afw will have an adaptive object type with an
id of:

_AdaptiveAdaptorTypeSpecific_${adaptorType}_modify_object

Where ${adaptorType} is the adaptor type id.

=cut

sub adaptor_objectCallback_signature {
    my ($object, $userData) = @_;

    my $request = $session->request()

    $request->set("function" => "adaptor_objectCallback_signature");
    $request->set("object", $object);

    if (defined $userData)
        $request->set("userData", $userData);

    return $request->getResult();
}

sub add_object {
    my ($adaptorId, $objectType, $object, $objectId, $journal, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "add_object");
    $request->set("objectType", $objectType);
    $request->set("object", $object);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $objectId)
        $request->set("objectId", $objectId);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub add_object_with_uri {
    my ($uri, $object, $journal, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "add_object_with_uri");
    $request->set("uri", $uri);
    $request->set("object", $object);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub convert_AdaptiveQueryCriteria_to_query_string {
    my ($queryCriteria, $adaptorId, $objectType, $style) = @_;

    my $request = $session->request()

    $request->set("function" => "convert_AdaptiveQueryCriteria_to_query_string");
    $request->set("queryCriteria", $queryCriteria);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $objectType)
        $request->set("objectType", $objectType);

    if (defined $style)
        $request->set("style", $style);

    return $request->getResult();
}

sub convert_query_string_to_AdaptiveQueryCriteria {
    my ($queryString, $adaptorId, $objectType) = @_;

    my $request = $session->request()

    $request->set("function" => "convert_query_string_to_AdaptiveQueryCriteria");
    $request->set("queryString", $queryString);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $objectType)
        $request->set("objectType", $objectType);

    return $request->getResult();
}

sub delete_object {
    my ($adaptorId, $objectType, $objectId, $journal, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "delete_object");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub delete_object_with_uri {
    my ($uri, $journal, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "delete_object_with_uri");
    $request->set("uri", $uri);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub get_object {
    my ($adaptorId, $objectType, $objectId, $options, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "get_object");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub get_object_with_uri {
    my ($uri, $options, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "get_object_with_uri");
    $request->set("uri", $uri);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub modify_object {
    my ($adaptorId, $objectType, $objectId, $entries, $journal, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "modify_object");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);
    $request->set("entries", $entries);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub modify_object_with_uri {
    my ($uri, $entries, $journal, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "modify_object_with_uri");
    $request->set("uri", $uri);
    $request->set("entries", $entries);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub reconcile_object {
    my ($object, $checkOnly) = @_;

    my $request = $session->request()

    $request->set("function" => "reconcile_object");
    $request->set("object", $object);

    if (defined $checkOnly)
        $request->set("checkOnly", $checkOnly);

    return $request->getResult();
}

sub replace_object {
    my ($adaptorId, $objectType, $objectId, $object, $journal, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "replace_object");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);
    $request->set("object", $object);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub replace_object_with_uri {
    my ($uri, $object, $journal, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "replace_object_with_uri");
    $request->set("uri", $uri);
    $request->set("object", $object);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects {
    my ($adaptorId, $objectType, $queryCriteria, $options, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects");
    $request->set("objectType", $objectType);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $queryCriteria)
        $request->set("queryCriteria", $queryCriteria);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_to_callback {
    my ($objectCallback, $userData, $adaptorId, $objectType, $queryCriteria, $options, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_to_callback");
    $request->set("objectCallback", $objectCallback);
    $request->set("userData", $userData);
    $request->set("objectType", $objectType);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $queryCriteria)
        $request->set("queryCriteria", $queryCriteria);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_to_response {
    my ($adaptorId, $objectType, $queryCriteria, $options, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_to_response");
    $request->set("objectType", $objectType);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $queryCriteria)
        $request->set("queryCriteria", $queryCriteria);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_to_stream {
    my ($streamNumber, $adaptorId, $objectType, $queryCriteria, $options, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_to_stream");
    $request->set("streamNumber", $streamNumber);
    $request->set("objectType", $objectType);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $queryCriteria)
        $request->set("queryCriteria", $queryCriteria);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_with_uri {
    my ($uri, $options, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_with_uri");
    $request->set("uri", $uri);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_with_uri_to_callback {
    my ($objectCallback, $userData, $uri, $options, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_with_uri_to_callback");
    $request->set("objectCallback", $objectCallback);
    $request->set("userData", $userData);
    $request->set("uri", $uri);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_with_uri_to_response {
    my ($uri, $options, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_with_uri_to_response");
    $request->set("uri", $uri);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_with_uri_to_stream {
    my ($streamNumber, $uri, $options, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_with_uri_to_stream");
    $request->set("streamNumber", $streamNumber);
    $request->set("uri", $uri);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub update_object {
    my ($adaptorId, $objectType, $objectId, $object, $journal, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "update_object");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);
    $request->set("object", $object);

    if (defined $adaptorId)
        $request->set("adaptorId", $adaptorId);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

sub update_object_with_uri {
    my ($uri, $object, $journal, $adaptorTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "update_object_with_uri");
    $request->set("uri", $uri);
    $request->set("object", $object);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adaptorTypeSpecific)
        $request->set("adaptorTypeSpecific", $adaptorTypeSpecific);

    return $request->getResult();
}

42;
