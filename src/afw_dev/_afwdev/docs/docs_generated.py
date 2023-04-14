# 
# docs_generated.py
#
# This script is intended to be temporary.
#
# The purpose of this script is to create documentation formats from 
# generated resources. Eventually, we would like to abstract this away 
# into a configuration file, generate/docs/config.txt or something.

import os
import pathlib
import re
from lxml import etree
from _afwdev.common import msg, nfc, package
    

# This function generates the page data for an individual dataType
def generate_data_type(fp, src, breadcrumbs, nav_items, docsHtml, doc_root):

    dataType = src["dataType"] 

    msg.info("        Building Reference for Data Type: " + dataType)
   
    intro_para = docsHtml.paragraph(
        "The following is a list of properties for the data type <code>{}</code>:".format(dataType)        
    )

    def map_data_type_field(d):
        return [docsHtml.literal(d[0]),docsHtml.escape(str(d[1])) ]

    data_type_table = docsHtml.table(
        [ "Property", "Value" ],
        list(map(map_data_type_field, src.items()))
    )    
    
    header = docsHtml.heading(dataType)

    content = header + intro_para + data_type_table

    content += docsHtml.spacer()
    content += docsHtml.paragraph(
        "Click " + docsHtml.link("here", "index.html") + 
        " for a complete list of data types and their descriptions."
    )

    fp.write(docsHtml.page(doc_root, dataType, breadcrumbs, nav_items, content))

    # This function generates an individual function category, listing all related functions
def generate_function_category(fp, src, breadcrumbs, nav_items, docsHtml, category_functions, doc_root):

    category = src["category"]

    msg.info("        Building Reference for category: " + category)

    intro_para = docsHtml.paragraph(
        "The following is a list of functions in the category <code>{}</code>".format(category)
    )    

    def map_function(f):       
        return [ 
            docsHtml.link(f['functionId'], f['functionId'] + '.html'), 
            docsHtml.escape(f['description'])
        ]              

    func_table = docsHtml.table(
        [ "Function", "Description" ],
        list(map(map_function, category_functions))
    )    

    header = docsHtml.heading(category)    

    content = header + intro_para + func_table

    fp.write(docsHtml.page(doc_root, category, breadcrumbs, nav_items, content))

# This function generates each dataType page, as well as the top-level index.html for all dataTypes.
def generate_data_types(options, docsHtml, generate_sidenav):

    msg.info("    Building Reference for Data Types") 

    srcdir = options.get('srcdir')
    data_types_dir = options.get('data_types_dir')  
    doc_output_dir = options.get('doc_output_dir')
    html_output_dir = os.path.join(doc_output_dir, srcdir, 'html')

    docs_data_types_dir = os.path.join(html_output_dir, 'reference', 'data_types')

    # create new documentation
    pathlib.Path(docs_data_types_dir).mkdir(parents=True, exist_ok=True)
    
    doc_root = "../../../.."

    dataTypes = []
    for f in os.listdir(data_types_dir):
        if f.endswith(".json"):                
            with nfc.open(data_types_dir + f, 'r') as fp:
                html_file = f.replace('.json', '.html')                
                html_file_path = os.path.join(docs_data_types_dir, html_file)
                with nfc.open(html_file_path, 'w') as fp2:
                    src = nfc.json_load(fp)                

                    breadcrumbs = docsHtml.breadcrumbs([
                        {
                            "href": "../../../../index.html",
                            "text": "Docs"
                        },
                        {
                            "href": "./index.html",
                            "text": "Data Types"
                        },
                        {
                            "text": src["dataType"]
                        }
                    ])                         
                    
                    nav_items = generate_sidenav(docsHtml, "../../", "Reference", "Data Types", src["dataType"])

                    generate_data_type(fp2, src, breadcrumbs, nav_items, docsHtml, doc_root)                    

                    dataTypes.append(src)

    dataTypes.sort(key=lambda x:x['dataType'])
    index_path = os.path.join(docs_data_types_dir, 'index.html')
    with nfc.open(index_path, 'w') as fp:            
        
        breadcrumbs = docsHtml.breadcrumbs([
            {
                "href": "../../../../index.html",
                "text": "Docs",
            },
            {
                "text": "Data Types"
            }
        ])

        intro_para = docsHtml.paragraph("The following is a list of data types, along with their descriptions.")        

        def map_data_type(d):       

            return [
                docsHtml.link(d['dataType'], d['dataType'] + '.html'),
                docsHtml.escape(d['description'])
            ]            

        data_types_table = docsHtml.table(
            [ "Data Type", "Description" ],
            list(map(map_data_type, dataTypes))
        )

        header = docsHtml.heading("Data Types")        

        content = header + intro_para + data_types_table

        fp.write(docsHtml.page(doc_root, "Data Types", breadcrumbs, nav_items, content))

