const p = get_object(
    "ldap", "inetOrgPerson", "cn=Ada,ou=people,dc=world,dc=test");
assert(p.sn === "Lovelace");
return true;
