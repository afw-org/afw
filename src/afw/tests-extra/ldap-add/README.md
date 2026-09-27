# ldap-add

One `inetOrgPerson` add through the LDAP adapter, then a get, a
delete, and a modify of `sn` on the seeded person. `slapd` rejects
the add when `sn` is missing. `cn` and `sn` are not single-value,
so a get returns an array of one string.

```bash
afwdev test -T src/afw/tests-extra/ldap-add
```

The directory is the firehose `slapd`: `127.0.0.1` on a free port,
seeded from `ldap/seed.ldif`, stopped when the leaf ends. The
`slapd` package has to be installed. This leaf is not part of
`afwdev test -j`, and it is not the firehose load loop. That loop
still only reads `cn=Ada`.