# This function generates an individual function
def generate_function(fp, src, breadcrumbs, nav_items, docsHtml, doc_root):

    functionId = src["functionId"]    

    msg.info("        Building Reference for Function: " + functionId)    

    intro_para = docsHtml.paragraph(
        "The following is a list of properties for the function <code>{}</code>:".format(functionId)        
    )    

    def map_function_field(f):
        val = f[1]           
        if type(val) is dict or type(val) is list:       
            val = nfc.json_dumps(val, indent=4)
        elif type(val) is bool:
            val = str(val).lower()
        
        return [docsHtml.literal(f[0]), docsHtml.escape(str(val)) ]

    func_table = docsHtml.table(
        [ "Property", "Value" ],
        list(map(map_function_field, src.items()))
    )    

    header = docsHtml.heading(functionId)

    content = header + intro_para + func_table    

    fp.write(docsHtml.page(doc_root, functionId, breadcrumbs, nav_items, content))

    # This function generates the page for an object type
def generate_object_type(fp, src, breadcrumbs, nav_items, docsHtml, doc_root):

    objectType = src["objectType"]    

    msg.info("        Building Reference for Object Type: " + objectType)    

    intro_para = docsHtml.paragraph(
        "The following is a list of properties for the object type <code>{}</code>:".format(objectType)        
    )    

    def map_object_type_field(o):
        val = o[1]        
        if type(val) is dict or type(val) is list:
            val = nfc.json_dumps(val, indent=4)
        elif type(val) is bool:
            val = str(val).lower()

        return [
            docsHtml.literal(o[0]),
            docsHtml.escape(str(val))
        ]            

    object_type_table = docsHtml.table(
        [ "Property", "Value" ],
        list(map(map_object_type_field, src.items()))
    )    

    header = docsHtml.heading(objectType)    

    content = header + intro_para + object_type_table

    content += docsHtml.spacer()
    content += docsHtml.paragraph(
        "Click " + docsHtml.link("here", "index.html") + 
        " for a complete list of object types and their descriptions."
    )

    fp.write(docsHtml.page(doc_root, objectType, breadcrumbs, nav_items, content))

# This function generates the page that lists all object types
def generate_object_types(options, docsHtml, generate_sidenav):

    msg.info("    Building Reference for Object Types")

    srcdir = options.get('srcdir')
    object_types_dir = options.get('object_types_dir') 
    doc_output_dir = options.get('doc_output_dir')
    html_output_dir = os.path.join(doc_output_dir, srcdir, 'html')

    docs_object_types_dir = os.path.join(html_output_dir, 'reference', 'object_types')

    # create new documentation
    pathlib.Path(docs_object_types_dir).mkdir(parents=True, exist_ok=True)
    
    doc_root = "../../../.."    

    object_types = []
    for f in os.listdir(object_types_dir):
        if f.endswith(".json"):
            with nfc.open(object_types_dir + f, 'r') as fp:
                src = nfc.json_load(fp)
                html_file = f.replace('.json', '.html')
                html_file_path = os.path.join(docs_object_types_dir, html_file)
                
                with nfc.open(html_file_path, 'w') as fp2:         

                    breadcrumbs = docsHtml.breadcrumbs([
                        {
                            "href": doc_root + "/index.html",
                            "text": "Docs"
                        },
                        {
                            "href": "./index.html",
                            "text": "Object Types"
                        },
                        {
                            "text": src["objectType"]
                        }
                    ])            

                    nav_items = generate_sidenav(docsHtml, "../../", "Reference", "Object Types", src["objectType"])
                    generate_object_type(fp2, src, breadcrumbs, nav_items, docsHtml, doc_root)
                    
                    object_types.append(src)

    object_types.sort(key=lambda x:x['objectType']) 
    index_file_path = os.path.join(docs_object_types_dir, 'index.html')   
    with nfc.open(index_file_path, 'w') as fp:

        breadcrumbs = docsHtml.breadcrumbs([
            {
                "href": doc_root + "/index.html",
                "text": "Docs"
            },
            {
                "text": "Object Types"
            }
        ])        

        intro_para = docsHtml.paragraph("The following is a list of available object types, along with their descriptions.")

        def map_object_type(o):       
            return [
                docsHtml.link(o['objectType'], o['objectType'] + ".html"),
                docsHtml.escape(o.get('description', ''))
            ]            

        object_types_table = docsHtml.table(
            [ "Object Type", "Description" ],
            list(map(map_object_type, object_types))
        )        

        header = docsHtml.heading("Object Types")        

        content = header + intro_para + object_types_table

        fp.write(docsHtml.page(doc_root, "Object Types", breadcrumbs, nav_items, content))

