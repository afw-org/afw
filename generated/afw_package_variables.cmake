# See the 'COPYING' file in the project root for licensing information.
#
# afw-package.json property variables
#
#----------------------------- N O T E -------------------------------------
#
# This file is generated by "afwdev generate".
#
# Do not change this file directly or the changes will be lost the next time
# this file is regenerated.
#
#----------------------------------------------------------------------------
#
# This sets a variable for each property in afw-package.json file. The variable
# names are "afw_package_", followed by any parent property names with an
# underscore ('_') appended, followed by the property name. For objects, the
# value is a list of its property names.
#
# This also sets additional variables AFW_PACKAGE_ID, AFW_PACKAGE_PROJECT_NAME,
# and AFW_PACKAGE_SOURCE_DIR, plus a variable prefixed with "AFW_PACKAGE_"
# instead of "CMAKE_" for each property set by GNUInstallDirs with some
# customizations.
#
# AFW_PACKAGE_INSTALL_CMAKEDIR and AFW_PACKAGE_INSTALL_FULL_CMAKEDIR are also
# set with the path where cmake related files for this particular afw package
# are installed.
#

set(afw_package 
    "afwPackageId"
    "brief"
    "bugReportEmail"
    "compileOptions"
    "copyright"
    "description"
    "homePageUrl"
    "installPackageSubdir"
    "srcdirManifest"
    "srcdirs"
    "version"
    "afwPackageLabel"
)
set(afw_package_afwPackageId "afw")
set(afw_package_brief "The base AFW repository including core")
set(afw_package_bugReportEmail "info@adaptiveframework.org")
set(afw_package_compileOptions 
    "GNU"
)
set(afw_package_compileOptions_GNU -std=c11 -D_DEFAULT_SOURCE -g -Wall -Wextra -Wno-missing-field-initializers -Wno-unused-parameter -Wno-sign-compare -Wno-implicit-fallthrough -Werror -fsigned-char)
set(afw_package_copyright "Copyright (c) 2010-2024 Clemson University")
set(afw_package_description "This is the Adaptive Framework (AFW) base which includes library libafw, the afw and afwdev commands, the core UI support, the AFW Administration application, plus a base set of extensions and server implementations.")
set(afw_package_homePageUrl "https://github.com/afw-org")
set(afw_package_installPackageSubdir "afw")
set(afw_package_srcdirManifest "afw")
set(afw_package_srcdirs 
    "afw"
    "afw_admin"
    "afw_bindings"
    "afw_client"
    "afw_command"
    "afw_components"
    "afw_components_react_core"
    "afw_components_react_material"
    "afw_components_react_monaco"
    "afw_dev"
    "afw_ldap"
    "afw_lmdb"
    "afw_lmdb_bindings"
    "afw_server_fcgi"
    "afw_test"
    "afw_ubjson"
    "afw_vfs"
    "afw_yaml"
)
set(afw_package_srcdirs_afw 
    "brief"
    "buildType"
    "description"
    "prefix"
    "produces"
    "srcdirPath"
    "version"
)
set(afw_package_srcdirs_afw_brief "Library libafw")
set(afw_package_srcdirs_afw_buildType "afwmake")
set(afw_package_srcdirs_afw_description "AFW shared object - libafw.")
set(afw_package_srcdirs_afw_prefix "afw_")
set(afw_package_srcdirs_afw_produces "libafw")
set(afw_package_srcdirs_afw_srcdirPath "src/afw/")
set(afw_package_srcdirs_afw_version "0.9.0")
set(afw_package_srcdirs_afw_admin 
    "brief"
    "buildType"
    "description"
    "srcdirPath"
    "prefix"
    "version"
)
set(afw_package_srcdirs_afw_admin_brief "Administration web application")
set(afw_package_srcdirs_afw_admin_buildType "js-app")
set(afw_package_srcdirs_afw_admin_description "React-based web application for administering AFW.")
set(afw_package_srcdirs_afw_admin_srcdirPath "src/afw_app/admin/")
set(afw_package_srcdirs_afw_admin_prefix "afw_admin_")
set(afw_package_srcdirs_afw_admin_version "0.9.0")
set(afw_package_srcdirs_afw_bindings 
    "brief"
    "buildType"
    "description"
    "srcdirPath"
    "prefix"
    "version"
)
set(afw_package_srcdirs_afw_bindings_brief "Javascript core language bindings")
set(afw_package_srcdirs_afw_bindings_buildType "js-module")
set(afw_package_srcdirs_afw_bindings_description "Javascript bindings for afw core.")
set(afw_package_srcdirs_afw_bindings_srcdirPath "src/afw/generated/javascript_bindings/")
set(afw_package_srcdirs_afw_bindings_prefix "afw_bindings_")
set(afw_package_srcdirs_afw_bindings_version "0.9.0")
set(afw_package_srcdirs_afw_client 
    "brief"
    "buildType"
    "description"
    "srcdirPath"
    "prefix"
    "version"
)
set(afw_package_srcdirs_afw_client_brief "Javascript client library")
set(afw_package_srcdirs_afw_client_buildType "js-module")
set(afw_package_srcdirs_afw_client_description "Javascript functions that bindings use to communicate with afw.")
set(afw_package_srcdirs_afw_client_srcdirPath "src/afw_client/javascript/")
set(afw_package_srcdirs_afw_client_prefix "afw_client_")
set(afw_package_srcdirs_afw_client_version "0.9.0")
set(afw_package_srcdirs_afw_command 
    "brief"
    "buildType"
    "description"
    "prefix"
    "produces"
    "srcdirPath"
    "version"
)
set(afw_package_srcdirs_afw_command_brief "Command afw")
set(afw_package_srcdirs_afw_command_buildType "afwmake")
set(afw_package_srcdirs_afw_command_description "AFW command - afw.")
set(afw_package_srcdirs_afw_command_prefix "afw_command_")
set(afw_package_srcdirs_afw_command_produces "afw")
set(afw_package_srcdirs_afw_command_srcdirPath "src/afw_command/")
set(afw_package_srcdirs_afw_command_version "0.9.0")
set(afw_package_srcdirs_afw_components 
    "brief"
    "buildType"
    "description"
    "prefix"
    "produces"
    "srcdirPath"
    "version"
)
set(afw_package_srcdirs_afw_components_brief "Library libafwcomponents")
set(afw_package_srcdirs_afw_components_buildType "afwmake")
set(afw_package_srcdirs_afw_components_description "AFW shared object - libafwcomponents.")
set(afw_package_srcdirs_afw_components_prefix "afw_components_")
set(afw_package_srcdirs_afw_components_produces "libafwcomponents")
set(afw_package_srcdirs_afw_components_srcdirPath "src/afw_components/")
set(afw_package_srcdirs_afw_components_version "0.9.0")
set(afw_package_srcdirs_afw_components_react_core 
    "brief"
    "buildType"
    "description"
    "srcdirPath"
    "prefix"
    "version"
)
set(afw_package_srcdirs_afw_components_react_core_brief "Javascript react core components")
set(afw_package_srcdirs_afw_components_react_core_buildType "js-module")
set(afw_package_srcdirs_afw_components_react_core_description "Javascript core components implemented in react.")
set(afw_package_srcdirs_afw_components_react_core_srcdirPath "src/afw_components/react/core/")
set(afw_package_srcdirs_afw_components_react_core_prefix "afw_components_react_core_")
set(afw_package_srcdirs_afw_components_react_core_version "0.9.0")
set(afw_package_srcdirs_afw_components_react_material 
    "brief"
    "buildType"
    "description"
    "srcdirPath"
    "prefix"
    "version"
)
set(afw_package_srcdirs_afw_components_react_material_brief "Javascript react material components")
set(afw_package_srcdirs_afw_components_react_material_buildType "js-module")
set(afw_package_srcdirs_afw_components_react_material_description "Javascript components implemented in react material-ui.")
set(afw_package_srcdirs_afw_components_react_material_srcdirPath "src/afw_components/react/material/")
set(afw_package_srcdirs_afw_components_react_material_prefix "afw_components_react_material_")
set(afw_package_srcdirs_afw_components_react_material_version "0.9.0")
set(afw_package_srcdirs_afw_components_react_monaco 
    "brief"
    "buildType"
    "description"
    "srcdirPath"
    "prefix"
    "version"
)
set(afw_package_srcdirs_afw_components_react_monaco_brief "Javascript react monaco components")
set(afw_package_srcdirs_afw_components_react_monaco_buildType "js-module")
set(afw_package_srcdirs_afw_components_react_monaco_description "Javascript components implemented in react monaco.")
set(afw_package_srcdirs_afw_components_react_monaco_srcdirPath "src/afw_components/react/monaco/")
set(afw_package_srcdirs_afw_components_react_monaco_prefix "afw_components_react_monaco_")
set(afw_package_srcdirs_afw_components_react_monaco_version "0.9.0")
set(afw_package_srcdirs_afw_dev 
    "brief"
    "buildType"
    "buildTypeParameters"
    "description"
    "produces"
    "srcdirPath"
    "prefix"
    "version"
)
set(afw_package_srcdirs_afw_dev_brief "Command afwdev")
set(afw_package_srcdirs_afw_dev_buildType "afwmake:python")
set(afw_package_srcdirs_afw_dev_buildTypeParameters 
    "manualMakefile"
)
set(afw_package_srcdirs_afw_dev_buildTypeParameters_manualMakefile ON)
set(afw_package_srcdirs_afw_dev_description "AFW shared object - libafw.")
set(afw_package_srcdirs_afw_dev_produces "afwdev")
set(afw_package_srcdirs_afw_dev_srcdirPath "src/afw_dev/")
set(afw_package_srcdirs_afw_dev_prefix "afw_dev_")
set(afw_package_srcdirs_afw_dev_version "0.9.0")
set(afw_package_srcdirs_afw_ldap 
    "brief"
    "buildType"
    "description"
    "optionalChoiceDefault"
    "prefix"
    "produces"
    "srcdirPath"
    "version"
)
set(afw_package_srcdirs_afw_ldap_brief "Library libafwldap")
set(afw_package_srcdirs_afw_ldap_buildType "afwmake")
set(afw_package_srcdirs_afw_ldap_description "AFW shared object - libafwldap.")
set(afw_package_srcdirs_afw_ldap_optionalChoiceDefault ON)
set(afw_package_srcdirs_afw_ldap_prefix "afw_ldap_")
set(afw_package_srcdirs_afw_ldap_produces "libafwldap")
set(afw_package_srcdirs_afw_ldap_srcdirPath "src/afw_ldap/")
set(afw_package_srcdirs_afw_ldap_version "0.9.0")
set(afw_package_srcdirs_afw_lmdb 
    "brief"
    "buildType"
    "description"
    "optionalChoiceDefault"
    "prefix"
    "produces"
    "srcdirPath"
    "version"
)
set(afw_package_srcdirs_afw_lmdb_brief "Library libafwlmdb")
set(afw_package_srcdirs_afw_lmdb_buildType "afwmake")
set(afw_package_srcdirs_afw_lmdb_description "AFW shared object - libafwlmdb.")
set(afw_package_srcdirs_afw_lmdb_optionalChoiceDefault ON)
set(afw_package_srcdirs_afw_lmdb_prefix "afw_lmdb_")
set(afw_package_srcdirs_afw_lmdb_produces "libafwlmdb")
set(afw_package_srcdirs_afw_lmdb_srcdirPath "src/afw_lmdb/")
set(afw_package_srcdirs_afw_lmdb_version "0.9.0")
set(afw_package_srcdirs_afw_lmdb_bindings 
    "brief"
    "buildType"
    "description"
    "srcdirPath"
    "prefix"
    "version"
)
set(afw_package_srcdirs_afw_lmdb_bindings_brief "Javascript LMDB language bindings")
set(afw_package_srcdirs_afw_lmdb_bindings_buildType "js-module")
set(afw_package_srcdirs_afw_lmdb_bindings_description "Javascript bindings for LMDB extension.")
set(afw_package_srcdirs_afw_lmdb_bindings_srcdirPath "src/afw_lmdb/generated/javascript_bindings/")
set(afw_package_srcdirs_afw_lmdb_bindings_prefix "afw_lmdb_bindings_")
set(afw_package_srcdirs_afw_lmdb_bindings_version "0.9.0")
set(afw_package_srcdirs_afw_server_fcgi 
    "brief"
    "buildType"
    "description"
    "optionalChoiceDefault"
    "prefix"
    "produces"
    "srcdirPath"
    "version"
)
set(afw_package_srcdirs_afw_server_fcgi_brief "Command afwfcgi")
set(afw_package_srcdirs_afw_server_fcgi_buildType "afwmake")
set(afw_package_srcdirs_afw_server_fcgi_description "AFW command - afwfcgi.")
set(afw_package_srcdirs_afw_server_fcgi_optionalChoiceDefault ON)
set(afw_package_srcdirs_afw_server_fcgi_prefix "afw_server_fcgi_")
set(afw_package_srcdirs_afw_server_fcgi_produces "afwfcgi")
set(afw_package_srcdirs_afw_server_fcgi_srcdirPath "src/afw_server_fcgi/")
set(afw_package_srcdirs_afw_server_fcgi_version "0.9.0")
set(afw_package_srcdirs_afw_test 
    "brief"
    "buildType"
    "description"
    "srcdirPath"
    "prefix"
    "version"
)
set(afw_package_srcdirs_afw_test_brief "Javascript test library")
set(afw_package_srcdirs_afw_test_buildType "js-module")
set(afw_package_srcdirs_afw_test_description "Javascript functions that are useful for testing components.")
set(afw_package_srcdirs_afw_test_srcdirPath "src/afw_test/javascript/")
set(afw_package_srcdirs_afw_test_prefix "afw_test_")
set(afw_package_srcdirs_afw_test_version "0.9.0")
set(afw_package_srcdirs_afw_ubjson 
    "brief"
    "buildType"
    "description"
    "optionalChoiceDefault"
    "prefix"
    "produces"
    "srcdirPath"
    "version"
)
set(afw_package_srcdirs_afw_ubjson_brief "Library libafwubjson")
set(afw_package_srcdirs_afw_ubjson_buildType "afwmake")
set(afw_package_srcdirs_afw_ubjson_description "AFW shared object - libafwubjson.")
set(afw_package_srcdirs_afw_ubjson_optionalChoiceDefault ON)
set(afw_package_srcdirs_afw_ubjson_prefix "afw_ubjson_")
set(afw_package_srcdirs_afw_ubjson_produces "libafwubjson")
set(afw_package_srcdirs_afw_ubjson_srcdirPath "src/afw_ubjson/")
set(afw_package_srcdirs_afw_ubjson_version "0.9.0")
set(afw_package_srcdirs_afw_vfs 
    "brief"
    "buildType"
    "description"
    "optionalChoiceDefault"
    "prefix"
    "produces"
    "srcdirPath"
    "version"
)
set(afw_package_srcdirs_afw_vfs_brief "Library libafwvfs")
set(afw_package_srcdirs_afw_vfs_buildType "afwmake")
set(afw_package_srcdirs_afw_vfs_description "AFW shared object - libafwvfs.")
set(afw_package_srcdirs_afw_vfs_optionalChoiceDefault ON)
set(afw_package_srcdirs_afw_vfs_prefix "afw_vfs_")
set(afw_package_srcdirs_afw_vfs_produces "libafwvfs")
set(afw_package_srcdirs_afw_vfs_srcdirPath "src/afw_vfs/")
set(afw_package_srcdirs_afw_vfs_version "0.9.0")
set(afw_package_srcdirs_afw_yaml 
    "brief"
    "buildType"
    "description"
    "optionalChoiceDefault"
    "prefix"
    "produces"
    "srcdirPath"
    "version"
)
set(afw_package_srcdirs_afw_yaml_brief "Library libafwyaml")
set(afw_package_srcdirs_afw_yaml_buildType "afwmake")
set(afw_package_srcdirs_afw_yaml_description "AFW shared object - libafwyaml.")
set(afw_package_srcdirs_afw_yaml_optionalChoiceDefault ON)
set(afw_package_srcdirs_afw_yaml_prefix "afw_yaml_")
set(afw_package_srcdirs_afw_yaml_produces "libafwyaml")
set(afw_package_srcdirs_afw_yaml_srcdirPath "src/afw_yaml/")
set(afw_package_srcdirs_afw_yaml_version "0.9.0")
set(afw_package_version "0.9.0")
set(afw_package_afwPackageLabel "afw")


