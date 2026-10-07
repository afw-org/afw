#! /usr/bin/env python3
##
# @file fuzz_hostile_pieces.py
# @brief Request pieces for the hostile fuzz source (#485 step 5).
#
# fuzz.py calls build(rnd) once per request. Contract:
#
#   build(rnd) -> {"path": str, "method": str, "body": bytes,
#                  "overrides": {param name: str or None}}
#
#   - rnd is the random.Random fuzz.py passes in. Use only it: no other
#     randomness and no clock, so request i is the same every run and
#     --replay SEED:INDEX resends it.
#   - The request goes through fcgi_client.fcgi_request(path, method,
#     body, param_overrides=overrides). An override of None drops that
#     FastCGI parameter; any other value replaces the default (for
#     example CONTENT_LENGTH, CONTENT_TYPE, HTTP_ACCEPT, QUERY_STRING).
#   - Action bodies stay simple: nothing that loops (that is the mutate
#     source's job, after #490).
#   - Paths name only the leaf's adapters: afw (runtime) and data (a file
#     adapter rooted in the leaf's work directory).
#   - Keep each request under about 1MB.
#
# Any HTTP status is a fine answer. A request fails only on a timeout, a
# lost connection, or a reply without a FastCGI end record or status
# line.
#
# These pieces are placeholders: ordinary valid requests so the plumbing
# is tested. The varied pieces are to be written by another contributor.
#

import json

_REQUESTS = (
    ("GET", "/afw/_AdaptiveProcess_/current", None),
    ("GET", "/afw/_AdaptiveServer_/current", None),
    ("GET", "/data/Demo/seed", None),
    ("POST", "/afw", {"actions": [{"function": "add", "a": 1, "b": 2}]}),
    ("POST", "/afw", {"actions": [
        {"function": "eval<script>", "source": "return 1;"}]}),
)


def build(rnd):
    method, path, body = rnd.choice(_REQUESTS)
    data = json.dumps(body).encode("utf-8") if body is not None else b""
    return {"path": path, "method": method, "body": data, "overrides": {}}
