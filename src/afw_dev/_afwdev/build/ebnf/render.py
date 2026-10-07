#!/usr/bin/env python3

##
# @file render.py
# @ingroup afwdev_build
# @brief Draw normalized EBNF productions as SVG railroad diagrams.
#
# Layout is railroad.py (vendored, MIT). This file adds the handbook look:
#
# - literal tokens ('if', '(') are rounded sky boxes in monospace, like code
# - rule names are plain boxes and link to their diagram
# - character classes, #xN characters and A - B differences are hexagons
# - a comment inside a rule is a small italic label above the next item
# - loops return over the top; a big choice under * runs forward instead
#
# One stylesheet carries both palettes. Dark follows prefers-color-scheme,
# like the handbook (tailwind darkMode: 'media').
#

from html import escape
import io

from . import railroad as rr

# railroad.py reads these module globals when it lays out.
rr.VS = 10                      # vertical space between alternatives
rr.AR = 10                      # arc radius
rr.INTERNAL_ALIGNMENT = 'left'  # choice branches start at the left

BOX_H = 26
FONT_PX = 13
LABEL_PX = 11.5
WRAP_WIDTH = 992                # wrap rows wider than this (rr's default)

# Advance widths (1/1000 em) of a Verdana-like sans; scaled by SANS_SCALE for
# Inter / Helvetica-class faces. The reader's font decides the real width,
# because an SVG shown in an <img> can only use installed fonts.
_ADVANCE = dict(zip(
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
    "abcdefghijklmnopqrstuvwxyz{|}~",
    [352, 394, 459, 818, 636, 1076, 727, 269, 454, 454, 636, 818, 364, 454, 364, 454]
    + [636] * 10
    + [454, 454, 818, 818, 818, 545, 1000, 684, 686, 698, 771, 632, 575, 775, 751,
       421, 455, 693, 557, 843, 748, 787, 603, 787, 695, 684, 616, 732, 684, 989,
       685, 615, 685, 454, 454, 454, 818, 636, 636, 601, 623, 521, 623, 596, 352,
       623, 633, 274, 344, 592, 274, 973, 633, 607, 623, 623, 427, 521, 394, 633,
       592, 818, 592, 592, 525, 635, 454, 635, 818]))
SANS_SCALE = 0.87
MONO_EM = 0.6


def sans_width(text, px=FONT_PX):
    return sum(_ADVANCE.get(c, 700) for c in text) * px / 1000 * SANS_SCALE


def mono_width(text, px=12.5):
    return len(text) * px * MONO_EM


_BASE_CSS = """
svg.railroad-diagram path { fill: none; stroke-width: 1.5; stroke-linecap: round; stroke-linejoin: round; }
svg.railroad-diagram text { font: 500 13px "Inter var", Inter, ui-sans-serif, system-ui, -apple-system, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; text-anchor: middle; }
svg.railroad-diagram g.terminal text, svg.railroad-diagram g.regexp text { font: 500 12.5px ui-monospace, SFMono-Regular, Menlo, Consolas, "Liberation Mono", monospace; }
svg.railroad-diagram g.regexp text { font-size: 12px; }
svg.railroad-diagram text.label-text { font-size: 11.5px; font-style: italic; font-weight: 400; text-anchor: start; }
svg.railroad-diagram .box { stroke-width: 1; }
svg.railroad-diagram a:hover .box { stroke-width: 2; }
"""

# Tailwind slate / sky, as the handbook pages use them.
_LIGHT_CSS = """
svg.railroad-diagram path { stroke: #94a3b8; }
svg.railroad-diagram .marker { fill: #94a3b8; }
svg.railroad-diagram g.terminal .box { fill: #f0f9ff; stroke: #7dd3fc; }
svg.railroad-diagram g.terminal text { fill: #0369a1; }
svg.railroad-diagram g.nonterminal .box { fill: #ffffff; stroke: #cbd5e1; }
svg.railroad-diagram g.nonterminal text { fill: #334155; }
svg.railroad-diagram g.regexp .box { fill: #f8fafc; stroke: #cbd5e1; }
svg.railroad-diagram g.regexp text { fill: #475569; }
svg.railroad-diagram text.label-text { fill: #64748b; }
"""

