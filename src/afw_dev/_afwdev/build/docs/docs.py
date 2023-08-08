#!/usr/bin/env python3

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
        if isinstance(node, etree._ElementStringResult) or isinstance(node, etree._ElementUnicodeResult):
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
            if isinstance(node, etree._ElementStringResult) or isinstance(node, etree._ElementUnicodeResult):                
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

    elif tag == 'array':        
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
            if isinstance(node, etree._ElementStringResult) or isinstance(node, etree._ElementUnicodeResult):
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

    # delete everything under the doc_output_srcdir, except ebnf
    rmtree(options, doc_output_srcdir, ['afw/html/reference/language/ebnf'])    

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

    # anything in the 'docs' directory should also be copied over
    # to the output directory
    copytree(doc_dir, doc_output_dir)

#
# build(options)
#
# This is the entry point from "afwdev build" where there is no srcdir option
# specified. This routine will iterate over all srcdirs and build the docs for
# each.
#
def build(options):

    for srcdir in package.get_afw_package(options)['srcdirs']:
        
        package.set_options_from_existing_package_srcdir(options, srcdir, set_all=True)          

        run(options)