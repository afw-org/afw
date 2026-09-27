const p = get_object(
    "ldap", "inetOrgPerson", "cn=Ada,ou=people,dc=world,dc=test");
/* sn is not single-value, so one value is an array. */
assert(p.sn[0] === "Lovelace");
return true;
