// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Const
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/*  ----------------------------- N O T E -------------------------------------
 *
 * This file is generated by "afwdev generate afw_vfs".
 *
 * Do not change this file directly or the changes will be lost the next time
 * this file is regenerated.
 *
 * -------------------------------------------------------------------------*/



/**
 * @file afw_vfs_const_objects.c
 * @brief Adaptive Framework builtin objects.
 */

#include "afw.h"
#include "afw_vfs_generated.h"


static const afw_runtime_unresolved_const_object_t
impl_1;

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_2;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects/otherProperties
 */

static const afw_runtime_unresolved_property_t
impl_2_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Custom property"
    }
};

static const afw_runtime_unresolved_property_t
impl_2_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Custom property."
    }
};

static const afw_runtime_unresolved_property_t *
impl_2_properties[] = {
    &impl_2_property_brief,
    &impl_2_property_description,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_2 = {
    &impl_2_properties[0]
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_3;

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_4;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects/propertyTypes/includeHidden
 */

static const afw_runtime_unresolved_property_t
impl_4_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Include"
    }
};

static const afw_runtime_unresolved_property_t
impl_4_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "boolean"
    }
};

static const afw_runtime_unresolved_property_t
impl_4_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "If true, filenames that begin with dot ('.') will be included."
    }
};

static const afw_runtime_unresolved_property_t
impl_4_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Include Hidden"
    }
};

static const afw_runtime_unresolved_property_t
impl_4_property_required = {
    "required",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t *
impl_4_properties[] = {
    &impl_4_property_brief,
    &impl_4_property_dataType,
    &impl_4_property_description,
    &impl_4_property_label,
    &impl_4_property_required,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_4 = {
    &impl_4_properties[0]
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_5;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects/propertyTypes/recursive
 */

static const afw_runtime_unresolved_property_t
impl_5_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Recursively traverse directories"
    }
};

static const afw_runtime_unresolved_property_t
impl_5_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "boolean"
    }
};

static const afw_runtime_unresolved_property_t
impl_5_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "If true, directories will be recursively traversed."
    }
};

static const afw_runtime_unresolved_property_t
impl_5_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Recursive"
    }
};

static const afw_runtime_unresolved_property_t
impl_5_property_required = {
    "required",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t *
impl_5_properties[] = {
    &impl_5_property_brief,
    &impl_5_property_dataType,
    &impl_5_property_description,
    &impl_5_property_label,
    &impl_5_property_required,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_5 = {
    &impl_5_properties[0]
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_6;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects/propertyTypes/subdirectory
 */

static const afw_runtime_unresolved_property_t
impl_6_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Subdirectory"
    }
};

static const afw_runtime_unresolved_property_t
impl_6_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "string"
    }
};

static const afw_runtime_unresolved_property_t
impl_6_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "If specified, the retrieve will start in the specified vfs subdirectory instead of the vfs root. The value can not start with '/', must end with '/', and can not contain '\\', './' or '../'."
    }
};

static const afw_runtime_unresolved_property_t
impl_6_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Subdirectory"
    }
};

static const afw_runtime_unresolved_property_t
impl_6_property_required = {
    "required",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t *
impl_6_properties[] = {
    &impl_6_property_brief,
    &impl_6_property_dataType,
    &impl_6_property_description,
    &impl_6_property_label,
    &impl_6_property_required,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_6 = {
    &impl_6_properties[0]
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_7;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects/propertyTypes/suffix
 */

static const afw_runtime_unresolved_property_t
impl_7_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Filename suffix"
    }
};

static const afw_runtime_unresolved_property_t
impl_7_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "string"
    }
};

static const afw_runtime_unresolved_property_t
impl_7_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "If specified, only files with filenames that end with the specified suffix are included."
    }
};

static const afw_runtime_unresolved_property_t
impl_7_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Filename Suffix"
    }
};

