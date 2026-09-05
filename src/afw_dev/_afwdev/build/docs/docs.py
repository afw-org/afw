#!/usr/bin/env python3

##
# @file docs.py
# @ingroup afwdev_build_docs
# @brief This file defines the build method for building documentation.
#

import os
import pathlib
import shutil
import subprocess
import itertools
import re
from lxml import etree

from _afwdev.build.docs import docs_generated
from _afwdev.build.docs.docs_html import DocsHtml
from _afwdev.common import msg, nfc, package
from _afwdev.build import ebnf_diagrams

# global variables to hold the sidenav contents, which are created by running through all the files
sidenav_content = {}
nav_groups = ''


#
# generate_sidenav(docsHtml, relative_root_path, category)
#
# Generates the side navigation for each page.
#
def generate_sidenav(docsHtml, relative_root_path, category, doc=None, title=None, sorted=False):

    msg.info("        Building sidenav for {} - {} - {}".format(category, doc, title))
    
    groups = sidenav_content    
    if category:
        groups = sidenav_content[category]

    nav_groups = []
        
    for ref in groups:
        group_title = ref.get("title")        
        links = ref.get("links")

        nav_list_links = []

        # iterate over links, sorted by title
        if sorted:
            links = sorted(links, key=lambda k: k['title'])

        for link in links:
            active = False
            if group_title == doc and title == link.get('title'):                                
                active = True
               
            nav_list_links.append([
                link.get("href"),
                link.get("title"),
                active
            ])            
        
        nav_groups.append([ group_title, nav_list_links ])

    return {
        "nav_groups": nav_groups,
        "relative_root_path": relative_root_path
    }


def generate_doc_links(src_doc_dir, href):

    doc_links = []

    doc_path = os.path.join(src_doc_dir, href)
    if os.path.isfile(doc_path):
        with nfc.open(doc_path, 'r') as fp:
            doc = etree.parse(fp)
            rel_path = os.path.relpath(
                os.path.dirname(doc_path), 
                start=src_doc_dir)

            for e in doc.getroot().iterchildren():
                # look for href links inside this doc
                href = e.get("href")
                if href:
                    href_path = os.path.join(src_doc_dir, rel_path, href)
                    if os.path.exists(href_path):
                        # load up the external doc/section it references 
                        href_doc = etree.parse(href_path)
                        if href_doc.find("title") is not None:
                            title = get_text(href_doc.find("title"))
                            doc_links.append({
                                "href": os.path.join(rel_path, href).replace('.xml', '.html'),
                                "title": title
                            })

                    else:
                        msg.error("  Invalid href: {}".format(href_path))

    return doc_links

def generate_data_type_links(options):

    data_types_dir = options.get('data_types_dir')  

    data_types = []

    for f in os.listdir(data_types_dir):
        if f.endswith(".json"):
            file = os.path.join(data_types_dir + f)
            with nfc.open(file, 'r') as fp:
                src = nfc.json_load(fp)
                dataType = src['dataType']
                data_types.append({
                    "title": dataType,
                    "href": os.path.join(
                        "reference", 
                        "data_types", 
                        dataType + '.html')
                })

    # sort dataTypes by title
    data_types = sorted(data_types, key=lambda k: k['title'])

    if len(data_types) > 0:
        return ({
            "title": "Data Types",
            "href": "reference/data_types/index.html",
            "links": data_types
        })  

def generate_object_type_links(options):

    object_types_dir = options.get('object_types_dir')        

    if os.path.exists(object_types_dir):
        object_types = []
        for f in os.listdir(object_types_dir):
            if f.endswith(".json"):
                file = os.path.join(object_types_dir + f)
                with nfc.open(file, 'r') as fp:
                    src = nfc.json_load(fp)
                    objectType = src['objectType']
                    object_types.append({
                        "title": objectType,
                        "href": os.path.join(
                            "reference", 
                            "object_types", 
                            objectType + '.html')
                    })

        # sort objectTypes by title
        object_types = sorted(object_types, key=lambda k: k['title'])                    

        if len(object_types) > 0:
            return ({
                "title": "Object Types",
                "href": "reference/object_types/index.html",
                "links": object_types
            })       


def generate_function_links(options):

    func_cats_dir = options.get('func_cats_dir')    

    if os.path.exists(func_cats_dir):

        func_cats = []
        for f in os.listdir(func_cats_dir):
            if f.endswith(".json"):
                file = os.path.join(func_cats_dir + f)
                with nfc.open(file, 'r') as fp:
                    src = nfc.json_load(fp)
                    category = src['category']
                    func_cats.append({
                        "title": category,
                        "href": os.path.join(
                            "reference", 
                            "functions", 
                            category)
                    })

        # sort function categories by title
        func_cats = sorted(func_cats, key=lambda k: k['title'])

        if len(func_cats) > 0:
            return ({
                "title": "Functions",
                "href": "reference/functions/index.html",
                "links": func_cats
            })  

def generate_nav_links(options):

    src_doc_dir = options.get('src_doc_dir')                  

    global sidenav_content

    # look for index.xml
    index_xml = os.path.join(src_doc_dir, "index.xml")
    if os.path.exists(index_xml):
        with nfc.open(index_xml, 'r') as fp:
            docs = etree.parse(fp)

            for category in docs.iterfind("category"):
                label = category.get("label")                
                if not sidenav_content.get(label):
                    sidenav_content[label] = []

                # iterate children of category
                for child in category.iterchildren():
                    href = child.get("href")
                    html_href = None
                    tag = child.tag
                    links = []

                    if tag == "doc":
                        title = child.get("label")     
                        html_href = href.replace(".xml", ".html")

                        # process the href file 
                        links = generate_doc_links(src_doc_dir, href)

                        sidenav_content[label].append({
                            "title": title,
                            "href": html_href,
                            "links": links,
                        })

                    elif tag == "data-type-reference":                        
                        links = generate_data_type_links(options)                        
                        sidenav_content["Reference"].append(links)

                    elif tag == "function-reference":                        
                        links = generate_function_links(options)
                        sidenav_content["Reference"].append(links)

                    elif tag == "object-type-reference":
                        links = generate_object_type_links(options)
                        sidenav_content["Reference"].append(links)

                    elif tag == "doxygen-reference":
                        title = "Doxygen"                    

        if msg.is_debug_mode():
            msg.debug("    Sidenav content:")
            msg.debug(nfc.json_dumps(sidenav_content, indent=4))    
    else:
        msg.error("        Missing index.xml")



