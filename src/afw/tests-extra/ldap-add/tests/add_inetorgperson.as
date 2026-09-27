/* inetOrgPerson requires sn. NAME ('sn' 'surname') is one schema
   object, so an add of sn used to be skipped and slapd rejected
   the entry. The seeded person is there so modify of sn is checked
   on the same directory. */
const dn = "cn=Grace,ou=people,dc=world,dc=test";
add_object("ldap", "inetOrgPerson", {
    "cn": "Grace",
    "sn": "Hopper"
}, dn);
const added = get_object("ldap", "inetOrgPerson", dn);
/* cn and sn are not single-value, so one value is an array. */
assert(added.cn[0] === "Grace", "cn");
assert(added.sn[0] === "Hopper", "sn");
delete_object("ldap", "inetOrgPerson", dn);

const ada = "cn=Ada,ou=people,dc=world,dc=test";
modify_object("ldap", "inetOrgPerson", ada, [
    ["set_property", "sn", "Byron"]
]);
const changed = get_object("ldap", "inetOrgPerson", ada);
assert(changed.sn[0] === "Byron", "modified sn");
return true;
