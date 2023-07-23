<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for object
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category object
 */
class object
{
    private $session;

    /**
     * Constructs the object class, using a session.
     *
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
     * @param object $target Target object. This object must not be
     *                       immutable.
     * @param object $source Source object(s).
     *
     * @return object The modified target object.
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
     *                        /afw/_AdaptiveObjectType_/_AdaptiveObjectOptions
     *                        _ for more information.
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
     * bag_object()
     *
     * Takes any number of object values and returns a list of list.
     *
     * @param list $values
     *
     * @return list
     */
    public function bag_object(, $values = null)
    {
        $request = $this->$session->request();

        $request->set("function", "bag<object>");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($values != null)
            $request->set('values', $values);

        return $request->get_result();
    }

    /**
     * bag_size_object()
     *
     * This returns the integer number of values in list.
     *
     * @param list $value
     *
     * @return integer
     */
    public function bag_size_object(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "bag_size<object>");

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
     * eq_object()
     *
     * Determine if object arg1 is equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use "eqx"
     * ("===") instead if you want false to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param object $arg1
     * @param  $arg2
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
     * arg2 then return the boolean result. Use "eq" ("==") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param object $arg1
     * @param  $arg2
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
     * Checks whether value is dataType object and return the boolean result.
     *
     * @param  $value Value to check
     *
     * @return boolean
     */
    public function is_object(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is<object>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

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
     * @param  $arg2
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
     * This is used to set the ids in a local mutable object. The ids are
     * used to construct a local path.
     *
     * @param object $object Object to set ids in.
     * @param string $adaptorId Adaptor id for object.
     * @param string $objectType Object type id for object.
     * @param string $objectId Object id for object.
     *
     * @return null
     */
    public function local_object_meta_set_ids(, $object, $objectType, $objectId, $adaptorId = null)
    {
        $request = $this->$session->request();

        $request->set("function", "local_object_meta_set_ids");

        /* pass along required parameters to the request payload */
        $request->set("object", $object);
        $request->set("objectType", $objectType);
        $request->set("objectId", $objectId);

        /* pass along any optional parameters to the request payload */
        if ($adaptorId != null)
            $request->set('adaptorId', $adaptorId);

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
     * Determine if object arg1 is not equal to the value of arg2 converted
     * to the data type of arg1 then return the boolean result. Use "nex"
     * ("!==") instead if you want true to be returned if arg1 and arg2's
     * data type don't match.
     *
     * @param object $arg1
     * @param  $arg2
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
     * arg2 then return the boolean result. Use "ne" ("!=") instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param object $arg1
     * @param  $arg2
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
     * Converts value to data type object returning object result.
     *
     * @param  $value Value to convert
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
     * Delete a property in an object.
     *
     * @param object $object This is the object to delete property from.
     * @param string $name This is a name of the property to delete.
     *
     * @return boolean True if object had the property and it was deleted.
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
     * Delete a property from an object by reference.
     *
     * @param  $reference This is a reference to the object property to
     *                    delete.
     *
     * @return boolean True if object had the property and it was deleted.
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
     * Return true if the named property exists in an object.
     *
     * @param object $object Object to get property from.
     * @param string $name Name of property to check.
     *
     * @return boolean True if object has named property.
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
     * Return the value of a property of an object. If property is not
     * available, return a default or null value.
     *
     * @param object $object Object to get property from.
     * @param string $name Name of property to get.
     * @param  $defaultValue The default value of property if it does not
     *                       exist in object. If not specified, null value is
     *                       the default.
     *
     * @return  Evaluated property value or default.
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
     * Return true if the named property exists in an object and is not null.
     *
     * @param object $object Object to get property from.
     * @param string $name Name of property to check.
     *
     * @return boolean True if object has named property that is not null.
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
     * Converts object value to string. For list values, the to_string()
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

}

?>