def get_text(e):
    text = e.text or ""
    return text.strip()

# simple little function that takes paragraphs that likely came from XML and 
# splits them on \n\n, into logical paragraphs. Then, on each paragraph, it 
# splits on whitespace and rejoins with a single space. This is to ensure that
# the paragraphs are formatted nicely in the HTML output.
def get_paras(text):
    if not text:
        return None

    paras = re.split(r"\n *\n", text)        
    return "\n\n".join(map(lambda x: " ".join(x.split()), paras))

def generate_doc_html_content_or_text(options, cwd, path, docsHtml, level, element):

    content = ""
    for node in element.xpath("child::node()"):            
        if isinstance(node, str):
            content += node                
        else:                   
            content += generate_doc_html_content(
                options, 
                cwd, 
                path, 
                docsHtml,
                node, 
                level)    

    return content

#
# generate_doc_html_content(options, cwd, path, element, level)
#
# This function parses the XML contents and produces HTML
def generate_doc_html_content(options, cwd, path, docsHtml, element, level): 
    
    content = ""        
    if type(element.tag) == str:
        msg.debug("                 Processing child element: " + str(element) + " [" + element.tag + "]")
    else:
        msg.debug("                 Processing child element" + str(element))

    tag = element.tag

    if tag == 'doc':
        for e in element.iterchildren():
            content += generate_doc_html_content(
                options, cwd, path, docsHtml, e, level)

    elif tag == 'description':
        content += docsHtml.div(docsHtml.paragraph(get_text(element)))
        content += docsHtml.spacer()

    elif tag == 'title':                
        if level == 2:
            content += docsHtml.heading(get_text(element), 2)
        elif level > 2:            
            content += docsHtml.heading(get_text(element), 3)                            
        else:
            content += docsHtml.heading(get_text(element))                

    elif tag == 'paragraph':
        # iterate over all text and element nodes
        para_content = ""
        for node in element.xpath("child::node()"):               
            if isinstance(node, str):                
                para_content += node                
            else:                                   
                para_content += generate_doc_html_content(
                    options, 
                    cwd, 
                    path, 
                    docsHtml, 
                    node, 
                    level)                         
        content += docsHtml.paragraph(para_content)

    elif tag == 'literal':
        content += docsHtml.literal(docsHtml.escape(get_text(element)))

    elif tag == 'strong':
        content += docsHtml.strong(get_text(element))        

    elif tag == 'italic':
        content += docsHtml.italic(get_text(element))        

    elif tag == 'section':
        # fixme: if attribute href is present, make this a link to that document
        sections = ""
        if element.get("href"):
            text = element.get("label")
            href = element.get("href")
            content += docsHtml.div(docsHtml.link(text, href.replace('.xml', '.html')))

        else:
            for node in element.iterchildren():            
                sections += generate_doc_html_content(
                    options, cwd, path, docsHtml, node, level + 1)

            content += docsHtml.section(sections)        

    elif tag == 'glossary':       

        rows = []
        if element.find("term") is not None:
            for term in element.iterfind("term"):                    
                identifier = ""
                if term.find("identifier") is not None:
                    identifier = term.find("identifier").text
                definition = ""
                if term.find("definition") is not None:
                    definition = term.find("definition").text
                
                rows.append([ docsHtml.literal(identifier), get_paras(definition) ])                

        content += docsHtml.table(
            [ "Identifier", "Definition" ],
            rows
        )                                   

    elif tag == 'table':            
               
        columns = element.find("columns")
        table_cols = []
        if len(columns) > 0:                                             
            for col in columns.iterfind("column"):  
                table_cols.append(col.text)                                  
        
        rows = element.find("rows")
        table_rows = []
        if len(rows) > 0:            
            for row in rows.iterfind("row"):
                table_row = []
                col_html = ""
                for col in row.iterfind("column"):
                    table_row.append(get_paras(
                        generate_doc_html_content_or_text(options, cwd, path, docsHtml, level, col)
                    ))
                    #table_row.append(get_paras(col.text))
                table_rows.append(table_row)                

        content += docsHtml.table(table_cols, table_rows)        

    elif tag == 'demo':
        content += docsHtml.code(get_text(element))        

    elif tag == 'code':
        if element.get("ref") is not None:
            ref = element.get("ref")

            with nfc.open(cwd + "/" + ref, 'r') as f:
                content += docsHtml.code(f.read())                
        else:                
            content += docsHtml.code(docsHtml.escape(get_text(element)))

    elif tag == 'list':        
        listitems = ""
        for node in element.iterchildren():
            listitems += generate_doc_html_content(
                options, 
                cwd, 
                path, 
                docsHtml,
                node, 
                level + 1)    

        content += docsHtml.list(listitems)      

    elif tag == 'item':

        item_content = ""
        for node in element.xpath("child::node()"):
            if isinstance(node, str):
                item_content += node
            else:
                item_content += generate_doc_html_content(
                    options, 
                    cwd, 
                    path, 
                    docsHtml,
                    node, 
                    level)                         
        content += docsHtml.list_item(item_content)     
    

    elif tag == 'steps':
        content += docsHtml.steps(
            generate_doc_html_content(
                options, 
                cwd, 
                path, 
                docsHtml, 
                element, 
                level + 1
            )
        )

    elif tag == "step":
        content += docsHtml.step(get_text(element))        

    elif tag == 'header':
        if level == 0:
            content += docsHtml.heading(get_text(element))
                
        elif level == 1:
            content += docsHtml.heading(get_text(element), 2)
                
        else:
            content += docsHtml.heading(get_text(element), 3)

    elif tag == 'image':
        src = element.get("generated-src")
        description = element.get("description")
        caption = element.get("caption")

        if src:
            # we need to copy over a generated resource to the output directory
            generated_src = os.path.join(options.get("generated_dir"), src)
            dest_path = os.path.join(path, src)                
            os.makedirs(os.path.dirname(dest_path), exist_ok=True)
            # check if file exists, if not, copy it over                
            if not os.path.exists(dest_path):                        
                if os.path.exists(generated_src):                             
                    shutil.copyfile(generated_src, dest_path)
                else:
                    msg.error("    Could not find generated image source: " + generated_src)

        else:
            src = element.get("src")
            # need to copy over the image to the output directory
            src_path = os.path.join(cwd, src)   
            dest_path = os.path.join(path, src)    
            os.makedirs(os.path.dirname(dest_path), exist_ok=True)         
            
            if os.path.exists(src_path):
                shutil.copyfile(src_path, dest_path)
            else:
                msg.error("    Could not find image source: " + src_path)

        content += docsHtml.image(src, description, caption)

    elif tag == 'link':   
        href = element.get("href")         
        text = element.get("text")
        src = element.get("generated-src")

        if src:
            # we need to copy over a generated resource to the output directory
            generated_src = os.path.join(options.get("generated_dir"), src)
            dest_path = os.path.join(path, src)                
            os.makedirs(os.path.dirname(dest_path), exist_ok=True)
            # check if file exists, if not, copy it over                
            if not os.path.exists(dest_path):                        
                if os.path.exists(generated_src):                             
                    shutil.copyfile(generated_src, dest_path)
                else:
                    msg.error("    Could not find generated link source: " + generated_src)

            href = src

        if text is None:
            text = get_text(element)  

        if not text:
            msg.error("    Could not find 'text' attribute for link: " + href)  

        else:
            content += docsHtml.link(text, href)

    elif tag == 'function-categories-table':
        content += docs_generated.generate_function_categories_table(
            options, 
            docsHtml)

    elif tag == 'data-types-table':
        content += docs_generated.generate_data_types_table(
            options, 
            docsHtml)

    elif tag == 'interfaces-table':
        content += docs_generated.generate_interfaces_table(
            options, 
            docsHtml)
            
    elif tag == 'usage-table':
        content += docs_generated.generate_usage_table(options, docsHtml)

    elif tag == 'object-type-table':
        srcdir = element.get("srcdir")
        objectTypeId = element.get("objectTypeId")
        propertyColumn = element.get("propertyColumn")

        content += docs_generated.generate_object_type_table(
            options, docsHtml, srcdir, objectTypeId, propertyColumn
        )

    elif tag == 'objects-table':
        srcdir = element.get("srcdir")
        objectTypeId = element.get("objectTypeId")
        columns = element.get("columns")

        # fixme: implement

    else:
        if type(element.tag) == str:
            msg.error("        Unknown element: " + element.tag)        

    return content