_DARK_CSS = """
svg.railroad-diagram path { stroke: #64748b; }
svg.railroad-diagram .marker { fill: #64748b; }
svg.railroad-diagram g.terminal .box { fill: #13283f; stroke: #0284c7; }
svg.railroad-diagram g.terminal text { fill: #7dd3fc; }
svg.railroad-diagram g.nonterminal .box { fill: #1e293b; stroke: #334155; }
svg.railroad-diagram g.nonterminal text { fill: #e2e8f0; }
svg.railroad-diagram g.regexp .box { fill: #172033; stroke: #334155; }
svg.railroad-diagram g.regexp text { fill: #94a3b8; }
svg.railroad-diagram text.label-text { fill: #94a3b8; }
"""

## Both palettes; dark under prefers-color-scheme (what the build writes).
CSS = (_BASE_CSS + _LIGHT_CSS
       + '@media (prefers-color-scheme: dark) {' + _DARK_CSS + '}\n')
## Light palette only.
CSS_LIGHT = _BASE_CSS + _LIGHT_CSS
## Dark palette only.
CSS_DARK = _BASE_CSS + _DARK_CSS


class Box(rr.DiagramItem):
    """terminal: rounded, monospace. nonterminal: square, links. regexp: hexagon."""

    def __init__(self, text, kind, href=None):
        rr.DiagramItem.__init__(self, 'g', {'class': kind})
        self.text, self.kind, self.href = text, kind, href
        if kind == 'nonterminal':
            self.width = round(sans_width(text)) + 24
        elif kind == 'terminal':
            self.width = max(BOX_H, round(mono_width(text)) + 22)
        else:
            self.width = round(mono_width(text, 12)) + 28
        self.up = self.down = BOX_H / 2
        self.needsSpace = True

    def format(self, x, y, width):
        left, right = rr.determineGaps(width, self.width)
        rr.Path(x, y).h(left).addTo(self)
        rr.Path(x + left + self.width, y).h(right).addTo(self)
        x0, top, w, h = x + left, y - BOX_H / 2, self.width, BOX_H
        target = self
        if self.href:
            target = rr.DiagramItem('a', {'xlink:href': self.href}).addTo(self)
        if self.kind == 'regexp':
            k = 9
            points = [(x0, y), (x0 + k, top), (x0 + w - k, top), (x0 + w, y),
                      (x0 + w - k, top + h), (x0 + k, top + h)]
            rr.DiagramItem('polygon', {
                'class': 'box',
                'points': ' '.join('%g,%g' % p for p in points)}).addTo(target)
        else:
            r = h / 2 if self.kind == 'terminal' else 4
            rr.DiagramItem('rect', {
                'class': 'box', 'x': x0, 'y': top, 'width': w, 'height': h,
                'rx': r, 'ry': r}).addTo(target)
        rr.DiagramItem('text', {'x': x0 + w / 2, 'y': y + 4.5}, self.text).addTo(target)
        return self


class LoopAbove(rr.DiagramItem):
    """item on the main line; the return path, through repeat, goes over the top."""

    def __init__(self, item, repeat=None):
        rr.DiagramItem.__init__(self, 'g')
        self.item = item
        self.rep = repeat or rr.Skip()
        self.width = max(self.item.width, self.rep.width) + rr.AR * 2
        self.height = self.item.height
        self.down = self.item.down
        self.up = max(rr.AR * 2, self.item.up + rr.VS + self.rep.down
                      + self.rep.height + self.rep.up)
        self.needsSpace = True

    def format(self, x, y, width):
        ar = rr.AR
        left, right = rr.determineGaps(width, self.width)
        rr.Path(x, y).h(left).addTo(self)
        rr.Path(x + left + self.width, y + self.height).h(right).addTo(self)
        x += left
        rr.Path(x, y).right(ar).addTo(self)
        self.item.format(x + ar, y, self.width - ar * 2).addTo(self)
        rr.Path(x + self.width - ar, y + self.height).right(ar).addTo(self)
        d = max(ar * 2, self.item.up + rr.VS + self.rep.down + self.rep.height)
        rr.Path(x + ar, y).arc('sw').up(d - ar * 2).arc('wn').addTo(self)
        self.rep.format(x + ar, y - d, self.width - ar * 2).addTo(self)
        rr.Path(x + self.width - ar, y - d + self.rep.height).arc('ne').down(
            d - ar * 2 - self.rep.height + self.item.height).arc('es').addTo(self)
        return self

    def walk(self, cb):
        cb(self)
        self.item.walk(cb)
        self.rep.walk(cb)


