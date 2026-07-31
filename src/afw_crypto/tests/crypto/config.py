#!/usr/bin/env python3

"""afw_crypto Adaptive Script tests."""

import os

Environment = "crypto"

# Seal key for bindParameters file-sealed template test (base64 of 32 octets).
# Not an LDAP password — only key material for AES-GCM.
_SEAL_KEY_B64 = "MDEyMzQ1Njc4OWFiY2RlZjAxMjM0NTY3ODlhYmNkZWY="


def before_all():
    os.environ["AFW_CRYPTO_TEST_SEAL_KEY"] = _SEAL_KEY_B64


def before_each():
    os.environ["AFW_CRYPTO_TEST_SEAL_KEY"] = _SEAL_KEY_B64
