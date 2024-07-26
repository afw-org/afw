#!/usr/bin/env perl

#**
# @file adapter.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the adapter category.
#*

package afw::adapter;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    adapter_objectCallback_signature 
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

afw::adapter - Perl module for adapter functions

=head2 Functions

The following functions are exported by default

=head3 adapter_objectCallback_signature

This is the function signature for the objectCallback parameter in adapter
functions. Calling this directly will throw a 'Do not call directly' error.
Signature used for adapter objectCallback

=head4 Parameters

    $object

This is the object from adapter or undefined if there are no more objects to
return.

    $userData

This is the value of the userData parameter when objectCallback was called.

=head3 add_object

Add an adaptive object to an adapter, specified by the adapterId, with a
specified objectType. You may supply an optional objectId, if the underlying
adapterId allows it.
Add adaptive object

=head4 Parameters

    $adapterId

Id of adapter.

    $objectType

Id of adaptive object type of object being added.

    $object

Object to add.

    $objectId

Optional preferred objectId of object to add. The adapter may ignore this.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_add_object

where ${adapterType} is the adapter type id.

=head3 add_object_with_uri

Add an adaptive object with a given URI.
Add adaptive object with URI

=head4 Parameters

    $uri

URI of object to add. If a URI begins with a single slash ('/'), it is the
local object path. In the case of a local path, an optional preferred objectId
of object can be including in URI which the adapter may ignore.

    $object

Object to add.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_add_object

Where ${adapterType} is the adapter type id.

=head3 convert_AdaptiveQueryCriteria_to_query_string

This function converts an _AdaptiveQueryCriteria_ object to a query string.
Convert _AdaptiveQueryCriteria_ to RQL

=head4 Parameters

    $queryCriteria

An object type _AdaptiveQueryCriteria_ object.

    $adapterId

This is the adapterId of adapter containing the objectType for validating and
mapping this query. If not specified, no validation or mapping will occur.

    $objectType

This is the objectType used in conjunction with the adapterId parameter. This
parameter is only allowed and is required if the adapterId parameter is
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

    $adapterId

This is the adapterId of adapter containing the objectType for validating and
mapping this query. If not specified, no validation or mapping will occur.

    $objectType

This is the objectType used in conjunction with the adapterId parameter. This
parameter is only allowed and is required if the adapterId parameter is
specified.

=head3 delete_object

Delete an adaptive object.
Delete adaptive object

=head4 Parameters

    $adapterId

Id of adapter containing object to delete.

    $objectType

Id of adaptive object type of object to delete.

    $objectId

Id of object to delete.

    $journal

The properties of this object will be added to the associated journal entry.
Refer to /afw/_AdaptiveObjectType_/_AdaptiveJournalEntry_ for property names
to avoid and for ones that have specific semantics.

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_delete_object

where ${adapterType} is the adapter type id.

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

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_delete_object

where ${adapterType} is the adapter type id.

=head3 get_object

Get an adaptive object from the adapter, specified by adapterId, objectType
and objectId. Optional view options and adapter-specific options may be
supplied to influence how the object is returned.
Get adaptive object

=head4 Parameters

    $adapterId

Id of adapter containing object to retrieve.

    $objectType

Id of adaptive object type of object to retrieve.

    $objectId

Id of object to retrieve.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_get_object

Where ${adapterType} is the adapter type id.

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

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_get_object

where ${adapterType} is the adapter type id.

=head3 modify_object

Modify an adaptive object.
Modify adaptive object

=head4 Parameters

    $adapterId

Id of adapter containing object to modify.

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

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_modify_object

Where ${adapterType} is the adapter type id.

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

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_modify_object

Where ${adapterType} is the adapter type id.

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

    $adapterId

Id of adapter containing object to replace.

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

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_replace_object

Where ${adapterType} is the adapter type id.

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

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_replace_object

Where ${adapterType} is the adapter type id.

=head3 retrieve_objects

This function retrieves adaptive objects from an adapter, specified by
adapterId, which match the type specified by objectType.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adapterId, can be optionally supplied.
Retrieve adaptive objects

=head4 Parameters

    $adapterId

Id of adapter containing objects to retrieve.

    $objectType

Id of adaptive object type of objects to retrieve.

    $queryCriteria

This is the query criteria for objects to be retrieved. If not specified, all
objects will be retrieved.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_retrieve_objects

Where ${adapterType} is the adapter type id.

=head3 retrieve_objects_to_callback

This function retrieves adaptive objects from an adapter, specified by
adapterId, which match the type specified by objectType.

Each object is passed to the callback as it's retrieved.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adapterId, can be optionally supplied.
Retrieve adaptive objects

