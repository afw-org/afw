#!/usr/bin/env python3

##
# @file normalize.py
# @ingroup afwdev_build
# @brief Rewrite a parsed grammar so its diagrams read well.
#
# These are the rewrites Railroad Diagram Generator (rr.war) applies by
# default, so diagrams keep the shape readers already know:
#
# - Left factoring: alternatives that start with the same items share them,
#   even when they are not next to each other:
#   'throw' | 'throw' Expression  ->  'throw' Expression?
# - Right factoring: alternatives that end with the same items share them.
# - ( X+ )?  ->  X*
# - X ( sep X )*  ->  ('loop', X, sep): one loop with sep on the return path.
#   X may be several items. X X*  becomes ('loop', X, None).
#
# The result uses the node kinds from parse.py plus ('loop', item, sep).
#


def _items(node):
    """A node as a list of sequence items ([] for the empty sequence)."""
    return list(node[1]) if node[0] == 'seq' else [node]


def _seq(items):
    return items[0] if len(items) == 1 else ('seq', items)


def _common_prefix(lists):
    k = 0
    while all(len(l) > k for l in lists) and all(l[k] == lists[0][k] for l in lists):
        k += 1
    return k


def _alternatives(lists):
    """Alternatives (item lists) as one node; an empty one makes it optional."""
    nonempty = [_seq(l) for l in lists if l]
    if not nonempty:
        return ('seq', [])
    body = nonempty[0] if len(nonempty) == 1 else normalize(('choice', nonempty))
    return normalize(('opt', body)) if len(nonempty) < len(lists) else body


def _group(alts, key):
    """Group item lists by key(list), keeping first-seen order."""
    groups = []
    for l in alts:
        k = key(l) if l else None
        for g in groups:
            if k is not None and g[0] == k:
                g[1].append(l)
                break
        else:
            groups.append((k, [l]))
    return [g[1] for g in groups]


def _left_factor(alts):
    out = []
    for group in _group(alts, lambda l: l[0]):
        if len(group) == 1:
            out.append(group[0])
            continue
        k = _common_prefix(group)
        out.append(group[0][:k] + _items(_alternatives([l[k:] for l in group])))
    return out


def _right_factor(alts):
    out = []
    for group in _group(alts, lambda l: l[-1]):
        if len(group) == 1:
            out.append(group[0])
            continue
        k = _common_prefix([l[::-1] for l in group])
        heads = [l[:len(l) - k] for l in group]
        out.append(_items(_alternatives(heads)) + group[0][len(group[0]) - k:])
    return out


def _loops(items):
    """X ( sep X )*  ->  ('loop', X, sep)."""
    out = []
    for item in items:
        if item[0] == 'star':
            body = _items(item[1])
            m = len(body)
            for k in range(min(m, len(out)), 0, -1):
                if out[len(out) - k:] == body[m - k:]:
                    x = out[len(out) - k:]
                    del out[len(out) - k:]
                    out.append(('loop', _seq(x), _seq(body[:m - k]) if m > k else None))
                    break
            else:
                out.append(item)
        else:
            out.append(item)
    return out


##
# @brief Normalize one production's node (see the file comment).
# @param node A node from parse.parse().
# @return The rewritten node.
#
def normalize(node):
    kind = node[0]
    if kind == 'seq':
        items = []
        for child in node[1]:
            child = normalize(child)
            items.extend(child[1] if child[0] == 'seq' else [child])
        items = _loops(items)
        return _seq(items) if items else ('seq', [])
    if kind == 'choice':
        alts = []
        for child in node[1]:
            child = normalize(child)
            if child[0] == 'choice':
                alts.extend(_items(a) for a in child[1])
            else:
                alts.append(_items(child))
        alts = _right_factor(_left_factor(alts))
        if len(alts) == 1:
            return _seq(alts[0]) if alts[0] else ('seq', [])
        return ('choice', [_seq(a) if a else ('seq', []) for a in alts])
    if kind == 'opt':
        child = normalize(node[1])
        if child[0] == 'plus':
            return ('star', child[1])
        if child[0] in ('star', 'opt'):
            return child
        return ('opt', child)
    if kind in ('plus', 'star'):
        return (kind, normalize(node[1]))
    if kind == 'diff':
        return ('diff', normalize(node[1]), normalize(node[2]))
    return node


##
# @brief Normalize every production.
# @param rules dict from parse.parse().
# @return dict of production name to normalized node.
#
def normalize_rules(rules):
    return {name: normalize(node) for name, node in rules.items()}
