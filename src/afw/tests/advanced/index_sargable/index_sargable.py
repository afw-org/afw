#!/usr/bin/env python3
"""
afw_adapter_impl_index_sargable() is the sole decision point gating whether
retrieve_objects() uses an index-accelerated query or falls back to a full
scan. It's a core, adapter-agnostic function that only ever reads
indexDefinitions (never calls through the interface's inf/vtable), so it's
directly testable with a fake instance and no live adapter/session at all
(issue #57).
"""

from _afwdev.test.c_probe import run_c_probe


def run():
    return run_c_probe(
        "index_sargable_probe.c",
        "Index sargability decision (afw_adapter_impl_index_sargable)",
        [
            (
                "indexed-eq",
                "eq on an indexed property with matching objectType is sargable",
            ),
            (
                "unindexed-property",
                "a property with no index definition at all is not sargable",
            ),
            (
                "wrong-objecttype",
                "an index scoped to a different objectType is not sargable",
            ),
            (
                "unsupported-operator",
                "ne is outside the eq/lt/le/gt/ge whitelist and is not sargable",
            ),
            (
                "no-filter",
                "criteria with no filter at all is not sargable",
            ),
            (
                "key-value-mismatch",
                "an index keyed differently than the property its value script "
                "indexes is not recognized as sargable (known limitation)",
            ),
        ],
    )