=head4 Parameters

    $objectCallback

If this is specified, this function is called once for each object retrieved
instead of adding the object to the return array. Parameter object will be an
object retrieved or undefined if there are no more objects. This function
should return true if it wants to abort the retrieve request.

    $userData

This value is passed to the objectCallback function in the userData parameter.

    $adapterId

Id of adapter containing objects to retrieve.

    $objectType

Id of adaptive object type of objects to retrieve.

    $queryCriteria

This is the query criteria for objects to be retrieved. If not specified, all
objects will be retrieved.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_retrieve_objects_to_callback

Where ${adapterType} is the adapter type id.

=head3 retrieve_objects_to_response

This function retrieves adaptive objects from an adapter, specified by
adapterId, which match the type specified by objectType.

This function is only allowed when called during a request with content type
'application/x-afw'. An '_AdaptiveResponse_' object is written to the
request's response as each object is retrieved. This '_AdaptiveResponse_'
object has a 'result' property containing the retrieved object and a
'intermediate' property set to true.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adapterId, can be optionally supplied.
Retrieve adaptive objects to a response

=head4 Parameters

    $adapterId

Id of adapter containing objects to retrieve.

    $objectType

Id of adaptive object type of objects to retrieve.

    $queryCriteria

This is the query criteria for objects to be retrieved. If not specified, all
objects will be retrieved.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_retrieve_objects

Where ${adapterType} is the adapter type id.

=head3 retrieve_objects_to_stream

This function retrieves adaptive objects from an adapter, specified by
adapterId, which match the type specified by objectType.

Each object is written to the 'streamId' stream as it's retrieved.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adapterId, can be optionally supplied.
Retrieve adaptive objects to a stream

=head4 Parameters

    $streamNumber

Stream number objects will be written to as they are retrieved.

    $adapterId

Id of adapter containing objects to retrieve.

    $objectType

Id of adaptive object type of objects to retrieve.

    $queryCriteria

This is the query criteria for objects to be retrieved. If not specified, all
objects will be retrieved.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_retrieve_objects

Where ${adapterType} is the adapter type id.

=head3 retrieve_objects_with_uri

Retrieve adaptive objects with a given URI.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adapterId, can be optionally supplied.
Retrieve adaptive object with URI

=head4 Parameters

    $uri

URI of objects to retrieve. If a URI begins with a single slash ('/'), it is
the local object path. A query string can be specified.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_retrieve_objects

Where ${adapterType} is the adapter type id.

=head3 retrieve_objects_with_uri_to_callback

Retrieve adaptive objects with a given URI.

Each object is passed to the callback as it's retrieved.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adapterId, can be optionally supplied.
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

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_retrieve_objects

Where ${adapterType} is the adapter type id.

=head3 retrieve_objects_with_uri_to_response

This function retrieves adaptive objects with a given URI.

This function is only allowed when called during a request with content type
'application/x-afw'. An '_AdaptiveResponse_' object is written to the
request's response as each object is retrieved. This '_AdaptiveResponse_'
object has a 'result' property containing the retrieved object and a 'partial'
property set to true.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adapterId, can be optionally supplied.
Retrieve adaptive object with URI to response

=head4 Parameters

    $uri

URI of objects to retrieve. If a URI begins with a single slash ('/'), it is
the local object path. A query string can be specified.

    $options

Object view options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for
more information.

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_retrieve_objects

Where ${adapterType} is the adapter type id.

=head3 retrieve_objects_with_uri_to_stream

This function retrieves adaptive objects with a given URI.

Each object is written to the 'streamId' stream as it's retrieved.

The optional queryCriteria is used to filter the adaptive objects returned.
Use the objectOptions parameter to influence how the objects are viewed.

Options, specific to the adapterId, can be optionally supplied.
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

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_retrieve_objects

Where ${adapterType} is the adapter type id.

=head3 update_object

Update an adaptive object.
Update adaptive object

=head4 Parameters

    $adapterId

Id of adapter containing object to update.

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

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_modify_object

Where ${adapterType} is the adapter type id.

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

    $adapterTypeSpecific

This is an optional object parameter with an objectType determined by the
adapterType associated with the adapterId parameter. If the adapterType
supports this parameter, adapter afw will have an adaptive object type with an
id of:

_AdaptiveAdapterTypeSpecific_${adapterType}_modify_object

Where ${adapterType} is the adapter type id.

=cut

sub adapter_objectCallback_signature {
    my ($object, $userData) = @_;

    my $request = $session->request()

    $request->set("function" => "adapter_objectCallback_signature");
    $request->set("object", $object);

    if (defined $userData)
        $request->set("userData", $userData);

    return $request->getResult();
}

