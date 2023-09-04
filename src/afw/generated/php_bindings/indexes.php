<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for indexes
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * Class definition for function category indexes
 */
class indexes
{
    private $session;

    /**
     * Constructs the indexes class, using a session.
     *
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * index_create()
     *
     * Create an index definition.
     *
     * @param string $adaptorId Id of adaptor.
     * @param string $key Name of the property index to be created.
     * @param string $value Expression to calculate the index value(s).
     * @param string $objectType Object Type(s) this index may apply to.
     * @param string $filter Expression to determine if this index applies to
     *                       a particular object.
     * @param string $options Indexing options.
     * @param boolean $retroactive Retroactively generate indexes for existing
     *                             objects.
     * @param boolean $test Test create (don't actually perform).
     *
     * @return object Object response from the indexing process
     */
    public function index_create(, $key, $adaptorId = null, $value = null, $objectType = null, $filter = null, $options = null, $retroactive = null, $test = null)
    {
        $request = $this->$session->request();

        $request->set("function", "index_create");

        /* pass along required parameters to the request payload */
        $request->set("key", $key);

        /* pass along any optional parameters to the request payload */
        if ($adaptorId != null)
            $request->set('adaptorId', $adaptorId);

        if ($value != null)
            $request->set('value', $value);

        if ($objectType != null)
            $request->set('objectType', $objectType);

        if ($filter != null)
            $request->set('filter', $filter);

        if ($options != null)
            $request->set('options', $options);

        if ($retroactive != null)
            $request->set('retroactive', $retroactive);

        if ($test != null)
            $request->set('test', $test);

        return $request->get_result();
    }

    /**
     * index_list()
     *
     * List property indexes
     *
     * @param string $adaptorId Id of adaptor.
     * @param string $objectType Id of adaptive object type indexes
     *
     * @return object Object response from the index repair process
     */
    public function index_list(, $adaptorId = null, $objectType = null)
    {
        $request = $this->$session->request();

        $request->set("function", "index_list");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($adaptorId != null)
            $request->set('adaptorId', $adaptorId);

        if ($objectType != null)
            $request->set('objectType', $objectType);

        return $request->get_result();
    }

    /**
     * index_remove()
     *
     * Remove an index definition.
     *
     * @param string $adaptorId Id of adaptor.
     * @param string $key The index key to be removed.
     *
     * @return object Object response from the indexing process
     */
    public function index_remove(, $key, $adaptorId = null)
    {
        $request = $this->$session->request();

        $request->set("function", "index_remove");

        /* pass along required parameters to the request payload */
        $request->set("key", $key);

        /* pass along any optional parameters to the request payload */
        if ($adaptorId != null)
            $request->set('adaptorId', $adaptorId);

        return $request->get_result();
    }

}

?>
