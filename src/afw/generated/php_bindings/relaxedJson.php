<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for relaxedJson
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file relaxedJson.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category relaxedJson
 */
class relaxedJson
{
    private $session;

    /**
     * Constructs the relaxedJson class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * compile_relaxedJson()
     *
     * Compile relaxedJson value and return either an unevaluated adaptive
     * value or a string containing the compiler listing. The listing is a
     * human-oriented dump (value tree interleaved with source, plus
     * ---Symbols tables) for Fiddle and debugging — not pure JSON (use
     * stringify) and not Adaptive compiled-form text (use decompile).
     *
     * @param relaxedJson $source relaxedJson string to compile
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
    public function compile_relaxedJson(, $source, $listing = null)
    {
        $request = $this->$session->request();

        $request->set("function", "compile<relaxedJson>");

        /* pass along required parameters to the request payload */
        $request->set("source", $source);

        /* pass along any optional parameters to the request payload */
        if ($listing != null)
            $request->set('listing', $listing);

        return $request->get_result();
    }

    /**
     * relaxedJson()
     *
     * Converts value to data type relaxedJson returning relaxedJson result.
     * Holds relaxed JSON source text as a relaxedJson value (does not
     * parse/compile). Use polymorphic compile to compile the source.
     *
     * @param $value Value to convert
     *
     * @return relaxedJson Converted value
     */
    public function relaxedJson(, $value)
    {
        $request = $this->$session->request();

        $request->set("function", "relaxedJson");

        /* pass along required parameters to the request payload */
        $request->set("value", $value);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

}

?>
