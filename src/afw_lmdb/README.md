Lightning Memory-Mapped Database (LMDB) Adaptor
===============================================

Description
-----------
LMDB is a pure in-memory, key/value database, maintained by OpenLDAP.

Doxygen Documentation: http://lmdb.tech/doc/
Source:  http://www.openldap.org/

This extension implements the Adaptor Factory, Adaptor, Adaptor Session, and Transaction interfaces to allow objects to be stored and retrieved in the LMDB Database.


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

Add the following to your configuration to use the LMDB adaptor:

````
/* UBJSON is a required content-type for the adaptor */
{
    type            : "extension",
    extension_id    : "ubjson"
},

/* register the LMDB extension. */
{
    type            : "extension",
    extension_id    : "afw_lmdb_adaptor"
},

/* configure the LMDB adaptor with uri of "lmdb" */
{
    type            : "adaptor",
    adaptor_type    : "lmdb",
    adaptor_id      : "lmdb",
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
    event_journal_adaptor_id : "lmdb"
}

````

Adding indexes
------------------
Indexes will increase the efficiency of retrieve_objects() queries.  The first recommended index is for the ObjectType.  To add an index, stop the server, or any other application using the LMDB database, and run:

````
  afw -f afw.conf -x "index_create('lmdb', 'surname', 'object.get("surname")', ['VaultIdentityData'], null, ['case-insensitive-string'], false, false)"
````

Where:

* ```` afw.conf ```` refers to your afw server configuration file.
* ```` lmdb ```` refers to the adaptor-id for the LMDB database you are adding an index for
* ```` VaultIdentityData ```` refers to the objectType.
* ```` surname ```` refers to the property you are indexing.

