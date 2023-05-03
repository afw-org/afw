#
# docs_html.py
#
# This file implements some basic HTML functions for creating HTML 
# code that will use resource templates to build from.

import html
import json
from _afwdev.common import resources

class DocsHtml:

    def __init__(self, options):

        self.options = options
        self.html_templates = {}        

        # load HTML templates from afwdev resources
        for object in resources.copy_resources(options, 'html_templates/'):
            self.html_templates[object['filename']] = object['resource']

    def get_html_template(self, template_name):            
        return self.html_templates.get(template_name)

    # escape text for HTML
    def escape(self, text):
        return html.escape(text) if text else ""

    # create HTML attributes string from dictionary
    def attributes(self, attributes):
            
        attr_str = ""
        for key in attributes:
            attr_str += ' %s="%s"' % (key.replace('_','') if key.startswith('_') else key, attributes[key])

        return attr_str    

    # create HTML <span> element
    def span(self, text, **kwargs):

        return '<span%s>%s</span>' % (self.attributes(kwargs), text)

    # create HTML <div> element
    def div(self, children, **kwargs):        

        return '<div%s>%s</div>' % (self.attributes(kwargs), children)

    # create HTML <span> element with italic style
    def italic(self, text="", **kwargs):

        return self.html_templates.get("italic.html") % {
            "text": text
        }

    # create a div with a height of spacing
    def spacer(self, spacing="20px"):

        return self.div("", style='height: ' + spacing + ';')

    # create a horizontal rule
    def hr(self):

        return '<hr/>'

    # create HTML <span> element with bold style
    def literal(self, text=""):

        return "<code>" + text + "</code>"

    # create a HTML code block section
    def code(self, code="", **kwargs):

        return self.html_templates.get("code.html") % { 
            "code": code
        }

    # create a HTML heading, with specified level
    def heading(self, text="", level=1, **kwargs):
    
        if level == 1:
            return self.html_templates.get("heading1.html") % { "heading": text }
        elif level == 2:
            return self.html_templates.get("heading2.html") % { "heading": text }
        elif level == 3:
            return self.html_templates.get("heading3.html") % { "heading": text }    

    # create a HTML <a> element
    def link(self, text="", href="", **kwargs):

        return self.html_templates.get("link.html") % { 
            "text": self.escape(text), "href": href 
        }

    # create a HTML <img> element
    def image(self, src="", description="", caption=None, **kwargs):

        img = self.html_templates.get("image.html") % { 
            "src": src, "alt": description
        }

        # center it
        img = self.div(img, _class="flex justify-center")

        if caption:
            img += self.div(
                self.span(caption, _class="text-sm"),
                _class="flex justify-center pb-8"
            )

        return img

    # create a HTML <p> element
    # Note: this function will split the text into multiple paragraphs
    #    if it contains double newlines
    def paragraph(self, text="", **kwargs):

        # if text contains newlines, then split it into multiple paragraphs        
        if text.find("\n\n") != -1:
            return "".join(map(lambda p: self.paragraph(p), text.split("\n\n")))
        else:
            return self.html_templates.get("paragraph.html") % { 
                "text": text
            }

    # create a HTML <li> element
    def step(self, step):
    
        return self.html_templates.get("step.html") % {
            "step": step
        }

    # create a HTML <ol> element
    def steps(self, steps, **kwargs):

        return self.html_templates.get("steps.html") % {
            "steps": steps
        }

    # create a HTML <li> element
    def list_item(self, content="", **kwargs):
    
        return self.html_templates.get("list_item.html") % {
            "item": content
        }

    # create a HTML <ul> element
    def list(self, items, **kwargs):

        return self.html_templates.get("list.html") % {
            "list": items
        }

    # create a <td> element
    def table_col(self, col, **kwargs):

        return self.html_templates.get("table_row_col.html") % {
            "col": col,
            "attrs": "style='white-space: pre-line;'"
        }

    # create a <tr> element
    def table_row(self, columns, **kwargs):

        return self.html_templates.get("table_row.html") % {
            "columns": "".join(map(lambda c: self.table_col(c), columns))
        }

    # create a HTML <table> element
    def table(self, columns, rows, **kwargs):
            
        return self.html_templates.get("table.html") % {
            "columns": "".join(map(lambda c: self.html_templates.get("table_head.html") % {
                "col": c
            }, columns)),
            "table_rows": "".join(map(lambda r: self.table_row(r), rows))
        }

    def footer_nav_next(self, href, text):
            
        return self.html_templates.get("footer_nav_next.html") % {
            "next_href": href,
            "next_text": text
        }

    def footer_nav_prev(self, href, text):
                
        return self.html_templates.get("footer_nav_prev.html") % {
            "prev_href": href,
            "prev_text": text
        }

    def footer_nav(self, prev_href, prev_text, next_href, next_text):
                
        return self.html_templates.get("footer_nav.html") % {
            "prev_href": prev_href,
            "prev_text": prev_text,
            "next_href": next_href,
            "next_text": next_text            
        }

    # create a HTML <header> element
    def header(self, content="", **kwargs):
    
        return self.html_templates.get("header.html") % {
            "content": content
        }

    # create a HTML <footer> element
    def footer(self, content="", **kwargs):

        return self.html_templates.get("footer.html") % {
            "content": content
        }

    # create a HTML <section> element
    def section(self, content="", **kwargs):

        return self.html_templates.get("section.html") % {
            "content": content
        }

    # creates breadcrumbs that can be used for page navigation
    def breadcrumbs(self, links, sep=" / "):

        breadcrumb_html = self.html_templates.get("breadcrumb.html")
        sep = '<li><span>' + sep + '</span></li>'
        breadcrumbs = []

        for link in links:
            if link.get('href') is None:
                breadcrumbs.append('<span>%s</span>' % link['text'])
            else:
                breadcrumbs.append(breadcrumb_html % {
                    "href": link['href'],
                    "text": link['text']
                })    

        return sep.join(breadcrumbs)
    

    # creates a page, suitable for the static doc style and layout
    def page(self, doc_root=".", title="", breadcrumbs="", nav_items = [], content="", header="", footer=""):

        return self.html_templates.get("page.html") % {            
            "title": title,
            "version": self.options.get('srcdir_info').get("version"),
            "breadcrumbs": breadcrumbs,            
            "nav_items": json.dumps(nav_items),
            "header": header if header != None else "",
            "content": content,
            "footer": footer if footer != None else "",
            "doc_root": doc_root
        }