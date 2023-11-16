<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for random
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file random.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category random
 */
class random
{
    private $session;

    /**
     * Constructs the random class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * random_base64Binary()
     *
     * This returns a specified number of random octets as dataType
     * base64Binary.
     *
     * @param integer $numberOfOctets The number of random octets to generate.
     *
     * @return base64Binary
     */
    public function random_base64Binary(, $numberOfOctets)
    {
        $request = $this->$session->request();

        $request->set("function", "random_base64Binary");

        /* pass along required parameters to the request payload */
        $request->set("numberOfOctets", $numberOfOctets);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * random_digits()
     *
     * Generate a string containing a specified number of random decimal
     * digits.
     *
     * @param integer $numberOfDigits The number (1-18) of digits to generate.
     *
     * @return string
     */
    public function random_digits(, $numberOfDigits)
    {
        $request = $this->$session->request();

        $request->set("function", "random_digits");

        /* pass along required parameters to the request payload */
        $request->set("numberOfDigits", $numberOfDigits);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * random_hexBinary()
     *
     * This returns a specified number of random octets as dataType hexBinary.
     *
     * @param integer $numberOfOctets The number of random octets to generate.
     *
     * @return hexBinary
     */
    public function random_hexBinary(, $numberOfOctets)
    {
        $request = $this->$session->request();

        $request->set("function", "random_hexBinary");

        /* pass along required parameters to the request payload */
        $request->set("numberOfOctets", $numberOfOctets);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * random_integer()
     *
     * This returns a random integer between specified values inclusive.
     *
     * @param integer $min Minimum integer inclusive.
     * @param integer $max Maximum integer inclusive.
     *
     * @return integer A random integer.
     */
    public function random_integer(, $min, $max)
    {
        $request = $this->$session->request();

        $request->set("function", "random_integer");

        /* pass along required parameters to the request payload */
        $request->set("min", $min);
        $request->set("max", $max);

        /* pass along any optional parameters to the request payload */
        return $request->get_result();
    }

    /**
     * random_number()
     *
     * This returns a random double between specified values
     *
     * @param double $min Minimum double inclusive. The default is 0.0.
     * @param double $max Maximum double exclusive. The default is 1.0.
     *
     * @return double
     */
    public function random_number(, $min = null, $max = null)
    {
        $request = $this->$session->request();

        $request->set("function", "random_number");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($min != null)
            $request->set('min', $min);

        if ($max != null)
            $request->set('max', $max);

        return $request->get_result();
    }

}

?>
