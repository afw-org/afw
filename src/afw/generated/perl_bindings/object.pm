#!/usr/bin/env perl

#**
# @file object.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the object category.
#*

package afw::object;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    add_properties 
    apply_object_options 
    bag_object 
    bag_size_object 
    clone_object 
    entries 
    eq_object 
    eqx_object 
    freeze_object 
    ge_object 
    gt_object 
    is_object 
    keys 
    le_object 
    local_object_meta_set_ids 
    lt_object 
    ne_object 
    nex_object 
    object 
    property_delete 
    property_delete_by_reference 
    property_exists 
    property_get 
    property_is_not_null 
    to_string_object 
    values 
);

=head1 NAME

afw::object - Perl module for object functions

=head2 Functions

The following functions are exported by default

=head3 add_properties

Add the properties of one object to another replacing existing properties by
the same name.
Add properties of one object to another

=head4 Parameters

    $target

Target object or undefined if result will only contain the properties of
source objects. The object must be mutable, if specified.

    $source

Source object(s).

=head3 apply_object_options

This will return an object with the specified object options applied.
Apply object options

=head4 Parameters

    $object

Object used to produce result.

    $options

Object options. See /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_ for more
information.

=head3 bag_object

Takes any number of object values and returns an array of array.
Makes an array from values

=head4 Parameters

    $values


=head3 bag_size_object

This returns the integer number of values in array.
Returns the number of values in a bag

=head4 Parameters

    $value


=head3 clone_object

Deep clone a object value.
Clone object value

=head4 Parameters

    $value

The object value to clone.

=head3 entries

Return a new array of property entries for an object. Each entry is a
two-element array [name, value] where name is a string. Order matches keys()
for the same object. The value may be undefined. The result is a snapshot.
Property name and value pairs of an object

=head4 Parameters

    $object

Object to list property entries from.

=head3 eq_object

Determine if object arg1 is equal to the value of arg2 converted to the data
type of arg1 then return the boolean result. Use 'eqx' ('===') instead if you
want false to be returned if arg1 and arg2's data type don't match.
Checks for equal

=head4 Parameters

    $arg1


    $arg2


=head3 eqx_object

Determine if for object arg1 is equal to the value and data type of arg2 then
return the boolean result. Use 'eq' ('==') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for equal and type

=head4 Parameters

    $arg1


    $arg2


=head3 freeze_object

Set a object value immutable so further mutation throws. If already immutable,
has no effect. Returns the same value.
Make object value immutable

=head4 Parameters

    $value

The object value to freeze.

=head3 ge_object

Checks for object arg1 is greater than or equal to object arg2 and return the
boolean result.
Checks for greater than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 gt_object

Checks for object arg1 is greater than object arg2 and return the boolean
result.
Checks for greater than

=head4 Parameters

    $arg1


    $arg2


=head3 is_object

Checks whether value is dataType object and return the boolean result.
Checks whether value is dataType object

=head4 Parameters

    $value

Value to check

=head3 keys

Return a new array of the property names of an object, in the object's
property iteration order. The array is a snapshot; later changes to the object
do not change a previous result.
Property names of an object

=head4 Parameters

    $object

Object to list property names from.

=head3 le_object

Checks for object arg1 is less than or equal to object arg2 and return the
boolean result.
Checks for less than or equal

=head4 Parameters

    $arg1


    $arg2


=head3 local_object_meta_set_ids

This is used to set the ids in a local mutable object. The ids are used to
construct a local path.
Set ids in a local mutable object

=head4 Parameters

    $object

Object to set ids in.

    $adapterId

Adapter id for object.

    $objectType

Object type id for object.

    $objectId

Object id for object.

=head3 lt_object

Checks for object arg1 is less that object arg2 and return the boolean result.
Checks for less than

=head4 Parameters

    $arg1


    $arg2


=head3 ne_object

Determine if object arg1 is not equal to the value of arg2 converted to the
data type of arg1 then return the boolean result. Use 'nex' ('!==') instead if
you want true to be returned if arg1 and arg2's data type don't match.
Checks for not equal

=head4 Parameters

    $arg1


    $arg2


=head3 nex_object

Determine if for object arg1 is not equal to the value or data type of arg2
then return the boolean result. Use 'ne' ('!=') instead if you want arg2 to be
converted to the data type of arg1 before comparison.
Checks for not equal value or type

=head4 Parameters

    $arg1


    $arg2


=head3 object

Converts value to data type object returning object result. A string is parsed
as JSON (or relaxed JSON) and must yield an object; an object is left
unchanged. This is not an object-literal constructor — use { ... } for that.
Convert to data type object

=head4 Parameters

    $value

Value to convert

=head3 property_delete

Delete a property in an object.
Delete a property

=head4 Parameters

    $object

This is the object to delete property from.

    $name

This is a name of the property to delete.

=head3 property_delete_by_reference

Delete a property from an object by reference.
Delete referenced property

=head4 Parameters

    $reference

This is a reference to the object property to delete.

=head3 property_exists