static const afw_runtime_unresolved_property_t
impl_7_property_required = {
    "required",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t *
impl_7_properties[] = {
    &impl_7_property_brief,
    &impl_7_property_dataType,
    &impl_7_property_description,
    &impl_7_property_label,
    &impl_7_property_required,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_7 = {
    &impl_7_properties[0]
};

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects/propertyTypes
 */

static const afw_runtime_unresolved_property_t
impl_3_property_includeHidden = {
    "includeHidden",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_4
    }
};

static const afw_runtime_unresolved_property_t
impl_3_property_recursive = {
    "recursive",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_5
    }
};

static const afw_runtime_unresolved_property_t
impl_3_property_subdirectory = {
    "subdirectory",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_6
    }
};

static const afw_runtime_unresolved_property_t
impl_3_property_suffix = {
    "suffix",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_7
    }
};

static const afw_runtime_unresolved_property_t *
impl_3_properties[] = {
    &impl_3_property_includeHidden,
    &impl_3_property_recursive,
    &impl_3_property_subdirectory,
    &impl_3_property_suffix,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_3 = {
    &impl_3_properties[0]
};

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects
 */

static const afw_runtime_unresolved_property_t
impl_1_property_allowAdd = {
    "allowAdd",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t
impl_1_property_allowChange = {
    "allowChange",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t
impl_1_property_allowDelete = {
    "allowDelete",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t
impl_1_property_allowEntity = {
    "allowEntity",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_utf8_t
impl_1_array_collectionURIs[] = {
    AFW_UTF8_LITERAL("/afw/_AdaptiveCollection_/vfs_extension")
};

static const afw_runtime_unresolved_const_array_t
impl_1_list_collectionURIs = {
    AFW_UTF8_LITERAL("string"),
    sizeof(impl_1_array_collectionURIs) / sizeof(afw_utf8_t),
    (const void *)&impl_1_array_collectionURIs
};

static const afw_runtime_unresolved_property_t
impl_1_property_collectionURIs = {
    "collectionURIs",
    {
        .type = afw_runtime_unresolved_primitive_type_array,
        .array = &impl_1_list_collectionURIs
    }
};

static const afw_runtime_unresolved_property_t
impl_1_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "This is the object type for the optional adaptorTypeSpecific parameter of the retrieve_objects() adaptive function when the adaptorType of the adaptorId is vfs."
    }
};

static const afw_runtime_unresolved_property_t
impl_1_property_objectType = {
    "objectType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "_AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects"
    }
};

static const afw_runtime_unresolved_property_t
impl_1_property_otherProperties = {
    "otherProperties",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_2
    }
};

static const afw_runtime_unresolved_property_t
impl_1_property_propertyTypes = {
    "propertyTypes",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_3
    }
};

static const afw_runtime_unresolved_property_t *
impl_1_properties[] = {
    &impl_1_property_allowAdd,
    &impl_1_property_allowChange,
    &impl_1_property_allowDelete,
    &impl_1_property_allowEntity,
    &impl_1_property_collectionURIs,
    &impl_1_property_description,
    &impl_1_property_objectType,
    &impl_1_property_otherProperties,
    &impl_1_property_propertyTypes,
    NULL
};

static const afw_utf8_t
impl_1_meta_path =
    AFW_UTF8_LITERAL("/afw/_AdaptiveObjectType_/_AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects");

static const afw_runtime_unresolved_const_object_t
impl_1 = {
    &afw_vfs_self_s__AdaptiveObjectType_,
    &afw_vfs_self_s__AdaptiveAdaptorTypeSpecific_vfs_retrieve_objects,
    &impl_1_meta_path,
    &impl_1_properties[0]
};

static const afw_runtime_unresolved_const_object_t
impl_8;

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_9;

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_10;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveConf_adaptor_vfs/propertyTypes/markExecutable
 */

static const afw_runtime_unresolved_property_t
impl_10_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "vfsPath filename patterns to mark as executable"
    }
};

static const afw_runtime_unresolved_property_t
impl_10_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "array"
    }
};

static const afw_runtime_unresolved_property_t
impl_10_property_dataTypeParameter = {
    "dataTypeParameter",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "string"
    }
};