#
# generate_doc(options, label, href)
# 
# Generate the documentation for an individual doc, found in 
# <srcdir>/doc/index.xml                    
#
def generate_html_doc(docsHtml, options, label, href, doc_label):

    msg.info("    Building Doc from: " + href)
 
    index_html = ""  
    srcdir = options.get('srcdir')
    src_doc_dir = options.get('src_doc_dir')   
    doc_output_dir = options.get('build_directory_docs') 
    html_output_dir = os.path.join(doc_output_dir, srcdir, 'html')

    version = options.get('srcdir_info').get('version')   

    # make output directory
    docs_html_reldir = os.path.relpath(os.path.dirname(os.path.join(html_output_dir, href)))    
    pathlib.Path(docs_html_reldir).mkdir(parents=True, exist_ok=True)
    
    # load XML doc
    index_xml = os.path.join(src_doc_dir, href)
    index_rel_path = os.path.relpath(os.path.dirname(index_xml))    
    if os.path.exists(index_xml):
        with nfc.open(index_xml, 'r') as fp:
            doc = etree.parse(fp)

            index_html = generate_doc_html_content(
                options, index_rel_path, docs_html_reldir, 
                docsHtml, doc.getroot(), 1
            )            

            section_iter, section_peek_iter = itertools.tee(doc.iterfind("section"))
            prev_section = None
            next_section = next(section_peek_iter, None)
            for section in section_iter:
                next_section = next(section_peek_iter, None)                

                # look for external href attributes                
                section_href = os.path.join(index_rel_path, section.get("href"))                
                if section_href:                            
                    with nfc.open(section_href) as input:
                        msg.info("        Processing doc file: " + section_href)

                        section_doc = etree.parse(input)
                        title = ""
                        if section_doc.find("title") is not None:
                            title = section_doc.find("title").text   

                        doc_root = "../../../.."
                        breadcrumbs = docsHtml.breadcrumbs([
                            {
                                "href": doc_root + "/index.html",
                                "text": "Docs"
                            },
                            {
                                "href": "index.html",
                                "text": doc_label
                            },
                            {
                                "text": title
                            }
                        ])                                                                        

                        nav_items = generate_sidenav(docsHtml, "../../", label, doc_label, title)

                        content = generate_doc_html_content(
                            options, 
                            index_rel_path, 
                            docs_html_reldir, 
                            docsHtml, 
                            section_doc.getroot(), 
                            1)
                                                
                        footer = None
                        if next_section is not None and prev_section is not None:                            
                            nav = docsHtml.footer_nav(
                                prev_section.get("href").replace(".xml", ".html"), 
                                prev_section.get("label"), 
                                next_section.get("href").replace(".xml", ".html"), 
                                next_section.get("label")
                            )
                            footer = docsHtml.footer(nav)
                        elif next_section is not None:
                            nav_next = docsHtml.footer_nav_next(
                                next_section.get("href").replace(".xml", ".html"), 
                                next_section.get("label")
                            )
                            footer = docsHtml.footer(nav_next)
                        elif prev_section is not None:
                            nav_prev = docsHtml.footer_nav_prev(
                                prev_section.get("href").replace(".xml", ".html"), 
                                prev_section.get("label")
                            )
                            footer = docsHtml.footer(nav_prev)

                        page_html = docsHtml.page(doc_root, title, breadcrumbs, nav_items, content, None, footer)                        

                        section_output_path = os.path.join(
                            docs_html_reldir, 
                            os.path.basename(section_href).replace(".xml", ".html")
                        )               

                        with nfc.open(section_output_path, 'w') as output:
                            output.write(page_html)
                        
                    prev_section = section

    return index_html
       

