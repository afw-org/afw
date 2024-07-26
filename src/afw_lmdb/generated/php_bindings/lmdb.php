<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for lmdb
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file lmdb.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category lmdb
 */
class lmdb
{
    private $session;

    /**
     * Constructs the lmdb class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * reader_check()
     *
     * Check for stale entries in the reader lock table.
     *
     * @param string $adapterId Adapter Id.
     *
     * @return integer Number of stale slots that were cleared.
     */
    public function reader_check(, $adapterId = null)
    {
        $request = $this->$session->request();

        $request->set("function", "reader_check");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($adapterId != null)
            $request->set('adapterId', $adapterId);

        return $request->get_result();
    }

    /**
     * reader_list()
     *
     * Dump the entries in the reader lock table.
     *
     * @param string $adapterId Adapter Id.
     *
     * @return string Reader entries.
     */
    public function reader_list(, $adapterId = null)
    {
        $request = $this->$session->request();

        $request->set("function", "reader_list");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($adapterId != null)
            $request->set('adapterId', $adapterId);

        return $request->get_result();
    }

}

?>