static const afw_runtime_unresolved_property_t
impl_10_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "This is a list of vfsPath filename patterns whose host files should be marked as executable when written. For instance, ['*.as'] will mark Adaptive Scripts whose vfsPath ends with '.as' as executable.\n\nA PATTERN match uses Apache apr function apr_fnmatch() which interprets a pattern as follows:\n\nThere are no illegal patterns.\n\nPATTERN: Backslash followed by any character, including another backslash.\nMATCHES: That character exactly.\n\nPATTERN: ?\nMATCHES: Any single character.\n\nPATTERN: *\nMATCHES: Any sequence of zero or more characters. (Note that multiple *s in a row are equivalent to one.)\n\nPATTERN: Any character other than \\?*[ or a \\ at the end of the pattern\nMATCHES: That character exactly. (Case sensitive.)\n\nPATTERN: [ followed by a class description followed by ]\nMATCHES: A single character described by the class description. (Never matches, if the class description reaches until the end of the string without a ].) If the first character of the class description is ^ or !, the sense of the description is reversed. The rest of the class description is a list of single characters or pairs of characters separated by -. Any of those characters can have a backslash in front of them, which is ignored; this lets you use the characters ] and - in the character class, as well as ^ and ! at the beginning. The pattern matches a single character if it is one of the listed characters or falls into one of the listed ranges (inclusive, case sensitive). Ranges with the first character larger than the second are legal but never match. Edge cases: [] never matches, and [^] and [!] always match without consuming a character.\n\nNote that these patterns attempt to match the entire vfsPath, not just find a substring matching the pattern."
    }
};

static const afw_runtime_unresolved_property_t
impl_10_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Mark Executable"
    }
};

static const afw_runtime_unresolved_property_t *
impl_10_properties[] = {
    &impl_10_property_brief,
    &impl_10_property_dataType,
    &impl_10_property_dataTypeParameter,
    &impl_10_property_description,
    &impl_10_property_label,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_10 = {
    &impl_10_properties[0]
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_11;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveConf_adaptor_vfs/propertyTypes/vfsMap
 */

static const afw_runtime_unresolved_property_t
impl_11_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "VFS to operating system directory map"
    }
};

static const afw_runtime_unresolved_property_t
impl_11_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "array"
    }
};

static const afw_runtime_unresolved_property_t
impl_11_property_dataTypeParameter = {
    "dataTypeParameter",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "string"
    }
};

static const afw_runtime_unresolved_property_t
impl_11_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "This property is a list of vfs map entries.\n\nThe vfsPath of objects in this vfs adaptor is of the form:\n\n/<adaptorId>/<objectId>\n\nWhere <adaptorId> is the id of this adaptor and where <objectId> is the relative path from the vfs root directory ('<adaptorId>/') to the associated file or directory. An <objectId> of a directory ends with a slash ('/').\n\nEach vfsMap entry is a string of the form:\n\n<objectId_prefix>=<host file system directory path>\n\n<objectId_prefix> can be empty if the entry applies to files and directories in the vfs root directory or the objectId of a directory within the vfs.\n\nThe <host file system directory path> must end in a slash ('/'). A backslash is tolerated on a Windows host. The host system file path is the <objectId> with <objectId_prefix> removed appended to the associated <host file system directory path>.\n\nThe map entry with the longest matching <objectId_prefix> is the one used for any adaptor functions objectId parameter.\n\nFor retrieve_objects*() the entries are processed in longest to shortest <objectId_prefix> order. If 'subdirectory' is supplied in the retrieve_objects*() adaptorSpecific parameter, only matching entries are used. Note, that depending on the vfsMap, a single physical file or directory can appear multiple times with different vfsPaths."
    }
};

static const afw_runtime_unresolved_property_t
impl_11_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "VFS Map"
    }
};

static const afw_runtime_unresolved_property_t
impl_11_property_required = {
    "required",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = true
    }
};

static const afw_runtime_unresolved_property_t *
impl_11_properties[] = {
    &impl_11_property_brief,
    &impl_11_property_dataType,
    &impl_11_property_dataTypeParameter,
    &impl_11_property_description,
    &impl_11_property_label,
    &impl_11_property_required,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_11 = {
    &impl_11_properties[0]
};

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveConf_adaptor_vfs/propertyTypes
 */

static const afw_runtime_unresolved_property_t
impl_9_property_markExecutable = {
    "markExecutable",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_10
    }
};