#
# generate_docs(options)
#
# This routine looks for <srcdir>/docs/index.xml and generates the 
# documentation for each doc found in the index.
#
def generate_html_docs(options, docsHtml):

    srcdir = options.get('srcdir')
    src_doc_dir = options.get('src_doc_dir')
    doc_output_dir = options.get('build_directory_docs')      

    html_output_dir = os.path.join(doc_output_dir, srcdir, 'html')
    doc_root = "../../../.."      

    # look for index.xml
    index_xml = os.path.join(src_doc_dir, "index.xml")      
    if os.path.exists(index_xml):
        with nfc.open(index_xml, 'r') as fp:
            docs = etree.parse(fp)

            for category in docs.iterfind("category"):
                label = category.get("label")                
                nav_items = generate_sidenav(docsHtml, "../../", label)    

                breadcrumbs = docsHtml.breadcrumbs([
                    {
                        "href": doc_root + "/index.html",
                        "text": "Docs"
                    },
                    {
                        "text": label
                    }
                ])                

                # iterate children of category
                for child in category.iterchildren():
                    doc_href = child.get("href")                      
                    doc_label = child.get("label")     
                    tag = child.tag                    

                    if tag == "doc":           

                        index_html = generate_html_doc(docsHtml, options, label, doc_href, doc_label)                              
                        
                        # write out an index.html
                        index_file_path = os.path.join(
                            html_output_dir, 
                            doc_href.replace(".xml", ".html"))

                        msg.debug("        Writing out index file: " + index_file_path)                        
                        with nfc.open(index_file_path, 'w') as fp:                                                                             
                            fp.write(docsHtml.page(doc_root, doc_label, breadcrumbs, 
                                nav_items, index_html
                            ))

                    elif tag == "data-type-reference":                        
                        docs_generated.generate_data_types(
                            options, 
                            docsHtml, 
                            generate_sidenav)        

                    elif tag == "function-reference":                        
                        docs_generated.generate_function_categories(
                            options, 
                            docsHtml, 
                            generate_sidenav)
                        docs_generated.generate_functions(
                            options, 
                            docsHtml, 
                            generate_sidenav)

                    elif tag == "object-type-reference":
                        docs_generated.generate_object_types(
                            options, 
                            docsHtml, 
                            generate_sidenav)

                    elif tag == "doxygen-reference":
                        pass
        
        
    else:
        msg.error("        Missing index.xml")
   

# removes the folder specified by path, except for any files or folders
# listed in the ignore list
def _doxygen_cache_bust_extra_css(doxygen_html_dir, mtime):
    """Rewrite doxygen-extra.css links in HTML to include ?v=<mtime>."""
    version = str(int(mtime))
    # Match plain or already versioned hrefs
    pattern = re.compile(
        r'href=(["\'])doxygen-extra\.css(?:\?[^"\']*)?\1')
    replacement = r'href=\1doxygen-extra.css?v=' + version + r'\1'
    updated = 0
    for root, _dirs, files in os.walk(doxygen_html_dir):
        for name in files:
            if not name.endswith('.html'):
                continue
            path = os.path.join(root, name)
            try:
                with nfc.open(path, 'r') as fp:
                    text = fp.read()
            except Exception:
                continue
            if 'doxygen-extra.css' not in text:
                continue
            new_text, n = pattern.subn(replacement, text)
            if n and new_text != text:
                with nfc.open(path, 'w') as fp:
                    fp.write(new_text)
                updated += n
    if updated:
        msg.info(
            'Cache-busted doxygen-extra.css in {} HTML link(s) (v={})'.format(
                updated, version))


