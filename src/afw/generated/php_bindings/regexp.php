<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for regexp
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file regexp.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category regexp
 */
class regexp
{
    private $session;

    /**
     * Constructs the regexp class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * array_regexp()
     *
     * Construct a new array of regexp from the given values, the same as
     * array() except that the new array is typed: each value, and each entry
     * of a spread array, must be dataType regexp, and adding a value of
     * another data type to the array later throws. A value of another data
     * type is an error, never converted. This is usually written
     * `array<regexp>(`...values).
     *
     * @param $values A value of dataType regexp, or ... followed by an
     *                 expression that results in an array, every entry of
     *                 which is included.
     *
     * @return array The constructed array of regexp.
     */
    public function array_regexp(, $values)
    {
        $request = $this->$session->request();

        $request->set("function", "array_regexp");

        /* pass along required parameters to the request payload */
        $request->set("values", $values);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * as_regexp()
     *
     * Returns value unchanged if it is dataType regexp. Otherwise, including
     * when value is undefined, throws an error. It never converts: to get a
     * regexp from a value of another data type, write a typed literal or call
     * a to_T conversion function where one exists. This is usually written
     * value as regexp.
     *
     * @param $value Value to assert
     *
     * @return regexp value, which is dataType regexp
     */
    public function as_regexp(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "as_regexp");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * compile_regexp()
     *
     * Compile regexp value and return either an unevaluated adaptive value or
     * a string containing the compiler listing. The listing is a
     * human-oriented dump (value tree interleaved with source, plus
     * ---Symbols tables) for Fiddle and debugging — not pure JSON (use
     * stringify) and not Adaptive compiled-form text (use decompile).
     *
     * @param regexp $source regexp string to compile
     * @param $listing If specified, a human compiler listing is produced
     *                  instead of an unevaluated compiled value (tree +
     *                  ---Symbols; not recompilable). Use decompile() for
     *                  Adaptive compiled-form text and stringify() for pure
     *                  JSON of evaluated data.
     * 
     *                  This parameter can be an integer between 0 and 10 or a
     *                  string that is used for indentation. If 0 is
     *                  specified, no whitespace is added to the resulting
     *                  string. If 1 through 10 is specified, that number of
     *                  spaces is used.
     *
     * @return unevaluated
     */
    public function compile_regexp(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * eq_regexp()
     *
     * Determine if regexp arg1 is equal to the value of arg2 converted to the
     * data type of arg1 then return the boolean result. Use 'eqx' ('===')
     * instead if you want false to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param regexp $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eq_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eq<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * eqx_regexp()
     *
     * Determine if for regexp arg1 is equal to the value and data type of
     * arg2 then return the boolean result. Use 'eq' ('==') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param regexp $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function eqx_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "eqx<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ge_regexp()
     *
     * Checks for regexp arg1 is greater than or equal to regexp arg2 and
     * return the boolean result.
     *
     * @param regexp $arg1
     * @param regexp $arg2
     *
     * @return boolean
     */
    public function ge_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ge<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * gt_regexp()
     *
     * Checks for regexp arg1 is greater than regexp arg2 and return the
     * boolean result.
     *
     * @param regexp $arg1
     * @param regexp $arg2
     *
     * @return boolean
     */
    public function gt_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "gt<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * is_regexp()
     *
     * Checks whether value is dataType regexp and returns the boolean result.
     * It is false for undefined. This can also be written
     * `is<regexp>(value`).
     *
     * @param $value Value to check
     *
     * @return boolean
     */
    public function is_regexp(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "is_regexp");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * le_regexp()
     *
     * Checks for regexp arg1 is less than or equal to regexp arg2 and return
     * the boolean result.
     *
     * @param regexp $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function le_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "le<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * lt_regexp()
     *
     * Checks for regexp arg1 is less that regexp arg2 and return the boolean
     * result.
     *
     * @param regexp $arg1
     * @param regexp $arg2
     *
     * @return boolean
     */
    public function lt_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "lt<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * ne_regexp()
     *
     * Determine if regexp arg1 is not equal to the value of arg2 converted to
     * the data type of arg1 then return the boolean result. Use 'nex' ('!==')
     * instead if you want true to be returned if arg1 and arg2's data type
     * don't match.
     *
     * @param regexp $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function ne_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "ne<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * nex_regexp()
     *
     * Determine if for regexp arg1 is not equal to the value or data type of
     * arg2 then return the boolean result. Use 'ne' ('!=') instead if you
     * want arg2 to be converted to the data type of arg1 before comparison.
     *
     * @param regexp $arg1
     * @param $arg2
     *
     * @return boolean
     */
    public function nex_regexp(, $arg1, $arg2)
    {
        $request = $this->$session->request();

        $request->set("function", "nex<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("arg1", $arg1);
        $request->set("arg2", $arg2);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * regexp()
     *
     * Converts value to data type regexp returning regexp result. Holds a
     * regular expression source string as a regexp value (does not compile
     * the pattern for matching by itself). Use polymorphic compile when a
     * compiled form is required.
     *
     * @param $value Value to convert
     *
     * @return regexp Converted value
     */
    public function regexp(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "regexp");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * to_string_regexp()
     *
     * Converts regexp value to string. For array values, the to_string()
     * value for each entry is returned separated with commas.
     *
     * @param regexp $value A regexp value.
     *
     * @return string The string representation of the value.
     */
    public function to_string_regexp(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "to_string<regexp>");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