# This function generates the functions list page, with all functions in a particular category
def generate_functions(options, docsHtml, generate_sidenav):

    msg.info("    Building Reference for Functions")

    srcdir = options.get('srcdir')
    functions_dir = options.get('functions_dir')   
    doc_output_dir = options.get('doc_output_dir')
    html_output_dir = os.path.join(doc_output_dir, srcdir, 'html')

    docs_functions_dir = os.path.join(html_output_dir, 'reference', 'functions')

    # create new documentation
    pathlib.Path(docs_functions_dir).mkdir(parents=True, exist_ok=True)    
    
    functions = []
    for f in os.listdir(functions_dir):
        if f.endswith(".json"):
            with nfc.open(functions_dir + f, 'r') as fp:
                src = nfc.json_load(fp)
                category = src['category']
                html_file = f.replace('.json', '.html')
                html_file_path = os.path.join(docs_functions_dir, category, html_file)
                with nfc.open(html_file_path, 'w') as fp2:

                    nav_items = generate_sidenav(docsHtml, "../../../", "Reference", "Functions", category)

                    breadcrumbs = docsHtml.breadcrumbs([
                        {
                            "href": "../../../../../index.html",
                            "text": "Docs"
                        },
                        {
                            "href": "../index.html",
                            "text": "Functions"
                        },
                        {
                            "href": "index.html",
                            "text": category
                        },
                        {
                            "text": src["functionId"]
                        }
                    ]) 
                    doc_root = "../../../../.."                    

                    generate_function(fp2, src, breadcrumbs, nav_items, docsHtml, doc_root)
                    
                    functions.append(src)