def _build_python_bindings_docs(options, doc_output_dir, docsHtml):
    """Build the Sphinx (autodoc + napoleon) reference for the generated
    Python bindings (src/afw/generated/python_bindings/). Skipped if that
    generated directory doesn't exist yet, or the output already exists
    (same skip-if-present convention as the Doxygen step above; use
    --docs --clean to force a refresh).

    Sphinx's own theme is discarded and the extracted content is re-wrapped
    in the handbook's own page shell (see _wrap_sphinx_html()) so these
    pages read as part of the site rather than a bolted-on Sphinx site.
    """

    from _afwdev.common import resources

    python_bindings_src = os.path.join(
        options['generated_dir'], 'python_bindings')
    if not os.path.isdir(python_bindings_src):
        return

    sphinx_output_dir = os.path.join(
        doc_output_dir, 'afw', 'html', 'reference', 'bindings',
        'python_bindings')
    if os.path.exists(sphinx_output_dir):
        return

    sphinx_build_cmd = shutil.which('sphinx-build')
    sphinx_apidoc_cmd = shutil.which('sphinx-apidoc')
    if not sphinx_build_cmd or not sphinx_apidoc_cmd:
        msg.warn(
            'Sphinx not found (pip install -r python-requirements.txt); '
            'skipping Python bindings doc build')
        return

    msg.highlighted_info("Running sphinx for Python bindings")

    sphinx_scratch_dir = os.path.join(
        options['build_directory'], 'sphinx', 'python_bindings')
    if os.path.exists(sphinx_scratch_dir):
        shutil.rmtree(sphinx_scratch_dir)

    # Generated binding modules (json.py, random.py, ...) are copied into a
    # package (named afw, merged with the real hand-written afw client
    # package below) so their filenames can't shadow same-named standard
    # library modules when autodoc imports them - see conf.py.
    modules_pkg_dir = os.path.join(sphinx_scratch_dir, 'afw')
    os.makedirs(modules_pkg_dir, exist_ok=True)
    for filename in os.listdir(python_bindings_src):
        if filename.endswith('.py'):
            shutil.copy2(
                os.path.join(python_bindings_src, filename), modules_pkg_dir)

    # Merge in the real afw client package (session.py/request.py/
    # exceptions.py/__init__.py) so the reference documents one unified afw
    # package - afw.Session/afw.HttpRequest alongside afw.adapter/afw.json/
    # etc - instead of a doc-only afw_bindings stand-in. None of these
    # modules do anything beyond class definitions at import time.
    afw_client_src = os.path.join(
        options['afw_package_dir_path'], 'src', 'afw_client', 'python', 'afw')
    for filename in os.listdir(afw_client_src):
        if filename.endswith('.py'):
            shutil.copy2(
                os.path.join(afw_client_src, filename), modules_pkg_dir)

    # Merge in any extension-generated Python bindings too (afw_curl,
    # afw_lmdb, afw_crypto, ...) so they show up in the same unified afw
    # package reference alongside the core bindings.
    for srcdir in package.get_afw_package(options)['srcdirs']:
        if srcdir == 'afw':
            continue
        ext_bindings_src = os.path.join(
            options['afw_package_dir_path'], 'src', srcdir, 'generated',
            'python_bindings')
        if not os.path.isdir(ext_bindings_src):
            continue
        for filename in os.listdir(ext_bindings_src):
            if not filename.endswith('.py'):
                continue
            dest = os.path.join(modules_pkg_dir, filename)
            if os.path.exists(dest):
                msg.warn(
                    '{} from {} would overwrite an existing Python binding '
                    'module of the same name; skipping'.format(
                        filename, srcdir))
                continue
            shutil.copy2(os.path.join(ext_bindings_src, filename), dest)

    resources.copy_resources(
        options, 'sphinx/python_bindings/', todir=sphinx_scratch_dir,
        recursive=True)

    quiet = [] if msg.is_verbose_mode() else ['-q']

    subprocess.call([
        sphinx_apidoc_cmd, '-f', '--doc-project', 'Modules',
        '-o', sphinx_scratch_dir, modules_pkg_dir] + quiet)

    raw_html_dir = os.path.join(sphinx_scratch_dir, '_raw_html')
    subprocess.call([
        sphinx_build_cmd, '-b', 'html',
        sphinx_scratch_dir, raw_html_dir] + quiet)

    _wrap_sphinx_html(
        docsHtml, sphinx_scratch_dir, raw_html_dir, sphinx_output_dir)


def _wrap_sphinx_html(docsHtml, sphinx_scratch_dir, raw_html_dir, sphinx_output_dir):
    """Strip Sphinx's own theme chrome from each raw output page (keeping
    only the <div role="main"> content) and re-render it through the
    handbook's own page() template - same header, breadcrumbs, sidenav,
    afw.css/afw.js - so it looks like a page of the site instead of a
    separate Sphinx site. See sphinx_theme_overrides.css for the color pass
    applied to Sphinx's generated markup (dl.py signatures, field lists).
    """

    from lxml import html as lxml_html

    # basic.css/pygments.css have bare unscoped selectors (a:visited,
    # body, dl, pre, h1..h6, ...) that would otherwise leak past this
    # page's own content and affect the shared site chrome (nav,
    # breadcrumbs) - e.g. a:visited's browser-default purple showing up
    # in the sidenav instead of just within our content. @scope confines
    # them to descendants of .afw-sphinx-content. Sphinx's basic.css has
    # no :root/custom-property block, so it can be scoped wholesale.
    vendor_css = ''
    for name in ('basic.css', 'pygments.css'):
        css_path = os.path.join(raw_html_dir, '_static', name)
        if os.path.isfile(css_path):
            with nfc.open(css_path) as fp:
                vendor_css += fp.read() + '\n'
    override_css = '@scope (.afw-sphinx-content) {\n' + vendor_css + '\n}\n'
    theme_css_path = os.path.join(sphinx_scratch_dir, 'sphinx_theme_overrides.css')
    if os.path.isfile(theme_css_path):
        with nfc.open(theme_css_path) as fp:
            override_css += fp.read()

    # Same sidenav entry ("Reference" > "Bindings" > "Python Bindings") the
    # handwritten reference/bindings/python.xml page already uses, but with
    # an absolute relative_root_path since these pages live at
    # /docs/python_bindings/, not under /docs/afw/html/.
    nav_items = generate_sidenav(
        docsHtml, "/docs/afw/html/", "Reference", "Bindings", "Python Bindings")

    index_href = "/docs/afw/html/reference/bindings/python_bindings/index.html"

    os.makedirs(sphinx_output_dir, exist_ok=True)

    for filename in sorted(os.listdir(raw_html_dir)):
        if not filename.endswith('.html'):
            continue

        tree = lxml_html.parse(os.path.join(raw_html_dir, filename))
        root = tree.getroot()

        main = root.find('.//div[@role="main"]')
        if main is None:
            continue

        inner = main.text or ''
        for child in main:
            inner += lxml_html.tostring(child, encoding='unicode')

        h1 = main.find('.//h1')
        page_title = h1.text_content().strip().rstrip('¶').strip() if h1 is not None else 'Python Bindings'

        if filename == 'index.html':
            breadcrumbs = docsHtml.breadcrumbs([
                {"href": "/docs/index.html", "text": "Docs"},
                {"href": "/docs/afw/html/reference/bindings/index.html", "text": "Bindings"},
                {"text": "Python Bindings"}
            ])
        else:
            breadcrumbs = docsHtml.breadcrumbs([
                {"href": "/docs/index.html", "text": "Docs"},
                {"href": "/docs/afw/html/reference/bindings/index.html", "text": "Bindings"},
                {"href": index_href, "text": "Python Bindings"},
                {"text": page_title}
            ])

        content = '<style>%s</style>\n<div class="afw-sphinx-content">%s</div>' % (
            override_css, inner)

        page_html = docsHtml.page("/docs", page_title, breadcrumbs, nav_items, content)

        with nfc.open(os.path.join(sphinx_output_dir, filename), 'w') as fp:
            fp.write(page_html)


