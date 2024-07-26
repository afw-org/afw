#!/usr/bin/env perl

#**
# @file journal.pm
# @ingroup afw_bindings_perl
# @brief This file contains the Perl bindings for the journal category.
#*

package afw::journal;
use strict;
use warnings;

use Exporter qw(import);

our @EXPORT_OK = qw(
    journal_advance_cursor_for_consumer 
    journal_get_by_cursor 
    journal_get_first 
    journal_get_next_after_cursor 
    journal_get_next_for_consumer 
    journal_get_next_for_consumer_after_cursor 
    journal_mark_consumed 
);

=head1 NAME

afw::journal - Perl module for journal functions

=head2 Functions

The following functions are exported by default

=head3 journal_advance_cursor_for_consumer

Update the advance cursor for a consumer referenced by the consumerId
parameter. The limit parameter specifies the maximum number of entries to scan
for an applicable entry for consumer before returning. NULL is always
returned.

There are no response properties set by this function.

The properties of the _AdaptiveProvisioningPeer_ object associated with the
consumer_id are used in the following way:

The consumerFilter expression is used to determine if an entry is applicable.

Journal entries are scanned beginning at the entry at the cursor in the
advanceCursor property. If the advanceCursor property is not present, the scan
begins after the cursor in currentCursor. If neither are present, the scan
begins at the start of the journal.

If an new applicable entry is found or if the limit is met, the advanceCursor
property is set to the currently scanned entry's cursor.
Advance journal cursor for consumer

=head4 Parameters

    $adapterId

Id of adapter.

    $consumerId

The consumerId property value of the associated _AdaptiveProvisioningPeer_
object.

    $limit

The maximum number of entries that will be scanned for an entry where the
consumerFilter expression in the associated _AdaptiveProvisioningPeer_ object
evaluates to true.

=head3 journal_get_by_cursor

Get journal entry specified by entry_cursor parameter.

This option will set response properties 'entry' and 'cursor' if there is an
entry to retrieve. If an entry with the supplied cursor does not exist, a
not_found error is thrown.
Get journal entry at cursor

=head4 Parameters

    $adapterId

Id of adapter.

    $cursor

Journal entry cursor.

=head3 journal_get_first

Get first journal entry.

This option will set response properties 'entry' and 'cursor' if there is a
first entry to return.
Get first journal entry

=head4 Parameters

    $adapterId

Id of adapter.

=head3 journal_get_next_after_cursor

Get the next journal entry after the one specified by the entry_cursor
parameter.

This option will set response properties 'entry' and 'cursor' if there is a
next entry to retrieve.
Get next journal entry after cursor

=head4 Parameters

    $adapterId

Id of adapter.

    $cursor

Journal entry cursor.

=head3 journal_get_next_for_consumer

Get the next journal entry for a consumer referenced by the consumer_id
parameter. The entry_cursor parameter is ignored. The limit parameter
specifies the maximum number of entries to scan for an applicable entry for
consumer before returning.

 This option will set response properties 'entry' and 'entryCursor' if an
applicable entry is retrieved. Property 'reissue' will be set as described
below.

The properties of the _AdaptiveProvisioningPeer_ object associated with the
consumer_id are used in the following way:

The consumerFilter expression is used to determine if an entry is applicable.

If consumeCursor property exists, return that entry at that cursor again
immediately with a 'reissue' property added and set to true.

Journal entries are scanned beginning at the entry at the cursor in the
advanceCursor property. If the advanceCursor property is not present, the scan
begins after the cursor in currentCursor. If neither are present, the scan
begins at the start of the journal.

If an applicable entry is found, properties consumeStart and consumeCursor are
set, advanceCursor is removed, and the entry is returned. Method
mark_entry_consumed() will remove these properties.

If no applicable entry is found, advanceCursor is set to the last entry
scanned.
Get next journal entry for consumer

=head4 Parameters

    $adapterId

Id of adapter.

    $consumerId

The consumerId property value of the associated _AdaptiveProvisioningPeer_
object.

    $limit

The maximum number of entries that will be scanned for an entry where the
consumerFilter expression in the associated _AdaptiveProvisioningPeer_ object
evaluates to true.

=head3 journal_get_next_for_consumer_after_cursor

Get the next journal entry for a consumer referenced by the consumer_id after
the one specified by the entry_cursor parameter. The limit parameter specifies
the maximum number of entries to scan for an applicable entry for consumer
before returning.

This option will set response properties 'entry' and 'cursor' if an applicable
entry is retrieved.

The properties of the _AdaptiveProvisioningPeer_ object associated with the
consumer_id are used in the following way:

The consumerFilter expression is used to determine if an entry is applicable.

Unlike option get_next_for_consumer, no other properties are referenced or
modified.
Get next journal entry for consumer after cursor

=head4 Parameters

    $adapterId

Id of adapter.

    $consumerId

The consumerId property value of the associated _AdaptiveProvisioningPeer_
object.

    $cursor

Journal entry cursor.

    $limit

The maximum number of entries that will be scanned for an entry where the
consumerFilter expression in the associated _AdaptiveProvisioningPeer_ object
evaluates to true.

=head3 journal_mark_consumed

Mark a journal entry returned by get_next_for_consumer() as consumed.
Mark journal entry consumed

=head4 Parameters

    $adapterId

Id of adapter.

    $consumerId

The consumerId property value of the associated _AdaptiveProvisioningPeer_
object.

    $cursor

Journal entry cursor.

=cut

sub journal_advance_cursor_for_consumer {
    my ($adapterId, $consumerId, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "journal_advance_cursor_for_consumer");
    $request->set("consumerId", $consumerId);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub journal_get_by_cursor {
    my ($adapterId, $cursor) = @_;

    my $request = $session->request()

    $request->set("function" => "journal_get_by_cursor");
    $request->set("cursor", $cursor);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    return $request->getResult();
}

sub journal_get_first {
    my ($adapterId) = @_;

    my $request = $session->request()

    $request->set("function" => "journal_get_first");

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    return $request->getResult();
}

sub journal_get_next_after_cursor {
    my ($adapterId, $cursor) = @_;

    my $request = $session->request()

    $request->set("function" => "journal_get_next_after_cursor");
    $request->set("cursor", $cursor);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    return $request->getResult();
}

sub journal_get_next_for_consumer {
    my ($adapterId, $consumerId, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "journal_get_next_for_consumer");
    $request->set("consumerId", $consumerId);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub journal_get_next_for_consumer_after_cursor {
    my ($adapterId, $consumerId, $cursor, $limit) = @_;

    my $request = $session->request()

    $request->set("function" => "journal_get_next_for_consumer_after_cursor");
    $request->set("consumerId", $consumerId);
    $request->set("cursor", $cursor);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    if (defined $limit)
        $request->set("limit", $limit);

    return $request->getResult();
}

sub journal_mark_consumed {
    my ($adapterId, $consumerId, $cursor) = @_;

    my $request = $session->request()

    $request->set("function" => "journal_mark_consumed");
    $request->set("consumerId", $consumerId);
    $request->set("cursor", $cursor);

    if (defined $adapterId)
        $request->set("adapterId", $adapterId);

    return $request->getResult();
}

42;