static const afw_runtime_unresolved_property_t
impl_9_property_vfsMap = {
    "vfsMap",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_11
    }
};

static const afw_runtime_unresolved_property_t *
impl_9_properties[] = {
    &impl_9_property_markExecutable,
    &impl_9_property_vfsMap,
    NULL
};

static const afw_utf8_t
impl_9_parentPaths_array[] = {
    AFW_UTF8_LITERAL("/afw/_AdaptiveObjectType_/_AdaptiveConf_adaptor/propertyTypes")
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_9 = {
    &impl_9_properties[0],
    sizeof(impl_9_parentPaths_array) / sizeof(afw_utf8_t),
    &impl_9_parentPaths_array[0]
};

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveConf_adaptor_vfs
 */

static const afw_runtime_unresolved_property_t
impl_8_property_allowEntity = {
    "allowEntity",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_utf8_t
impl_8_array_collectionURIs[] = {
    AFW_UTF8_LITERAL("/afw/_AdaptiveCollection_/vfs_extension")
};

static const afw_runtime_unresolved_const_array_t
impl_8_list_collectionURIs = {
    AFW_UTF8_LITERAL("string"),
    sizeof(impl_8_array_collectionURIs) / sizeof(afw_utf8_t),
    (const void *)&impl_8_array_collectionURIs
};

static const afw_runtime_unresolved_property_t
impl_8_property_collectionURIs = {
    "collectionURIs",
    {
        .type = afw_runtime_unresolved_primitive_type_array,
        .array = &impl_8_list_collectionURIs
    }
};

static const afw_runtime_unresolved_property_t
impl_8_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Adaptive Framework configuration component for type=adaptor adaptorType=vfs. This is a virtual file system adaptor that allows files to be accessed as objects that have a 'data' property and additional file stats properties. The vfs path to files begins with '/', the adaptor name, '/_AdaptiveFile_vfs'. The path of the operating system directory as well as the paths to subdirectories are defined in the 'vfsMap' property."
    }
};

static const afw_runtime_unresolved_property_t
impl_8_property_objectType = {
    "objectType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "_AdaptiveConf_adaptor_vfs"
    }
};

static const afw_runtime_unresolved_property_t
impl_8_property_propertyTypes = {
    "propertyTypes",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_9
    }
};

static const afw_runtime_unresolved_property_t *
impl_8_properties[] = {
    &impl_8_property_allowEntity,
    &impl_8_property_collectionURIs,
    &impl_8_property_description,
    &impl_8_property_objectType,
    &impl_8_property_propertyTypes,
    NULL
};

static const afw_utf8_t
impl_8_meta_path =
    AFW_UTF8_LITERAL("/afw/_AdaptiveObjectType_/_AdaptiveConf_adaptor_vfs");

static const afw_runtime_unresolved_const_object_t
impl_8 = {
    &afw_vfs_self_s__AdaptiveObjectType_,
    &afw_vfs_self_s__AdaptiveConf_adaptor_vfs,
    &impl_8_meta_path,
    &impl_8_properties[0]
};

static const afw_runtime_unresolved_const_object_t
impl_12;

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_13;

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_14;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveFile_vfs/propertyTypes/data
 */

static const afw_runtime_unresolved_property_t
impl_14_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Contents of file"
    }
};

static const afw_runtime_unresolved_property_t
impl_14_property_defaultValue = {
    "defaultValue",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = ""
    }
};

static const afw_runtime_unresolved_property_t
impl_14_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "This is the contents of the file. If this is a directory, data is a list of strings containing the name from its entries. If this is not a directory and the data is valid utf-8, data is a string otherwise, it is hexBinary. This property is ignored when adding a directory."
    }
};

static const afw_runtime_unresolved_property_t
impl_14_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Data"
    }
};

static const afw_runtime_unresolved_property_t
impl_14_property_required = {
    "required",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t *
impl_14_properties[] = {
    &impl_14_property_brief,
    &impl_14_property_defaultValue,
    &impl_14_property_description,
    &impl_14_property_label,
    &impl_14_property_required,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_14 = {
    &impl_14_properties[0]
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_15;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveFile_vfs/propertyTypes/isDirectory
 */

static const afw_runtime_unresolved_property_t
impl_15_property_allowWrite = {
    "allowWrite",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t
impl_15_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Is directory"
    }
};

static const afw_runtime_unresolved_property_t
impl_15_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "boolean"
    }
};