def _build_javascript_bindings_docs(options, doc_output_dir, docsHtml):
    """Build the TypeDoc reference for the generated Javascript bindings
    (src/afw/generated/javascript_bindings/, plus any extension's own
    generated/javascript_bindings/). Skipped if the core one doesn't exist
    yet, or the output already exists (same skip-if-present convention as
    the Doxygen/Python steps above; use --docs --clean to force a refresh).

    Unlike the Python client, the hand-written afw_client/javascript
    classes (AfwClient, AfwStreams, the model/ tree) aren't merged in here:
    they have real cross-file type dependencies (an event-target-style
    mixin, a Node @types/node import) that aren't self-contained the way
    the Python client's session/request/exceptions were, so pulling them in
    surfaced type errors that would need separate work to sort out. Only
    the generated function bindings - self-contained, one category per
    file - are documented here.

    TypeDoc's own theme is discarded the same way Sphinx's was: see
    _wrap_typedoc_html().
    """

    from _afwdev.common import resources

    javascript_bindings_src = os.path.join(
        options['generated_dir'], 'javascript_bindings', 'src')
    if not os.path.isdir(javascript_bindings_src):
        return

    typedoc_output_dir = os.path.join(
        doc_output_dir, 'afw', 'html', 'reference', 'bindings',
        'javascript_bindings')
    if os.path.exists(typedoc_output_dir):
        return

    typedoc_cmd = os.path.join(
        options['afw_package_dir_path'], 'node_modules', '.bin', 'typedoc')
    if not os.path.isfile(typedoc_cmd):
        msg.warn(
            'typedoc not found (npm install); skipping Javascript '
            'bindings doc build')
        return

    msg.highlighted_info("Running typedoc for Javascript bindings")

    typedoc_scratch_dir = os.path.join(
        options['build_directory'], 'typedoc', 'javascript_bindings')
    if os.path.exists(typedoc_scratch_dir):
        shutil.rmtree(typedoc_scratch_dir)

    modules_src_dir = os.path.join(typedoc_scratch_dir, 'src')
    os.makedirs(modules_src_dir, exist_ok=True)
    for filename in os.listdir(javascript_bindings_src):
        if filename.endswith('.ts') and filename != 'index.ts':
            shutil.copy2(
                os.path.join(javascript_bindings_src, filename),
                modules_src_dir)

    # Merge in any extension-generated Javascript bindings too (afw_curl,
    # afw_lmdb, afw_crypto, ...) so they show up in the same reference
    # alongside the core bindings, same as the Python side.
    for srcdir in package.get_afw_package(options)['srcdirs']:
        if srcdir == 'afw':
            continue
        ext_bindings_src = os.path.join(
            options['afw_package_dir_path'], 'src', srcdir, 'generated',
            'javascript_bindings', 'src')
        if not os.path.isdir(ext_bindings_src):
            continue
        for filename in os.listdir(ext_bindings_src):
            if not filename.endswith('.ts') or filename == 'index.ts':
                continue
            dest = os.path.join(modules_src_dir, filename)
            if os.path.exists(dest):
                msg.warn(
                    '{} from {} would overwrite an existing Javascript '
                    'binding module of the same name; skipping'.format(
                        filename, srcdir))
                continue
            shutil.copy2(os.path.join(ext_bindings_src, filename), dest)

    resources.copy_resources(
        options, 'typedoc/javascript_bindings/', todir=typedoc_scratch_dir,
        recursive=True)

    log_level = [] if msg.is_verbose_mode() else ['--logLevel', 'Error']

    raw_html_dir = os.path.join(typedoc_scratch_dir, '_raw_html')
    subprocess.call([
        typedoc_cmd,
        '--entryPoints', modules_src_dir,
        '--entryPointStrategy', 'expand',
        '--tsconfig', os.path.join(typedoc_scratch_dir, 'tsconfig.json'),
        '--name', 'Adaptive Framework Javascript Bindings',
        # Without this, TypeDoc defaults to using the package's own
        # README.md (found by walking up from cwd - the repo root's
        # README.md, in this case) as index.html's content instead of a
        # generated module index.
        '--readme', 'none',
        '--out', raw_html_dir] + log_level)

    _wrap_typedoc_html(
        docsHtml, typedoc_scratch_dir, raw_html_dir, typedoc_output_dir)


