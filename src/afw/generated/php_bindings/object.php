<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for object
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file object.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category object
 */
class object
{
    private $session;

    /**
     * Constructs the object class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * add_properties()
     *
     * Add the properties of one object to another replacing existing
     * properties by the same name.
     *
     * @param object $target Target object or undefined if result will only
     *                       contain the properties of source objects. The
     *                       object must be mutable, if specified.
     * @param object $source Source object(s).
     *
     * @return object The resulting object.
     */
    public function add_properties(, $target, $source)
    {
        $request = $this->$session->request();

        $request->set("function", "add_properties");

        /* pass along required parameters to the request payload */
        $request->set("target", $target);
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * apply_object_options()
     *
     * This will return an object with the specified object options applied.
     *
     * @param object $object Object used to produce result.
     * @param object $options Object options. See
     *                        /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions_
     *                        for more information.
     *
     * @return object This is an object with object options applied.
     */
    public function apply_object_options(, $object, $options = null)
    {
        $request = $this->$session->request();

        $request->set("function", "apply_object_options");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);

        /* pass along any optional parameters to the request payload */
        if ($options != null)
            $request->set('options', $options);

        return $request->get_result();
    }

    /**
     * array_object()
     *
     * Construct a new array of object from the given values, the same as
     * array() except that the new array is typed: each value, and each entry
     * of a spread array, must be dataType object, and adding a value of
     * another data type to the array later throws. A value of another data
     * type is an error, never converted. This is usually written
     * `array<object>(`...values).
     *
     * @param $values A value of dataType object, or ... followed by an
     *                 expression that results in an array, every entry of
     *                 which is included.
     *
     * @return array The constructed array of object.
     */
    public function array_object(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array_object");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * as_object()
     *
     * Returns value unchanged if it is dataType object. Otherwise, including
     * when value is undefined, throws an error. It never converts: to get a
     * object from a value of another data type, write a typed literal or call
     * a to_T conversion function where one exists. This is usually written
     * value as object.
     *
     * @param $value Value to assert
     *
     * @return object value, which is dataType object
     */
    public function as_object(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_object");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * clone_object()
     *
     * Deep clone a object value.
     *
     * @param object $value The object value to clone.
     *
     * @return object The cloned object value.
     */
    public function clone_object(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "clone<object>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * entries()
     *
     * Return a new array of [key, value] pairs: for an object, its property
     * names and values; for an array, its indexes and entries, as ECMAScript
     * entries does. Order matches keys(). A value may be undefined. The
     * result is a snapshot.
     *
     * @param $object The object or array.
     *
     * @return array Array of [key, value] pair arrays.
     */
    public function entries(, $object)
    {
        $request = $this->$session->request();

        $request->set("function", "entries");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eq_object()
     *
     * Determine if object arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param object $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eq_object(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<object>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_object()
     *
     * Determine if for object arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param object $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eqx_object(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<object>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * freeze_object()
     *
     * Set a object value immutable so further mutation throws. If already
     * immutable, has no effect. Returns the same value.
     *
     * @param object $value The object value to freeze.
     *
     * @return object The same value, now immutable.
     */
    public function freeze_object(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "freeze<object>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_object()
     *
     * Checks for object arg1 is greater than or equal to object arg2 and
     * return the boolean result.
     *
     * @param object $arg1
     * @param object $arg2
     *
     * @return boolean
     */
    public function ge_object(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<object>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_object()
     *
     * Checks for object arg1 is greater than object arg2 and return the
     * boolean result.
     *
     * @param object $arg1
     * @param object $arg2
     *
     * @return boolean
     */
    public function gt_object(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<object>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_object()
     *
     * Checks whether value is dataType object and returns the boolean result.
     * It is false for undefined. This can also be written
     * `is<object>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_object(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_object");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * keys()
     *
     * Return a new array of the keys: for an object, its property names
     * (strings) in its property iteration order; for an array, its indexes
     * (integers), as ECMAScript keys does. The result is a snapshot; later
     * changes do not change it.
     *
     * @param $object The object or array.
     *
     * @return array Array of property name strings, or of integer indexes.
     */
    public function keys(, $object)
    {
        $request = $this->$session->request();

        $request->set("function", "keys");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_object()
     *
     * Checks for object arg1 is less than or equal to object arg2 and return
     * the boolean result.
     *
     * @param object $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function le_object(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<object>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * local_object_meta_set_ids()
     *
     * This is used to set the ids in a local mutable object. The ids are used
     * to construct a local path.
     *
     * @param object $object Object to set ids in.
     * @param string $adapterId Adapter id for object.
     * @param string $objectType Object type id for object.
     * @param string $objectId Object id for object.
     *
     * @return void
     */
    public function local_object_meta_set_ids(, $object, $objectType, $objectId, $adapterId = null)
    {
        $request = $this->$session->request();

        $request->set("function", "local_object_meta_set_ids");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);
        $request->set("objectType", $objectType);
        $request->set("objectId", $objectId);

        /* pass along any optional parameters to the request payload */
        if ($adapterId != null)
            $request->set('adapterId', $adapterId);

        return $request->get_result();
    }

    /**
     * lt_object()
     *
     * Checks for object arg1 is less that object arg2 and return the boolean
     * result.
     *
     * @param object $arg1
     * @param object $arg2
     *
     * @return boolean
     */
    public function lt_object(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<object>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_object()
     *
     * Determine if object arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
     * instead if you want true to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param object $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function ne_object(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<object>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_object()
     *
     * Determine if for object arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param object $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function nex_object(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<object>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * object()
     *
     * Converts value to data type object returning object result. A string is
     * parsed as JSON (or relaxed JSON) and must yield an object; an object is
     * left unchanged. This is not an object-literal constructor — use { ... }
     * for that.
     *
     * @param $value Value to convert
     *
     * @return object Converted value
     */
    public function object(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "object");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * property_delete()
     *
     * Set a property's value to undefined. The name stays on the object.
     *
     * @param object $object This is the object.
     * @param string $name This is the property name.
     *
     * @return boolean True if the object had the property. Its value is now
     *                 undefined.
     */
    public function property_delete(, $object, $name)
    {
        $request = $this->$session->request();

        $request->set("function", "property_delete");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);
        $request->set("name", $name);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * property_delete_by_reference()
     *
     * Set a referenced property's value to undefined. The name stays on the
     * object.
     *
     * @param $reference This is a reference to the object property.
     *
     * @return boolean True if the object had the property. Its value is now
     *                 undefined.
     */
    public function property_delete_by_reference(, $reference)
    {
        $request = $this->$session->request();

        $request->set("function", "property_delete_by_reference");

        /* pass along required parameters to the request payload */
        $request->set("reference", $reference);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * property_exists()
     *
     * Return true if the named property is present on the object, including
     * when its value is undefined or null. False only when the key is
     * missing. Use is_defined / is_nullish for the value.
     *
     * @param object $object Object to check.
     * @param string $name Property name.
     *
     * @return boolean True if the property is present.
     */
    public function property_exists(, $object, $name)
    {
        $request = $this->$session->request();

        $request->set("function", "property_exists");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);
        $request->set("name", $name);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * property_get()
     *
     * Return the value of a property. Optional default applies only when the
     * property is missing — not when the value is undefined. If missing and
     * no default is given, the result is undefined. The default is the
     * evaluated value at that moment (identity).
     *
     * @param object $object Object to get property from.
     * @param string $name Property name.
     * @param $defaultValue Value to return only if the property is missing.
     *                       The evaluated value at that moment (identity).
     *
     * @return Property value, or default / undefined if missing.
     */
    public function property_get(, $object, $name, $defaultValue = null)
    {
        $request = $this->$session->request();

        $request->set("function", "property_get");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);
        $request->set("name", $name);

        /* pass along any optional parameters to the request payload */
        if ($defaultValue != null)
            $request->set('defaultValue', $defaultValue);

        return $request->get_result();
    }

    /**
     * property_is_not_null()
     *
     * Return true if the named property is present and its value is not
     * Adaptive null. Undefined counts as not null. False if the property is
     * missing or the value is null. Not the same as is_defined or not
     * is_nullish.
     *
     * @param object $object Object to check.
     * @param string $name Property name.
     *
     * @return boolean True if present and value is not Adaptive null.
     */
    public function property_is_not_null(, $object, $name)
    {
        $request = $this->$session->request();

        $request->set("function", "property_is_not_null");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);
        $request->set("name", $name);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_object()
     *
     * Converts object value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param object $value A object value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_object(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<object>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * values()
     *
     * Return a new array of the values: for an object, its property values in
     * the same order as keys(); for an array, its entries, as ECMAScript
     * values does. A value may be undefined. The result is a snapshot.
     *
     * @param $object The object or array.
     *
     * @return array Array of values.
     */
    public function values(, $object)
    {
        $request = $this->$session->request();

        $request->set("function", "values");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