static const afw_runtime_unresolved_property_t
impl_15_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "This is true if this is a directory. This property is read-only. A slash ('/') at the end of an objectId is used to indicate this in requests."
    }
};

static const afw_runtime_unresolved_property_t
impl_15_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Is Directory"
    }
};

static const afw_runtime_unresolved_property_t *
impl_15_properties[] = {
    &impl_15_property_allowWrite,
    &impl_15_property_brief,
    &impl_15_property_dataType,
    &impl_15_property_description,
    &impl_15_property_label,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_15 = {
    &impl_15_properties[0]
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_16;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveFile_vfs/propertyTypes/timeAccessed
 */

static const afw_runtime_unresolved_property_t
impl_16_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Time last accessed"
    }
};

static const afw_runtime_unresolved_property_t
impl_16_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "dateTime"
    }
};

static const afw_runtime_unresolved_property_t
impl_16_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "This is the time the file was last accessed."
    }
};

static const afw_runtime_unresolved_property_t
impl_16_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Time Accessed"
    }
};

static const afw_runtime_unresolved_property_t
impl_16_property_required = {
    "required",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t *
impl_16_properties[] = {
    &impl_16_property_brief,
    &impl_16_property_dataType,
    &impl_16_property_description,
    &impl_16_property_label,
    &impl_16_property_required,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_16 = {
    &impl_16_properties[0]
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_17;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveFile_vfs/propertyTypes/timeCreated
 */

static const afw_runtime_unresolved_property_t
impl_17_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Time created"
    }
};

static const afw_runtime_unresolved_property_t
impl_17_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "dateTime"
    }
};

static const afw_runtime_unresolved_property_t
impl_17_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "This is the time the file was created."
    }
};

static const afw_runtime_unresolved_property_t
impl_17_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Time Created"
    }
};

static const afw_runtime_unresolved_property_t
impl_17_property_required = {
    "required",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t *
impl_17_properties[] = {
    &impl_17_property_brief,
    &impl_17_property_dataType,
    &impl_17_property_description,
    &impl_17_property_label,
    &impl_17_property_required,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_17 = {
    &impl_17_properties[0]
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_18;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveFile_vfs/propertyTypes/timeModified
 */

static const afw_runtime_unresolved_property_t
impl_18_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Time last modified"
    }
};

static const afw_runtime_unresolved_property_t
impl_18_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "dateTime"
    }
};

static const afw_runtime_unresolved_property_t
impl_18_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "This is the time the file was last modified."
    }
};

static const afw_runtime_unresolved_property_t
impl_18_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "Time Modified"
    }
};

static const afw_runtime_unresolved_property_t
impl_18_property_required = {
    "required",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t *
impl_18_properties[] = {
    &impl_18_property_brief,
    &impl_18_property_dataType,
    &impl_18_property_description,
    &impl_18_property_label,
    &impl_18_property_required,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_18 = {
    &impl_18_properties[0]
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_19;

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveFile_vfs/propertyTypes/vfsPath
 */

static const afw_runtime_unresolved_property_t
impl_19_property_allowWrite = {
    "allowWrite",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = false
    }
};

static const afw_runtime_unresolved_property_t
impl_19_property_brief = {
    "brief",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "VFS path"
    }
};

static const afw_runtime_unresolved_property_t
impl_19_property_dataType = {
    "dataType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "anyURI"
    }
};

static const afw_runtime_unresolved_property_t
impl_19_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "This is the vfs path within this instance of Adaptive Framework. This property is read-only."
    }
};

static const afw_runtime_unresolved_property_t
impl_19_property_label = {
    "label",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "VFS Path"
    }
};