def _wrap_typedoc_html(docsHtml, typedoc_scratch_dir, raw_html_dir, typedoc_output_dir):
    """Strip TypeDoc's own theme chrome from each raw output page (keeping
    only the <div class="col-content"> content) and re-render it through
    the handbook's own page() template - same header, breadcrumbs, sidenav,
    afw.css/afw.js - so it looks like a page of the site instead of a
    separate TypeDoc site. See typedoc_theme_overrides.css for the color
    pass applied to TypeDoc's generated markup.
    """

    from lxml import html as lxml_html

    # style.css/highlight.css have bare unscoped selectors (a bare `body`
    # rule among them) that would otherwise leak past this page's own
    # content and affect the shared site chrome - see the matching
    # comment in _wrap_sphinx_html. @scope confines them to descendants
    # of .afw-typedoc-content. Unlike Sphinx, style.css opens with an
    # unconditional `:root { --light-color-*, --dark-color-* }` block
    # defining the raw palette typedoc_theme_overrides.css's --color-*
    # remap reads via var() - :root only ever matches the actual
    # document root, so scoping that block would silently break the
    # whole remap (its rules would just never match). Keep it global
    # (harmless: unused custom property definitions have no visual
    # effect on their own) and scope everything else.
    vendor_css = ''
    for name in ('style.css', 'highlight.css'):
        css_path = os.path.join(raw_html_dir, 'assets', name)
        if os.path.isfile(css_path):
            with nfc.open(css_path) as fp:
                vendor_css += fp.read() + '\n'

    root_block_match = re.match(r'(:root\s*\{[^}]*\})(.*)', vendor_css, re.DOTALL)
    if root_block_match:
        root_block, rest = root_block_match.group(1), root_block_match.group(2)
    else:
        root_block, rest = '', vendor_css

    override_css = root_block + '\n@scope (.afw-typedoc-content) {\n' + rest + '\n}\n'
    theme_css_path = os.path.join(
        typedoc_scratch_dir, 'typedoc_theme_overrides.css')
    if os.path.isfile(theme_css_path):
        with nfc.open(theme_css_path) as fp:
            override_css += fp.read()

    # Same sidenav entry ("Reference" > "Bindings" > "Javascript Bindings")
    # the handwritten reference/bindings/javascript.xml page already uses,
    # but with an absolute relative_root_path since these pages live at
    # /docs/javascript_bindings/, not under /docs/afw/html/.
    nav_items = generate_sidenav(
        docsHtml, "/docs/afw/html/", "Reference", "Bindings",
        "Javascript Bindings")

    index_href = "/docs/afw/html/reference/bindings/javascript_bindings/index.html"

    os.makedirs(typedoc_output_dir, exist_ok=True)

    assets_src = os.path.join(raw_html_dir, 'assets')
    assets_dst = os.path.join(typedoc_output_dir, 'assets')
    if os.path.isdir(assets_src):
        shutil.copytree(assets_src, assets_dst)

    for root, _dirs, files in os.walk(raw_html_dir):
        rel_dir = os.path.relpath(root, raw_html_dir)

        for filename in files:
            if not filename.endswith('.html'):
                continue

            tree = lxml_html.parse(os.path.join(root, filename))
            page_root = tree.getroot()

            content_div = page_root.find('.//div[@class="col-content"]')
            if content_div is None:
                continue

            inner = content_div.text or ''
            for child in content_div:
                inner += lxml_html.tostring(child, encoding='unicode')

            h1 = content_div.find('.//h1')
            page_title = h1.text_content().strip() if h1 is not None else 'Javascript Bindings'

            rel_path = filename if rel_dir == '.' else os.path.join(rel_dir, filename)

            if rel_path == 'index.html':
                breadcrumbs = docsHtml.breadcrumbs([
                    {"href": "/docs/index.html", "text": "Docs"},
                    {"href": "/docs/afw/html/reference/bindings/index.html", "text": "Bindings"},
                    {"text": "Javascript Bindings"}
                ])
            else:
                breadcrumbs = docsHtml.breadcrumbs([
                    {"href": "/docs/index.html", "text": "Docs"},
                    {"href": "/docs/afw/html/reference/bindings/index.html", "text": "Bindings"},
                    {"href": index_href, "text": "Javascript Bindings"},
                    {"text": page_title}
                ])

            content = '<style>%s</style>\n<div class="afw-typedoc-content">%s</div>' % (
                override_css, inner)

            page_html = docsHtml.page("/docs", page_title, breadcrumbs, nav_items, content)

            out_path = os.path.join(typedoc_output_dir, rel_path)
            os.makedirs(os.path.dirname(out_path), exist_ok=True)
            with nfc.open(out_path, 'w') as fp:
                fp.write(page_html)


def rmtree(options, path, ignore = []):

    build_directory_docs = options['build_directory_docs']

    if os.path.exists(path):
        # walk the path and remove all files and folders except those
        # listed in the ignore list
        for root, dirs, files in os.walk(path, topdown=False):
            for name in files:
                skip = False
                for ign in ignore:
                    if os.path.join(build_directory_docs, ign) in os.path.join(root, name):
                        skip = True                
                if not skip:                                    
                    os.remove(os.path.join(root, name))                                  
            for name in dirs:
                skip = False
                for ign in ignore:
                    if os.path.join(build_directory_docs, ign) in os.path.join(root, name) or \
                        os.path.join(root, name) in os.path.join(build_directory_docs, ign):
                        skip = True
                if not skip:             
                    os.rmdir(os.path.join(root, name))
                    
# workaround python 3.7 copytree limitations
def copytree(src, dst, symlinks=False, ignore=None):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            if not os.path.exists(d):
                os.mkdir(d)
            copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)