# Set package id.
set(AFW_PACKAGE_ID ${afw_package_afwPackageId})

# Set variable with afw package project name.
set(AFW_PACKAGE_PROJECT_NAME ${PROJECT_NAME})

# Set variable with afw package source dir.
set(AFW_PACKAGE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# ----------------------------------------------------------------------------
#
# The following ar sets for each 'CMAKE_INSTALL_' prefixed variable set by
# GNUInstallDirs with a prefix of 'AFW_PACKAGE_INSTALL_' instead along with
# customization for some variables.
# 
# The LIBDIR and INCLUDEDIR a slash and the installPackageSubdir property from
# afw-package.json appended and the PROJECT_NAME part of DOCDIR is the value of
# the afwPackageId property from afw-package.json.
#
# Check https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html for more
# information.
#
# Also, AFW_PACKAGE_INSTALL_CMAKEDIR and AFW_PACKAGE_INSTALL_FULL_CMAKEDIR are
# set with path of LIBDIR subdirectory cmake/${PROJECT_NAME}-${PROJECT_VERSION}
#
# These variables are used exclusively by afwdev generated cmake instead of
# their 'CMAKE_INSTALL_' counterparts.
#
# ----------------------------------------------------------------------------

include(GNUInstallDirs)

set(AFW_PACKAGE_INSTALL_BINDIR ${CMAKE_INSTALL_BINDIR})

set(AFW_PACKAGE_INSTALL_SBINDIR ${CMAKE_INSTALL_SBINDIR})

set(AFW_PACKAGE_INSTALL_LIBEXECDIR ${CMAKE_INSTALL_LIBEXECDIR})

set(AFW_PACKAGE_INSTALL_SYSCONFDIR ${CMAKE_INSTALL_SYSCONFDIR})

set(AFW_PACKAGE_INSTALL_SHAREDSTATEDIR ${CMAKE_INSTALL_SHAREDSTATEDIR})

set(AFW_PACKAGE_INSTALL_LOCALSTATE ${CMAKE_INSTALL_LOCALSTATE})

set(AFW_PACKAGE_INSTALL_RUNSTATEDIR ${CMAKE_INSTALL_RUNSTATEDIR})

if(DEFINED afw_package_installPackageSubdir)
    set(AFW_PACKAGE_INSTALL_LIBDIR
        ${CMAKE_INSTALL_LIBDIR}/${afw_package_installPackageSubdir})
    set(AFW_PACKAGE_INSTALL_INCLUDEDIR
        ${CMAKE_INSTALL_INCLUDEDIR}/${afw_package_installPackageSubdir})
else()
    set(AFW_PACKAGE_INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR})
    set(AFW_PACKAGE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR})