# This function generates a list of all function categories
def generate_function_categories(options, docsHtml, generate_sidenav):

    msg.info("    Building Reference for Function Categories")    

    srcdir = options.get('srcdir')
    func_cats_dir = options.get('func_cats_dir')
    functions_dir = options.get('functions_dir')  
    doc_output_dir = options.get('doc_output_dir')
    html_output_dir = os.path.join(doc_output_dir, srcdir, 'html')

    docs_func_cats_dir = os.path.join(html_output_dir, 'reference', 'functions')

    # create new documentation
    pathlib.Path(docs_func_cats_dir).mkdir(parents=True, exist_ok=True)
    
    doc_root = "../../../../.."

    funcCats = []
    for f in os.listdir(func_cats_dir):        
        if f.endswith(".json"):                
            with nfc.open(func_cats_dir + f, 'r') as fp:
                src = nfc.json_load(fp)
                category = src['category']

                msg.info("        Building Reference for Function Category: " + category)

                pathlib.Path(docs_func_cats_dir + "/" + category).mkdir(parents=True, exist_ok=True)                    
                funcCats.append(src)

                nav_items = generate_sidenav(docsHtml, "../../../", "Reference", "Functions", category)
                
                if os.path.exists(functions_dir):
                    # find functions for this category        
                    category_functions = []

                    for f2 in os.listdir(functions_dir):
                        if f2.endswith(".json"):                
                            with nfc.open(functions_dir + f2, 'r') as fp2:
                                src2 = nfc.json_load(fp2)
                                if category == src2['category']:
                                    category_functions.append(src2)

                    category_functions.sort(key=lambda x:x['functionId'])  

                    breadcrumbs = docsHtml.breadcrumbs([
                        {
                            "href": "../../../../../index.html",
                            "text": "Docs"
                        },
                        {
                            "href": "../index.html",
                            "text": "Functions"
                        },
                        {
                            "text": category
                        }
                    ])                                      

                    index_file_path = os.path.join(docs_func_cats_dir, category, 'index.html')
                    with nfc.open(index_file_path, 'w') as fp2:            
                        
                        generate_function_category(fp2, src, breadcrumbs, nav_items, docsHtml, category_functions, doc_root)                        
                                    

    funcCats.sort(key=lambda x:x['category'])
    index_file_path = os.path.join(docs_func_cats_dir, 'index.html')

    with nfc.open(index_file_path, 'w') as fp:

        nav_items = generate_sidenav(docsHtml, "../../", "Reference")
        breadcrumbs = docsHtml.breadcrumbs([
            {
                "href": "../../../../index.html",
                "text": "Docs"
            },
            {
                "text": "Functions"
            }
        ])        
        doc_root = "../../../.."

        intro_para = docsHtml.paragraph("The following is a list of function categories.")        

        def map_function_category(c):       
            return [
                docsHtml.link(c['category'], c['category']),
                docsHtml.escape(c['description'])
            ]
            
        categories_table = docsHtml.table(
            [ "Category", "Description" ],
            list(map(map_function_category, funcCats))
        )        

        header = docsHtml.heading("Function Categories")        

        content = header + intro_para + categories_table

        fp.write(docsHtml.page(doc_root, "Function Categories", breadcrumbs, nav_items, content))            


def generate_function_categories_table(options, docsHtml):

    func_cats_dir = options.get('func_cats_dir')    

    content = ""
    rows = []
    for category in os.listdir(func_cats_dir):
        if category.endswith(".json"):
            src = nfc.json_load_from_file(os.path.join(func_cats_dir, category))
            category = src['category']
            description = src['description']

            rows.append([ docsHtml.literal(category), docsHtml.escape(description)] )

    if len(rows) > 0:
        content = docsHtml.table( [ "Category", "Description" ], rows)        

    return content


def generate_data_types_table(options, docsHtml):

    data_types_dir = options.get('data_types_dir')    

    content = ""
    rows = []
    for d in os.listdir(data_types_dir):
        if d.endswith(".json"):
            src = nfc.json_load_from_file(os.path.join(data_types_dir, d))
            dataType = src['dataType']
            description = src['description']

            rows.append([ docsHtml.literal(dataType),docsHtml.escape(description)] )

    if len(rows) > 0:
        content = docsHtml.table( [ "Data Type", "Description" ], rows)        

    return content

# simple little function that takes paragraphs that likely came from XML and 
# splits them on \n\n, into logical paragraphs. Then, on each paragraph, it 
# splits on whitespace and rejoins with a single space. This is to ensure that
# the paragraphs are formatted nicely in the HTML output.
def get_paras(text):
    if not text:
        return None

    paras = re.split(r"\n *\n", text)        
    return "\n\n".join(map(lambda x: " ".join(x.split()), paras))

def generate_interfaces_table(options, docsHtml):

    interfaces_dir = options.get('interfaces_dir')      

    rows = []
    for f in os.listdir(interfaces_dir):        
        if f.endswith(".xml"):            
            with nfc.open(os.path.join(interfaces_dir, f), 'r') as fp:                
                interfaces = etree.parse(fp)
                
                for interface in interfaces.findall(".//interface"):                    
                    name = interface.get("name")
                    description = ""

                    for descr in interface.iterfind("description"):
                        description = descr.text.strip()                    

                    rows.append([ docsHtml.literal(name), docsHtml.escape(get_paras(description))] )

    if len(rows) > 0:
        content = docsHtml.table( [ "Interface", "Description" ], rows)        

    return content