class Label(rr.DiagramItem):
    """A comment with nothing after it: italic text above the track."""

    def __init__(self, text):
        rr.DiagramItem.__init__(self, 'g')
        self.text = text
        self.width = round(sans_width(text, LABEL_PX)) + 8
        self.up, self.down = 20, 0
        self.needsSpace = True

    def format(self, x, y, width):
        rr.Path(x, y).h(width).addTo(self)
        rr.DiagramItem('text', {'class': 'label-text', 'x': x + 4, 'y': y - 6},
                       self.text).addTo(self)
        return self


class LabelOver(rr.DiagramItem):
    """A comment drawn above the item after it. Adds height, not width."""

    def __init__(self, text, item):
        rr.DiagramItem.__init__(self, 'g')
        self.text, self.item = text, item
        self.width = max(item.width, round(sans_width(text, LABEL_PX)) + 4)
        self.height, self.down = item.height, item.down
        self.up = item.up + 16
        self.needsSpace = item.needsSpace

    def format(self, x, y, width):
        self.item.format(x, y, width).addTo(self)
        rr.DiagramItem('text', {'class': 'label-text', 'x': x + 2,
                                'y': y - self.item.up - 5}, self.text).addTo(self)
        return self

    def walk(self, cb):
        cb(self)
        self.item.walk(cb)


class _Dot(rr.DiagramItem):

    def __init__(self, end):
        rr.DiagramItem.__init__(self, 'g')
        self.type = 'simple'
        self.end = end
        self.width, self.up, self.down = 10, 5, 5
        self.needsSpace = False

    def format(self, x, y, width):
        if self.end:
            rr.Path(x, y).h(2).addTo(self)
        else:
            rr.Path(x + 8, y).h(2).addTo(self)
        rr.DiagramItem('circle', {'class': 'marker', 'cx': x + (6 if self.end else 4),
                                  'cy': y, 'r': 4}).addTo(self)
        return self


class Start(_Dot, rr.Start):
    def __init__(self):
        _Dot.__init__(self, end=False)


class End(_Dot, rr.End):
    def __init__(self):
        _Dot.__init__(self, end=True)


def _text(node):
    kind = node[0]
    if kind == 'diff':
        return _text(node[1]) + ' - ' + _text(node[2])
    if kind == 'str':
        return node[1][1:-1]
    if kind in ('name', 'cls', 'hex'):
        return node[1]
    return '…'


def _is_directive(node):
    # /* ws: explicit */ and friends are REx parser options, not prose.
    return node[0] == 'comment' and node[1].startswith('ws:')