endif()

set(AFW_PACKAGE_INSTALL_OLDINCLUDEDIR ${CMAKE_INSTALL_OLDINCLUDEDIR})

set(AFW_PACKAGE_INSTALL_DATAROOTDIR ${CMAKE_INSTALL_DATAROOTDIR})

set(AFW_PACKAGE_INSTALL_DATADIR ${CMAKE_INSTALL_DATADIR})

set(AFW_PACKAGE_INSTALL_LOCALEDIR ${CMAKE_INSTALL_LOCALEDIR})

set(AFW_PACKAGE_INSTALL_MANDIR ${CMAKE_INSTALL_MANDIR})

set(AFW_PACKAGE_INSTALL_DOCDIR
    ${AFW_PACKAGE_INSTALL_DATAROOTDIR}/doc/${afw_package_afwPackageId})

set(AFW_PACKAGE_INSTALL_CMAKEDIR
    ${CMAKE_INSTALL_LIBDIR}/cmake/${afw_package_afwPackageId}-${afw_package_version}
)

# Repeat for CMAKE_INSTALL_FULL_<dir>

set(AFW_PACKAGE_INSTALL_FULL_BINDIR ${CMAKE_INSTALL_FULL_BINDIR})

set(AFW_PACKAGE_INSTALL_FULL_SBINDIR ${CMAKE_INSTALL_FULL_SBINDIR})