static const afw_runtime_unresolved_property_t *
impl_19_properties[] = {
    &impl_19_property_allowWrite,
    &impl_19_property_brief,
    &impl_19_property_dataType,
    &impl_19_property_description,
    &impl_19_property_label,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_19 = {
    &impl_19_properties[0]
};

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveFile_vfs/propertyTypes
 */

static const afw_runtime_unresolved_property_t
impl_13_property_data = {
    "data",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_14
    }
};

static const afw_runtime_unresolved_property_t
impl_13_property_isDirectory = {
    "isDirectory",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_15
    }
};

static const afw_runtime_unresolved_property_t
impl_13_property_timeAccessed = {
    "timeAccessed",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_16
    }
};

static const afw_runtime_unresolved_property_t
impl_13_property_timeCreated = {
    "timeCreated",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_17
    }
};

static const afw_runtime_unresolved_property_t
impl_13_property_timeModified = {
    "timeModified",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_18
    }
};

static const afw_runtime_unresolved_property_t
impl_13_property_vfsPath = {
    "vfsPath",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_19
    }
};

static const afw_runtime_unresolved_property_t *
impl_13_properties[] = {
    &impl_13_property_data,
    &impl_13_property_isDirectory,
    &impl_13_property_timeAccessed,
    &impl_13_property_timeCreated,
    &impl_13_property_timeModified,
    &impl_13_property_vfsPath,
    NULL
};

static const afw_runtime_unresolved_const_embedded_untyped_object_t
impl_13 = {
    &impl_13_properties[0]
};

/*
 * /afw/_AdaptiveObjectType_/_AdaptiveFile_vfs
 */

static const afw_runtime_unresolved_property_t
impl_12_property_allowEntity = {
    "allowEntity",
    {
        .type = afw_runtime_unresolved_primitive_type_boolean,
        .boolean = true
    }
};

static const afw_utf8_t
impl_12_array_collectionURIs[] = {
    AFW_UTF8_LITERAL("/afw/_AdaptiveCollection_/vfs_extension")
};

static const afw_runtime_unresolved_const_array_t
impl_12_list_collectionURIs = {
    AFW_UTF8_LITERAL("string"),
    sizeof(impl_12_array_collectionURIs) / sizeof(afw_utf8_t),
    (const void *)&impl_12_array_collectionURIs
};

static const afw_runtime_unresolved_property_t
impl_12_property_collectionURIs = {
    "collectionURIs",
    {
        .type = afw_runtime_unresolved_primitive_type_array,
        .array = &impl_12_list_collectionURIs
    }
};

static const afw_runtime_unresolved_property_t
impl_12_property_description = {
    "description",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "This is the single object type of objects in a vfs adaptor."
    }
};

static const afw_runtime_unresolved_property_t
impl_12_property_objectType = {
    "objectType",
    {
        .type = afw_runtime_unresolved_primitive_type_string,
        .string = "_AdaptiveFile_vfs"
    }
};

static const afw_runtime_unresolved_property_t
impl_12_property_propertyTypes = {
    "propertyTypes",
    {
        .type = afw_runtime_unresolved_primitive_type_object,
        .object = &impl_13
    }
};

static const afw_runtime_unresolved_property_t *
impl_12_properties[] = {
    &impl_12_property_allowEntity,
    &impl_12_property_collectionURIs,
    &impl_12_property_description,
    &impl_12_property_objectType,
    &impl_12_property_propertyTypes,
    NULL
};

static const afw_utf8_t
impl_12_meta_path =
    AFW_UTF8_LITERAL("/afw/_AdaptiveObjectType_/_AdaptiveFile_vfs");

static const afw_runtime_unresolved_const_object_t
impl_12 = {
    &afw_vfs_self_s__AdaptiveObjectType_,
    &afw_vfs_self_s__AdaptiveFile_vfs,
    &impl_12_meta_path,
    &impl_12_properties[0]
};

/* NULL terminated list of unresolved objects. */
static const afw_runtime_unresolved_const_object_t *
impl_const_objects[] = {
    &impl_1,
    &impl_8,
    &impl_12,
    NULL
};

/* Resolved and set const objects */
void
afw_vfs_const_objects_register(afw_xctx_t *xctx)
{
    afw_runtime_env_resolve_and_set_const_objects(
        &impl_const_objects[0], false, xctx);
}