# run(options)
#
# Main entry point from afwdev.py when invoked via "afwdev docs". This 
# deletes previous built docs and calls the appropriate routines for building 
# docs.
#
def run(options):    

    srcdir = options.get('srcdir')
    objects_dir = options['srcdir_path'] + 'generate/objects/'
    interfaces_dir = options['srcdir_path'] + 'generate/interfaces/'
    generated_dir = options['srcdir_path'] + 'generated/'
    generated_ebnf_syntax = generated_dir + "ebnf/syntax"
    doc_dir = options['afw_package_dir_path'] + 'doc/'        
    src_doc_dir = options['srcdir_path'] + 'doc/'        
    data_types_dir = objects_dir + '_AdaptiveDataTypeGenerate_/'
    functions_dir = objects_dir + '_AdaptiveFunctionGenerate_/'
    polymorphic_functions_dir = objects_dir + '_PolymorphicFunctionGenerate_/'
    func_cats_dir = objects_dir + '_AdaptiveFunctionCategory_/'
    object_types_dir = objects_dir + '_AdaptiveObjectType_/'        

    options['doc_dir'] = doc_dir
    options['src_doc_dir'] = src_doc_dir
    options['objects_dir'] = objects_dir
    options['interfaces_dir'] = interfaces_dir
    options['data_types_dir'] = data_types_dir
    options['functions_dir'] = functions_dir
    options['polymorphic_functions_dir'] = polymorphic_functions_dir
    options['object_types_dir'] = object_types_dir        
    options['func_cats_dir'] = func_cats_dir
    options['generated_dir'] = generated_dir
    options['generated_ebnf_syntax'] = generated_ebnf_syntax      

    src_doc_dir = options.get('src_doc_dir')
    doc_dir = options.get('doc_dir')
    doc_output_dir = options.get('build_directory_docs')

    doxygen_output_dir = doc_output_dir + '/doxygen'
    docsHtml = DocsHtml(options)       

    # check if 'docs' directory exists
    if not os.path.exists(src_doc_dir):        
        return
    
    msg.highlighted_info("Building documentation for {}".format(srcdir))

    global sidenav_content
    global nav_groups

    # reset global variables
    sidenav_content = {}
    nav_groups = ''
    
    doc_output_srcdir = os.path.join(doc_output_dir, srcdir)

    # delete everything under the doc_output_srcdir, except ebnf and the
    # generated bindings references (Sphinx/TypeDoc output, preserved
    # across rebuilds the same way - see _build_python_bindings_docs /
    # _build_javascript_bindings_docs, which skip rebuilding when their
    # output dir already exists)
    rmtree(options, doc_output_srcdir, [
        'afw/html/reference/language/ebnf',
        'afw/html/reference/bindings/python_bindings',
        'afw/html/reference/bindings/javascript_bindings',
    ])

    # the 'afw' srcdir may do some special processing for now
    if options['is_core_afw_package']:        
        if options['srcdir'] == 'afw':
            ebnf_diagrams.build(options)

            # if doxygen folder doesn't exist yet, run doxygen
            # this saves a lot of time if it's already been generated
            if not os.path.exists(doxygen_output_dir):
                # invoke the doxygen command-line
                if msg.is_verbose_mode():
                    doxygen_cmd = "doxygen Doxyfile"
                else:
                    doxygen_cmd = "doxygen Doxyfile > /dev/null 2>&1"
                msg.highlighted_info("Running doxygen")

                subprocess.call(doxygen_cmd, shell=True)

            # Always refresh the skin stylesheet when present. Doxygen is often
            # skipped (folder already exists), so CSS-only edits would otherwise
            # never reach build/docs until --docs --clean. Match Doxyfile
            # HTML_EXTRA_STYLESHEET = src/afw/doc/doxygen-extra.css
            doxygen_html_dir = os.path.join(doxygen_output_dir, 'html')
            extra_css_src = os.path.join(
                options['afw_package_dir_path'],
                'src', 'afw', 'doc', 'doxygen-extra.css')
            extra_css_dst = os.path.join(doxygen_html_dir, 'doxygen-extra.css')
            if os.path.isfile(extra_css_src) and os.path.isdir(doxygen_html_dir):
                shutil.copy2(extra_css_src, extra_css_dst)
                msg.info('Updated ' + extra_css_dst)
                # Cache-bust: browsers often keep doxygen-extra.css on 304 even
                # after a hard refresh of the HTML page. Rewrite link tags to
                # include ?v=<mtime> so each skin edit loads fresh CSS.
                _doxygen_cache_bust_extra_css(
                    doxygen_html_dir, os.path.getmtime(extra_css_src))


    # copy over any static resources
    css = docsHtml.get_html_template("afw.css")
    # write out css
    afw_css = os.path.join(doc_output_dir, "afw.css")        
    with nfc.open(afw_css, 'w') as fp:
        fp.write(css)
    
    js = docsHtml.get_html_template("afw.js")
    # write out js
    afw_js = os.path.join(doc_output_dir, "afw.js")       
    with nfc.open(afw_js, 'w') as fp:
        fp.write(js)

    # touch an empty .nojekyll file
    nojekyll = os.path.join(doc_output_dir, ".nojekyll")
    with nfc.open(nojekyll, 'w') as fp:
        fp.write("")

    # index all documentation into a navigation tree
    generate_nav_links(options)

    # now process every doc
    msg.highlighted_info("    Building HTML resources")
    generate_html_docs(options, docsHtml)

    if options['is_core_afw_package'] and options['srcdir'] == 'afw':
        _build_python_bindings_docs(options, doc_output_dir, docsHtml)
        _build_javascript_bindings_docs(options, doc_output_dir, docsHtml)

    # anything in the 'docs' directory should also be copied over
    # to the output directory
    copytree(doc_dir, doc_output_dir)

##
# @brief Builds documentation.
# @param options The options dictionary.
# @details This is the entry point from "afwdev build" where there is no srcdir 
#          option specified. This routine will iterate over all srcdirs and 
#          build the docs for each.
#
def build(options):

    for srcdir in package.get_afw_package(options)['srcdirs']:
        
        package.set_options_from_existing_package_srcdir(options, srcdir, set_all=True)          

        run(options)

    if options.get("build_install", False):
        msg.highlighted_info("Installing documentation")

        web_root = options.get("build_web_root")
        # FIXME use --sudo, if specified
        pathlib.Path(web_root + '/docs').mkdir(parents=True, exist_ok=True)

        # if any files exist previously, delete them
        # FIXME use --sudo, if specified
        rmtree(options, web_root + '/docs')

        # copy over the docs directory
        # FIXME use --sudo, if specified
        copytree(options.get("build_directory_docs"), web_root + '/docs')