set(AFW_PACKAGE_INSTALL_FULL_LIBEXECDIR ${CMAKE_INSTALL_FULL_LIBEXECDIR})

set(AFW_PACKAGE_INSTALL_FULL_SYSCONFDIR ${CMAKE_INSTALL_FULL_SYSCONFDIR})

set(AFW_PACKAGE_INSTALL_FULL_SHAREDSTATEDIR ${CMAKE_INSTALL_FULL_SHAREDSTATEDIR})

set(AFW_PACKAGE_INSTALL_FULL_LOCALSTATE ${CMAKE_INSTALL_FULL_LOCALSTATE})

set(AFW_PACKAGE_INSTALL_FULL_RUNSTATEDIR ${CMAKE_INSTALL_FULL_RUNSTATEDIR})

if(DEFINED afw_package_installPackageSubdir)
    set(AFW_PACKAGE_INSTALL_FULL_LIBDIR
        ${CMAKE_INSTALL_FULL_LIBDIR}/${afw_package_installPackageSubdir})
    set(AFW_PACKAGE_INSTALL_FULL_INCLUDEDIR
        ${CMAKE_INSTALL_FULL_INCLUDEDIR}/${afw_package_installPackageSubdir})
else()
    set(AFW_PACKAGE_INSTALL_FULL_LIBDIR ${CMAKE_INSTALL_FULL_LIBDIR})
    set(AFW_PACKAGE_INSTALL_FULL_INCLUDEDIR ${CMAKE_INSTALL_FULL_INCLUDEDIR})
endif()

set(AFW_PACKAGE_INSTALL_FULL_OLDINCLUDEDIR ${CMAKE_INSTALL_FULL_OLDINCLUDEDIR})

set(AFW_PACKAGE_INSTALL_FULL_DATAROOTDIR ${CMAKE_INSTALL_FULL_DATAROOTDIR})

set(AFW_PACKAGE_INSTALL_FULL_DATADIR ${CMAKE_INSTALL_FULL_DATADIR})

set(AFW_PACKAGE_INSTALL_FULL_LOCALEDIR ${CMAKE_INSTALL_FULL_LOCALEDIR})

set(AFW_PACKAGE_INSTALL_FULL_MANDIR ${CMAKE_INSTALL_FULL_MANDIR})

set(AFW_PACKAGE_INSTALL_FULL_DOCDIR
    ${AFW_PACKAGE_INSTALL_FULL_DATAROOTDIR}/doc/${afw_package_afwPackageId})

set(AFW_PACKAGE_INSTALL_FULL_CMAKEDIR
    ${CMAKE_INSTALL_FULL_LIBDIR}/cmake/${afw_package_afwPackageId}-${afw_package_version}
)
