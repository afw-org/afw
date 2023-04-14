package afw::administrative;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    extension_load 
    extension_load_by_module_path 
    flag_get_active 
    flag_get_active_defaults 
    flag_get_defaults 
    flag_modify_defaults 
    flag_replace_defaults 
    flag_set 
    registry_key_check 
    service_get 
    service_restart 
    service_start 
    service_stop 
);

=head1 NAME

afw::administrative - Perl module for administrative functions

=head2 Functions

The following functions are exported by default

=head3 extension_load

Load an extension by its extension id if it is not already loaded. Loading an
AFW package's manifest extension will register the manifest of all extensions
in the package.
Load extension by id

=head4 Parameters

    $extension_id

This is the object id of a /afw/_AdaptiveManifest_/ object.

=head3 extension_load_by_module_path

Load an extension by its module path. Loading an AFW package's manifest
extension will register the manifest of all extensions in the package.
Load extension by module path

=head4 Parameters

    $module_path

This is the path to the dso containing the extension. If the extension is
installed in the normal place, the library name without a file extension
(.so) will suffice.

=head3 flag_get_active

Get a list of of the flagId of flags that are set in the current execution
context (xctx).
Get a list of active flags

=head4 Parameters

=head3 flag_get_active_defaults

Get a list of the flagId of flags that are set by default when a new
execution context (xctx) is created.
Get a list of default active flags

=head4 Parameters

=head3 flag_get_defaults

Get the list of the flagId of flags that are used to determine the default
active flags when an execution context (xctx) is created. This list can
contain the flagId of flags that have not yet been registered. Each of these
flags and the flags they include are set as the active default flags.
Get the list of flags used to determine the default active flags

=head4 Parameters

=head3 flag_modify_defaults

Add or remove flags from the list of the flagId of flags that are used to
determine the default active flags when an execution context (xctx) is
created. This list can contain the flagId of flags that have not yet been
registered. These flags and the flags they include are set as the active
default flags.

This change only lasts for the life of the current adaptive environment (e.g.
until the adaptive server or afw command ends). If you want the change to
persist, change the defaultFlags property in the application config.
Add or remove flags used to determine the default active flags

=head4 Parameters

    $flagId

The flagId of flags to be added or removed.

    $add

Specify true to add and false to remove flags. If not specified, flags are
added.

=head3 flag_replace_defaults

Completely replace the list of the flagId of flags that are used to determine
the default active flags when an execution context (xctx) is created. This
list can contain the flagId of flags that have not yet been registered. These
flags and the flags they include are set as the active default flags.

This change only lasts for the life of the current adaptive environment (e.g.
until the adaptive server or afw command ends). If you want the change to
persist, change the defaultFlags property in the application config.
Replace the list of flags used to determine the default active flags

=head4 Parameters

    $flagId

The list of the flagId of flags used to determine the default active flags.

=head3 flag_set

Set or unset one or more active xctx (request) flags.
Set or unset active xctx flags

=head4 Parameters

    $flagId

List of flagId of flags to set or unset.

    $setTo

Specify true to set and false to unset. If not specified, flags are set.

=head3 registry_key_check

This will check to see if a registry key exists for a specified registry type
and optionally load it's associated extension if needed.
Check to see if a registry key exists

=head4 Parameters

    $registryType

This is the registry type, which is the object id of a
/afw/_AdaptiveEnvironmentRegistryType_/ object.

    $key

This is a key to check for existence in the specified registryType.

    $loadExtension

Specifying true for this optional parameter will cause the associated
extension to be loaded if needed.

=head3 service_get

Get a service object.
Get service object

=head4 Parameters

    $serviceId

The serviceId of the service.

=head3 service_restart

Restart a service.
Restart service

=head4 Parameters

    $serviceId

The serviceId of the service to restart.

=head3 service_start

Start a service.
Start service

=head4 Parameters

    $serviceId

The serviceId of the service to start

=head3 service_stop

Stop a service.
Stop service

=head4 Parameters

    $serviceId

The serviceId of the service to stop.

=cut

sub extension_load {
    my ($extension_id) = @_;

    my $request = $session->request()

    $request->set("function" => "extension_load");
    $request->set("extension_id", $extension_id);

    return $request->getResult();
}

sub extension_load_by_module_path {
    my ($module_path) = @_;

    my $request = $session->request()

    $request->set("function" => "extension_load_by_module_path");
    $request->set("module_path", $module_path);

    return $request->getResult();
}

sub flag_get_active {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "flag_get_active");

    return $request->getResult();
}

sub flag_get_active_defaults {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "flag_get_active_defaults");

    return $request->getResult();
}

sub flag_get_defaults {
    my () = @_;

    my $request = $session->request()

    $request->set("function" => "flag_get_defaults");

    return $request->getResult();
}

sub flag_modify_defaults {
    my ($flagId, $add) = @_;

    my $request = $session->request()

    $request->set("function" => "flag_modify_defaults");
    $request->set("flagId", $flagId);

    if (defined $add)
        $request->set("add", $add);

    return $request->getResult();
}

sub flag_replace_defaults {
    my ($flagId) = @_;

    my $request = $session->request()

    $request->set("function" => "flag_replace_defaults");
    $request->set("flagId", $flagId);

    return $request->getResult();
}

sub flag_set {
    my ($flagId, $setTo) = @_;

    my $request = $session->request()

    $request->set("function" => "flag_set");
    $request->set("flagId", $flagId);

    if (defined $setTo)
        $request->set("setTo", $setTo);

    return $request->getResult();
}

sub registry_key_check {
    my ($registryType, $key, $loadExtension) = @_;

    my $request = $session->request()

    $request->set("function" => "registry_key_check");
    $request->set("registryType", $registryType);
    $request->set("key", $key);

    if (defined $loadExtension)
        $request->set("loadExtension", $loadExtension);

    return $request->getResult();
}

sub service_get {
    my ($serviceId) = @_;

    my $request = $session->request()

    $request->set("function" => "service_get");
    $request->set("serviceId", $serviceId);

    return $request->getResult();
}

sub service_restart {
    my ($serviceId) = @_;

    my $request = $session->request()

    $request->set("function" => "service_restart");
    $request->set("serviceId", $serviceId);

    return $request->getResult();
}

sub service_start {
    my ($serviceId) = @_;

    my $request = $session->request()

    $request->set("function" => "service_start");
    $request->set("serviceId", $serviceId);

    return $request->getResult();
}

sub service_stop {
    my ($serviceId) = @_;

    my $request = $session->request()

    $request->set("function" => "service_stop");
    $request->set("serviceId", $serviceId);

    return $request->getResult();
}

42;