class Renderer:
    """Turns normalized productions into railroad items and SVG."""

    ##
    # @param rules Normalized productions (normalize.normalize_rules()).
    # @param href  Function from production name to link target.
    #
    def __init__(self, rules, href=lambda name: '#' + name):
        self.rules = rules
        self.href = href
        # rr default: a production that is a single literal is drawn inline.
        self.inline = {name: node[1][1:-1] for name, node in rules.items()
                       if node[0] == 'str'}

    def item(self, node, limit):
        kind = node[0]
        if kind == 'str':
            return Box(node[1][1:-1], 'terminal')
        if kind == 'name':
            if node[1] in self.inline:
                return Box(self.inline[node[1]], 'terminal')
            return Box(node[1], 'nonterminal', href=self.href(node[1]))
        if kind in ('cls', 'hex', 'diff'):
            return Box(_text(node), 'regexp')
        if kind == 'comment':
            return rr.Skip() if _is_directive(node) else Label(node[1])
        if kind == 'choice':
            return rr.Choice(0, *[self.item(a, limit - 4 * rr.AR) for a in node[1]])
        if kind == 'opt':
            inner = node[1]
            if inner[0] == 'choice':        # ( A | B )?: one choice with a bypass
                return rr.Choice(0, rr.Skip(),
                                 *[self.item(a, limit - 4 * rr.AR) for a in inner[1]])
            item = self.item(inner, limit - 4 * rr.AR)
            if isinstance(item, LoopAbove):  # an optional loop stays on the main line
                return rr.Choice(0, item, rr.Skip())
            return rr.Choice(0, rr.Skip(), item)
        if kind == 'plus':
            return LoopAbove(self.item(node[1], limit - 2 * rr.AR))
        if kind == 'star':
            if node[1][0] == 'choice':      # a big choice runs forward, bypass below
                return rr.Choice(0, LoopAbove(self.item(node[1], limit - 6 * rr.AR)),
                                 rr.Skip())
            return LoopAbove(rr.Skip(), self.item(node[1], limit - 2 * rr.AR))
        if kind == 'loop':
            sep = node[2]
            if sep is not None and sep[0] == 'choice':
                sep = ('choice', sep[1][::-1])  # read bottom-up on the return path
            sub = limit - 2 * rr.AR
            return LoopAbove(self.item(node[1], sub),
                             self.item(sep, sub) if sep is not None else None)
        if kind == 'seq':
            return self.sequence(node[1], limit)
        raise ValueError('unknown node kind: ' + repr(kind))

    def sequence(self, nodes, limit):
        out, i = [], 0
        while i < len(nodes):
            node = nodes[i]
            nxt = nodes[i + 1] if i + 1 < len(nodes) else None
            if (node[0] == 'comment' and not _is_directive(node)
                    and nxt is not None and nxt[0] != 'comment'):
                out.append(LabelOver(node[1], self.item(nxt, limit)))
                i += 2
                continue
            out.append(self.item(node, limit))
            i += 1
        out = [o for o in out if not isinstance(o, rr.Skip)] or [rr.Skip()]
        if len(out) == 1:
            return out[0]
        return self._wrap(out, limit)

    @staticmethod
    def _wrap(items, limit):
        if sum(o.width + 20 for o in items) <= limit:
            return rr.Sequence(*items)
        rows, row, width = [], [], 0
        for o in items:
            if row and width + o.width + 20 > limit - 2 * rr.AR:
                rows.append(row)
                row, width = [], 0
            row.append(o)
            width += o.width + 20
        rows.append(row)
        if len(rows) == 1:
            return rr.Sequence(*items)
        return rr.Stack(*[rr.Sequence(*r) for r in rows])

    def diagram(self, name):
        return rr.Diagram(Start(), self.item(self.rules[name], WRAP_WIDTH - 80), End())

    ##
    # @brief One production as SVG text.
    # @param name Production name.
    # @param css  Stylesheet to embed; None leaves it to the containing page.
    # @param doc  Optional doc comment, written as <desc>.
    #
    def svg(self, name, css=CSS, doc=None):
        d = self.diagram(name)
        d.format(paddingTop=8, paddingRight=8, paddingBottom=8, paddingLeft=8)
        buf = io.StringIO()
        if css is None:
            d.attrs['xmlns'] = 'http://www.w3.org/2000/svg'
            d.attrs['xmlns:xlink'] = 'http://www.w3.org/1999/xlink'
            d.writeSvg(buf.write)
        else:
            d.writeStandalone(buf.write, css=css)
        svg = buf.getvalue()
        head = '<title>%s</title>' % escape(name)
        if doc:
            head += '<desc>%s</desc>' % escape(doc)
        i = svg.index('>') + 1
        return (svg[:i].replace('<svg ', '<svg role="img" ', 1) + head + svg[i:])
