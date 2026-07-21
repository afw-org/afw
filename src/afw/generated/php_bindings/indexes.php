<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for indexes
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file indexes.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category indexes
 */
class indexes
{
    private $session;

    /**
     * Constructs the indexes class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * index_create()
     *
     * Create an index definition.
     *
     * @param string $adapterId Id of adapter.
     * @param string $key Name of the property index to be created.
     * @param string $value Adaptive script (expression-like; must return a
     *                      value) used to calculate the index value(s). While
     *                      evaluating, current::object, current::objectId,
     *                      current::objectType, and current::key are
     *                      available (issue #54). If omitted, the property
     *                      named by key is indexed.
     * @param array $objectType Object type id(s) this index may apply to.
     * @param string $filter Adaptive script that must return a boolean to
     *                       decide whether this index applies to a particular
     *                       object. Uses the same current:: variables as
     *                       value (issue #54). If omitted, the filter is
     *                       always true.
     * @param array $options Indexing options.
     * @param boolean $retroactive Retroactively generate indexes for existing
     *                             objects.
     * @param boolean $test Test create (don't actually perform).
     *
     * @return object Object response from the indexing process
     */
    public function index_create(, $key, $adapterId = null, $value = null, $objectType = null, $filter = null, $options = null, $retroactive = null, $test = null)
    {
        $request = $this->$session->request();

        $request->set("function", "index_create");

        /* pass along required parameters to the request payload */
        $request->set("key", $key);

        /* pass along any optional parameters to the request payload */
        if ($adapterId != null)
            $request->set('adapterId', $adapterId);

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
     * @param string $adapterId Id of adapter.
     * @param string $objectType Id of adaptive object type indexes
     *
     * @return object Object response from the index repair process
     */
    public function index_list(, $adapterId = null, $objectType = null)
    {
        $request = $this->$session->request();

        $request->set("function", "index_list");

        /* pass along required parameters to the request payload */

        /* pass along any optional parameters to the request payload */
        if ($adapterId != null)
            $request->set('adapterId', $adapterId);

        if ($objectType != null)
            $request->set('objectType', $objectType);

        return $request->get_result();
    }

    /**
     * index_remove()
     *
     * Remove an index definition.
     *
     * @param string $adapterId Id of adapter.
     * @param string $key The index key to be removed.
     *
     * @return object Object response from the indexing process
     */
    public function index_remove(, $key, $adapterId = null)
    {
        $request = $this->$session->request();

        $request->set("function", "index_remove");

        /* pass along required parameters to the request payload */
        $request->set("key", $key);

        /* pass along any optional parameters to the request payload */
        if ($adapterId != null)
            $request->set('adapterId', $adapterId);

        return $request->get_result();
    }

}

?>