# For the afw_dev srcdir, this routine generates the Usage section of the 
# documentation.
#
# It could be generalized to other command-line usage options in the future
def generate_usage_table(options, docsHtml): 

    afwdev_info = options.get('afwdev_info')    
    content = ""   

    # process the overall info description
    content += docsHtml.paragraph(afwdev_info.get('overall').get("description"))
    content += docsHtml.hr()

    # process the general options
    content += docsHtml.heading("General Options", 2)        
    args = []
    for option in afwdev_info.get('args_for_all_commands'):
        optionName = option.get('optionName')
        arg = option.get('arg')
        help = option.get('help')

        args.append([ docsHtml.literal(arg),docsHtml.span(help) ])        

    if len(args) > 0:
        content += docsHtml.paragraph(
            "The following are a list of general options, which may be used for all subcommands:"
        )   
        content += docsHtml.table([ "Option", "Description" ], args)        

        content += docsHtml.spacer()

    # process the subcommands
    content += docsHtml.heading("Subcommands", 2)        
    subcommands = []
    for subcommand_info in afwdev_info.get('subcommands'):
        subcommand = subcommand_info.get('subcommand')
        help = subcommand_info.get('help')

        subcommands.append([ docsHtml.literal(subcommand),docsHtml.span(help) ])
    
    if len(subcommands) > 0:
        content += docsHtml.paragraph("The following subcommands are available:") 
       
        content += docsHtml.table([ "Subcommand", "Description" ], subcommands)

        content += docsHtml.spacer()

    # for each subcommand, process the subcommand options
    for subcommand_info in afwdev_info.get('subcommands'):
        subcommand = subcommand_info.get('subcommand')
        description = subcommand_info.get('description')
        args = subcommand_info.get('args')

        content += docsHtml.heading(subcommand, 3) 
        content += docsHtml.paragraph(description)               

        rows = []        
        for arg in args:
            arg_arg = arg.get('arg')
            arg_help = arg.get('help')

            if not arg_arg.startswith("--"):
                arg_arg = "&lt;" + arg_arg + "&gt;"

            rows.append([ docsHtml.literal(arg_arg),docsHtml.span(arg_help) ])            

        if len(rows) > 0:
            content += docsHtml.paragraph( 
                docsHtml.italic(
                    "The following options may be used for the <code>%s</code> subcommand:" % subcommand
                ),
            )          
            content += docsHtml.table(
                [ "Option", "Description" ],
                rows,
            )

            content += docsHtml.spacer()

    # process the overall info epilog
    content += docsHtml.heading("Examples", 2)    
    content += docsHtml.code(docsHtml.escape(afwdev_info.get('overall').get("epilog")))

    return content

def generate_object_type_table(options, docsHtml, srcdir, objectTypeId, propertyColumn):

    generated_object_type_path = options.get('generated_dir') + 'objects/_AdaptiveObjectType_/' + objectTypeId + '.json'
    content = ""

    if srcdir:
        if package.get_afw_package(options)['srcdirs'].get(srcdir, None):
            srcdirPath = package.get_afw_package(options)['srcdirs'].get(srcdir).get('srcdirPath')
            generated_object_type_path = options.get('afw_package_dir_path') + \
                srcdirPath + 'generated/objects/_AdaptiveObjectType_/' + objectTypeId + '.json'            
    
    if os.path.exists(generated_object_type_path):
        with nfc.open(generated_object_type_path, 'r') as fp:
            src = nfc.json_load(fp)

            rows = []
            propertyTypes = src['propertyTypes']               
            for pType in propertyTypes:
                if pType == "_meta_":
                    continue
                rows.append([ docsHtml.literal(pType), docsHtml.escape(propertyTypes[pType].get('description')) ])
            
            if len(rows) > 0:
                content = docsHtml.table( 
                    [ 
                        propertyColumn if propertyColumn else "Property", 
                        "Description" 
                    ], 
                    rows
                )
    else:
        msg.error("Path does not exist: %s" % generated_object_type_path)


    return content