Return true if the named property is present on the object, including when its
value is undefined or null. False only when the key is missing. Use is_defined
/ is_nullish for the value.
True if a property is present on an object

=head4 Parameters

    $object

Object to check.

    $name

Property name.

=head3 property_get

Return the value of a property. Optional default applies only when the
property is missing — not when the value is undefined. If missing and no
default is given, the result is undefined. The default is the evaluated value
at that moment (identity).
Get a property value

=head4 Parameters

    $object

Object to get property from.

    $name

Property name.

    $defaultValue

Value to return only if the property is missing. The evaluated value at that
moment (identity).

=head3 property_is_not_null

Return true if the named property is present and its value is not Adaptive
null. Undefined counts as not null. False if the property is missing or the
value is null. Not the same as is_defined or not is_nullish.
True if property present and not Adaptive null

=head4 Parameters

    $object

Object to check.

    $name

Property name.

=head3 to_string_object

Converts object value to string. For array values, the to_string() value for
each entry is returned separated with commas.
Converts value to string

=head4 Parameters

    $value

A object value.

=head3 values

Return a new array of the property values of an object, in the same order as
keys() for that object. Values may be undefined if a property was set to
undefined. The array is a snapshot.
Property values of an object

=head4 Parameters

    $object

Object to list property values from.

=cut

sub add_properties {
    my ($target, $source) = @_;

    my $request = $session->request()

    $request->set("function" => "add_properties");
    $request->set("target", $target);
    $request->set("source", $source);

    return $request->getResult();
}

sub apply_object_options {
    my ($object, $options) = @_;

    my $request = $session->request()

    $request->set("function" => "apply_object_options");
    $request->set("object", $object);

    if (defined $options)
        $request->set("options", $options);

    return $request->getResult();
}

sub bag_object {
    my ($values) = @_;

    my $request = $session->request()

    $request->set("function" => "bag<object>");

    if (defined $values)
        $request->set("values", $values);

    return $request->getResult();
}

sub bag_size_object {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "bag_size<object>");
    $request->set("value", $value);

    return $request->getResult();
}

sub clone_object {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "clone<object>");
    $request->set("value", $value);

    return $request->getResult();
}

sub entries {
    my ($object) = @_;

    my $request = $session->request()

    $request->set("function" => "entries");
    $request->set("object", $object);

    return $request->getResult();
}

sub eq_object {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eq<object>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub eqx_object {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "eqx<object>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub freeze_object {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "freeze<object>");
    $request->set("value", $value);

    return $request->getResult();
}

sub ge_object {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ge<object>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub gt_object {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "gt<object>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub is_object {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "is<object>");
    $request->set("value", $value);

    return $request->getResult();
}

sub keys {
    my ($object) = @_;

    my $request = $session->request()

    $request->set("function" => "keys");
    $request->set("object", $object);

    return $request->getResult();
}

sub le_object {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "le<object>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub local_object_meta_set_ids {
    my ($object, $adapterId, $objectType, $objectId) = @_;

    my $request = $session->request()

    $request->set("function" => "local_object_meta_set_ids");
    $request->set("object", $object);
    $request->set("objectType", $objectType);
    $request->set("objectId", $objectId);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    return $request->getResult();
}

sub lt_object {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "lt<object>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub ne_object {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "ne<object>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub nex_object {
    my ($arg1, $arg2) = @_;

    my $request = $session->request()

    $request->set("function" => "nex<object>");
    $request->set("arg1", $arg1);
    $request->set("arg2", $arg2);

    return $request->getResult();
}

sub object {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "object");
    $request->set("value", $value);

    return $request->getResult();
}

sub property_delete {
    my ($object, $name) = @_;

    my $request = $session->request()

    $request->set("function" => "property_delete");
    $request->set("object", $object);
    $request->set("name", $name);

    return $request->getResult();
}

sub property_delete_by_reference {
    my ($reference) = @_;

    my $request = $session->request()

    $request->set("function" => "property_delete_by_reference");
    $request->set("reference", $reference);

    return $request->getResult();
}

sub property_exists {
    my ($object, $name) = @_;

    my $request = $session->request()

    $request->set("function" => "property_exists");
    $request->set("object", $object);
    $request->set("name", $name);

    return $request->getResult();
}

sub property_get {
    my ($object, $name, $defaultValue) = @_;

    my $request = $session->request()

    $request->set("function" => "property_get");
    $request->set("object", $object);
    $request->set("name", $name);

    if (defined $defaultValue)
        $request->set("defaultValue", $defaultValue);

    return $request->getResult();
}

sub property_is_not_null {
    my ($object, $name) = @_;

    my $request = $session->request()

    $request->set("function" => "property_is_not_null");
    $request->set("object", $object);
    $request->set("name", $name);

    return $request->getResult();
}

sub to_string_object {
    my ($value) = @_;

    my $request = $session->request()

    $request->set("function" => "to_string<object>");
    $request->set("value", $value);

    return $request->getResult();
}

sub values {
    my ($object) = @_;

    my $request = $session->request()

    $request->set("function" => "values");
    $request->set("object", $object);

    return $request->getResult();
}

42;
