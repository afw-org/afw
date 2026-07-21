Lightning Memory-Mapped Database (LMDB) Adapter
===============================================

Description
-----------
LMDB is a pure in-memory, key/value database, maintained by OpenLDAP.

Doxygen Documentation: http://lmdb.tech/doc/
Source:  http://www.openldap.org/

This extension implements the Adapter Factory, Adapter, Adapter Session, and Transaction interfaces to allow objects to be stored and retrieved in the LMDB Database.


Advantages
----------

* In-memory database, providing very fast and efficient database operations.
* B+Tree for sorted records.
* Support for reader/writer transactions.
* OpenLDAP Public License


Disadvantages
-------------

* Replication and Sharding not provided


Configuration
-------------

Add the following to your configuration to use the LMDB adapter:

````
/* UBJSON is a required content-type for the adapter */
{
    type            : "extension",
    extension_id    : "ubjson"
},

/* register the LMDB extension. */
{
    type            : "extension",
    extension_id    : "afw_lmdb_adapter"
},

/* configure the LMDB adapter with uri of "lmdb" */
{
    type            : "adapter",
    adapter_type    : "lmdb",
    adapter_id      : "lmdb",
    // properties related to the LMDB environment
    env             : {
                          path             : "/home/ubuntu/afw_lmdb",
                          mode             : "384",      // decimal for 0600
                          maxreaders       : "126",
                          maxdbs           : "128",
                          mapsize          : "1000485760"  // size in bytes (multiple of OS page size)
    },
    // soft and hard limits on the operations
    limits          : {
                          size : {
                              soft : "500",
                              hard : "1000"
                          },
                          time : {
                              soft : "3600",
                              hard : "14400"
                          }
                      },
    event_journal_adapter_id : "lmdb"
}

````

Adding indexes
------------------
Indexes are **optional**. Without them, LMDB still works as a normal object store (add/get/replace/modify/delete/retrieve by dump). Indexes can make some `retrieve_objects()` queries more efficient.

Definitions are **not** written in the adapter conf stanza. They are stored **inside the LMDB environment** (`internalConfig.indexDefinitions` on Primary UUID 0) by the core Adaptive functions `index_create` / `index_list` / `index_remove`. `afw -x "..."` is only the CLI way to run those functions once conf/extensions are loaded.

The first recommended index is often for ObjectType. To add an index, stop the server (or any other process using that LMDB path), then for example:

````
  afw -f afw.conf -x "index_create('lmdb', 'surname', 'current::object.get(\"surname\")', ['VaultIdentityData'], undefined, ['case-insensitive-string'], false, false)"
````

Where:

* ```` afw.conf ```` — AFW configuration that loads the LMDB adapter
* ```` lmdb ```` — adapter id
* ```` VaultIdentityData ```` — objectType filter for the definition
* ```` surname ```` — index key (also the default property name if `value` is omitted)
* Filter/value scripts (issue #54): **`current::object`**, **`current::objectId`**, **`current::objectType`**, **`current::key`**. Bare ambient `object` is not set.
* Lookup data lives in LMDB named DBs of the form **`Index#<objectType>#<key>`**.

Note: `index_create` / retroactive rebuild still has known txn/persistence issues in some session setups; track with GitHub #54 / #57.