sub add_object {
    my ($adapterId, $objectType, $object, $objectId, $journal, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "add_object");
    $request->set("objectType", $objectType);
    $request->set("object", $object);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $objectId)
        $request->set("objectId", $objectId);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub add_object_with_uri {
    my ($uri, $object, $journal, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "add_object_with_uri");
    $request->set("uri", $uri);
    $request->set("object", $object);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub convert_AdaptiveQueryCriteria_to_query_string {
    my ($queryCriteria, $adapterId, $objectType, $style) = @_;

    my $request = $session->request()

    $request->set("function" => "convert_AdaptiveQueryCriteria_to_query_string");
    $request->set("queryCriteria", $queryCriteria);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $objectType)
        $request->set("objectType", $objectType);

    if (defined $style)
        $request->set("style", $style);

    return $request->getResult();
}

sub convert_query_string_to_AdaptiveQueryCriteria {
    my ($queryString, $adapterId, $objectType) = @_;

    my $request = $session->request()

    $request->set("function" => "convert_query_string_to_AdaptiveQueryCriteria");
    $request->set("queryString", $queryString);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $objectType)
        $request->set("objectType", $objectType);

    return $request->getResult();
}

sub delete_object {
    my ($adapterId, $objectType, $objectId, $journal, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "delete_object");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub delete_object_with_uri {
    my ($uri, $journal, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "delete_object_with_uri");
    $request->set("uri", $uri);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub get_object {
    my ($adapterId, $objectType, $objectId, $options, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "get_object");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub get_object_with_uri {
    my ($uri, $options, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "get_object_with_uri");
    $request->set("uri", $uri);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub modify_object {
    my ($adapterId, $objectType, $objectId, $entries, $journal, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "modify_object");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);
    $request->set("entries", $entries);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub modify_object_with_uri {
    my ($uri, $entries, $journal, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "modify_object_with_uri");
    $request->set("uri", $uri);
    $request->set("entries", $entries);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

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
    my ($adapterId, $objectType, $objectId, $object, $journal, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "replace_object");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);
    $request->set("object", $object);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub replace_object_with_uri {
    my ($uri, $object, $journal, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "replace_object_with_uri");
    $request->set("uri", $uri);
    $request->set("object", $object);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects {
    my ($adapterId, $objectType, $queryCriteria, $options, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects");
    $request->set("objectType", $objectType);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $queryCriteria)
        $request->set("queryCriteria", $queryCriteria);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_to_callback {
    my ($objectCallback, $userData, $adapterId, $objectType, $queryCriteria, $options, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_to_callback");
    $request->set("objectCallback", $objectCallback);
    $request->set("userData", $userData);
    $request->set("objectType", $objectType);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $queryCriteria)
        $request->set("queryCriteria", $queryCriteria);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_to_response {
    my ($adapterId, $objectType, $queryCriteria, $options, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_to_response");
    $request->set("objectType", $objectType);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $queryCriteria)
        $request->set("queryCriteria", $queryCriteria);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_to_stream {
    my ($streamNumber, $adapterId, $objectType, $queryCriteria, $options, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_to_stream");
    $request->set("streamNumber", $streamNumber);
    $request->set("objectType", $objectType);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $queryCriteria)
        $request->set("queryCriteria", $queryCriteria);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_with_uri {
    my ($uri, $options, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_with_uri");
    $request->set("uri", $uri);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_with_uri_to_callback {
    my ($objectCallback, $userData, $uri, $options, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_with_uri_to_callback");
    $request->set("objectCallback", $objectCallback);
    $request->set("userData", $userData);
    $request->set("uri", $uri);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_with_uri_to_response {
    my ($uri, $options, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_with_uri_to_response");
    $request->set("uri", $uri);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub retrieve_objects_with_uri_to_stream {
    my ($streamNumber, $uri, $options, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "retrieve_objects_with_uri_to_stream");
    $request->set("streamNumber", $streamNumber);
    $request->set("uri", $uri);

    if (defined $options)
        $request->set("options", $options);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub update_object {
    my ($adapterId, $objectType, $objectId, $object, $journal, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "update_object");
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);
    $request->set("object", $object);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

sub update_object_with_uri {
    my ($uri, $object, $journal, $adapterTypeSpecific) = @_;

    my $request = $session->request()

    $request->set("function" => "update_object_with_uri");
    $request->set("uri", $uri);
    $request->set("object", $object);

    if (defined $journal)
        $request->set("journal", $journal);

    if (defined $adapterTypeSpecific)
        $request->set("adapterTypeSpecific", $adapterTypeSpecific);

    return $request->getResult();
}

42;
