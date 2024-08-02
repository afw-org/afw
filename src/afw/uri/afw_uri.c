// See the 'COPYING' file in the project root for licensing information.
/*
 * Adaptive Framework Uniform Resource Identifier (URI) helpers
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_uri.c
 * @brief Adaptive Framework Uniform Resource Identifier (URI) helpers.
 */

#include "afw_internal.h"


AFW_DEFINE_CONST_DATA(afw_uri_octet_type_t)
afw_uri_octet_type[256] = {

    /* 00      */
    AFW_URI_OCTET_INVALID,

    /* 01      */
    AFW_URI_OCTET_INVALID,

    /* 02      */
    AFW_URI_OCTET_INVALID,

    /* 03      */
    AFW_URI_OCTET_INVALID,

    /* 04      */
    AFW_URI_OCTET_INVALID,

    /* 05      */
    AFW_URI_OCTET_INVALID,

    /* 06      */
    AFW_URI_OCTET_INVALID,

    /* 07      */
    AFW_URI_OCTET_INVALID,

    /* 08      */
    AFW_URI_OCTET_INVALID,

    /* 09      */
    AFW_URI_OCTET_INVALID,

    /* 0A      */
    AFW_URI_OCTET_INVALID,

    /* 0B      */
    AFW_URI_OCTET_INVALID,

    /* 0C      */
    AFW_URI_OCTET_INVALID,

    /* 0D      */
    AFW_URI_OCTET_INVALID,

    /* 0E      */
    AFW_URI_OCTET_INVALID,

    /* 0F      */
    AFW_URI_OCTET_INVALID,


    /* 10      */
    AFW_URI_OCTET_INVALID,

    /* 11      */
    AFW_URI_OCTET_INVALID,

    /* 12      */
    AFW_URI_OCTET_INVALID,

    /* 13      */
    AFW_URI_OCTET_INVALID,

    /* 14      */
    AFW_URI_OCTET_INVALID,

    /* 15      */
    AFW_URI_OCTET_INVALID,

    /* 16      */
    AFW_URI_OCTET_INVALID,

    /* 17      */
    AFW_URI_OCTET_INVALID,

    /* 18      */
    AFW_URI_OCTET_INVALID,

    /* 19      */
    AFW_URI_OCTET_INVALID,

    /* 1A      */
    AFW_URI_OCTET_INVALID,

    /* 1B      */
    AFW_URI_OCTET_INVALID,

    /* 1C      */
    AFW_URI_OCTET_INVALID,

    /* 1D      */
    AFW_URI_OCTET_INVALID,

    /* 1E      */
    AFW_URI_OCTET_INVALID,

    /* 1F      */
    AFW_URI_OCTET_INVALID,

    /* 20 SP   */
    AFW_URI_OCTET_INVALID,

    /* 21 !    */
    AFW_URI_OCTET_SUB_DELIM |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 22 "    */
    AFW_URI_OCTET_INVALID,

    /* 23 #    */
    AFW_URI_OCTET_GEN_DELIM |
    AFW_URI_OCTET_ENCODE_URI,

    /* 24 $    */
    AFW_URI_OCTET_SUB_DELIM |
    AFW_URI_OCTET_ENCODE_URI,

    /* 25 %    */
    AFW_URI_OCTET_PERCENT,

    /* 26 &    */
    AFW_URI_OCTET_SUB_DELIM |
    AFW_URI_OCTET_ENCODE_URI,

    /* 27 '    */
    AFW_URI_OCTET_SUB_DELIM |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 28 (    */
    AFW_URI_OCTET_SUB_DELIM |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT,

    /* 29 )    */
    AFW_URI_OCTET_SUB_DELIM |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT,

    /* 2A *   */
    AFW_URI_OCTET_SUB_DELIM |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 2B +    */
    AFW_URI_OCTET_SUB_DELIM |
    AFW_URI_OCTET_PLUS |
    AFW_URI_OCTET_ENCODE_URI,

    /* 2C ,    */
    AFW_URI_OCTET_SUB_DELIM |
    AFW_URI_OCTET_ENCODE_URI,

    /* 2D -    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_MINUS |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 2E .    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_PERIOD |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 2F /    */
    AFW_URI_OCTET_GEN_DELIM |
    AFW_URI_OCTET_SLASH |
    AFW_URI_OCTET_ENCODE_URI,

    /* 30 0    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_DIGIT |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 31 1    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_DIGIT |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 32 2    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_DIGIT |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 33 3    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_DIGIT |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 34 4    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_DIGIT |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 35 5    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_DIGIT |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 36 6    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_DIGIT |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 37 7    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_DIGIT |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 38 8    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_DIGIT |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 39 9    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_DIGIT |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 3A :    */
    AFW_URI_OCTET_GEN_DELIM |
    AFW_URI_OCTET_COLON |
    AFW_URI_OCTET_ENCODE_URI,

    /* 3B ;    */
    AFW_URI_OCTET_SUB_DELIM |
    AFW_URI_OCTET_ENCODE_URI,

    /* 3C <    */
    AFW_URI_OCTET_INVALID,

    /* 3D =    */
    AFW_URI_OCTET_SUB_DELIM,

    /* 3E >    */
    AFW_URI_OCTET_INVALID,

    /* 3F ?    */
    AFW_URI_OCTET_GEN_DELIM |
    AFW_URI_OCTET_QUESTION |
    AFW_URI_OCTET_ENCODE_URI,


    /* 40 @    */
    AFW_URI_OCTET_GEN_DELIM |
    AFW_URI_OCTET_AT |
    AFW_URI_OCTET_ENCODE_URI,

    /* 41 A    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 42 B    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 43 C    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 44 D    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 45 E    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 46 F    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 47 G    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 48 H    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 49 I    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 4A J    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 4B K    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 4C L    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 4D M    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 4E N    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 4F O    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,


    /* 50 P    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 51 Q    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 52 R    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 53 S    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 54 T    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 55 U    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 56 V    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 57 W    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 58 X    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 59 Y    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 5A Z    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_UPPER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 5B [    */
    AFW_URI_OCTET_GEN_DELIM,

    /* 5C \    */
    AFW_URI_OCTET_INVALID,

    /* 5D ]    */
    AFW_URI_OCTET_GEN_DELIM,

    /* 5E ^    */
    AFW_URI_OCTET_INVALID,

    /* 5F _    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,


    /* 60 `    */
    AFW_URI_OCTET_INVALID,

    /* 61 a    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 62 b    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 63 c    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 64 d    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 65 e    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 66 f    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_HEXDIGIT |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 67 g    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 68 h    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 69 i    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 6A j    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 6B k    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 6C l    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 6D m    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 6E n    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 6F o    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,


    /* 70 p    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 71 q    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 72 r    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 73 s    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 74 t    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 75 u    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 76 v    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 77 w    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 78 x    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 79 y    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 7A z    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ALPHA_LOWER |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 7B {    */
    AFW_URI_OCTET_INVALID,

    /* 7C |    */
    AFW_URI_OCTET_INVALID,

    /* 7D }    */
    AFW_URI_OCTET_INVALID,

    /* 7E ~    */
    AFW_URI_OCTET_UNRESERVED |
    AFW_URI_OCTET_ENCODE_URI |
    AFW_URI_OCTET_ENCODE_COMPONENT |
    AFW_URI_OCTET_ENCODE_COMPONENT_VALUE,

    /* 7F DEL  */
    AFW_URI_OCTET_INVALID

    /* 80 - FF will be 0 which is AFW_URI_OCTET_INVALID */

};




typedef enum {
    impl_state_error,
    impl_state_end,
    impl_state_initial,
    impl_state_hier_part_begin,
    impl_state_hier_part_slash1,
    impl_state_hier_part_slash2,
    impl_state_scheme_begin,
    impl_state_scheme,
    impl_state_authority_begin,
    impl_state_authority_determine,
    impl_state_authority_userinfo_begin,
    impl_state_authority_userinfo,
    impl_state_authority_host_begin,
    impl_state_authority_reg_name,
    impl_state_authority_host_v4,
    impl_state_authority_host_v6,
    impl_state_authority_optional_port,
    impl_state_authority_port,
    impl_state_path_begin,
    impl_state_path,
    impl_state_query_begin,
    impl_state_query,
    impl_segment_begin,
    impl_segment,
    impl_segment_parameter_begin,
    impl_segment_parameter_value_begin,
    impl_state_fragment_begin,
    impl_state_fragment
} impl_state;


/* The length needed to URI encode a string. */
AFW_DECLARE(afw_size_t)
afw_uri_encode_len(
    const afw_utf8_t *string,
    afw_uri_octet_type_t mask,
    afw_xctx_t *xctx)
{
    afw_size_t len;
    const afw_octet_t *c, *end;

    for (len = 0,
        c = (const afw_octet_t *)string->s,
        end = c + string->len;
        c < end;
        c++, len++)
    {
        if (!afw_uri_octet_test(*c, mask))
        {
            len += 2; 
        }
    }

    return len;
}


/* The length needed to URI encode a raw. */
AFW_DEFINE(afw_size_t)
afw_uri_encode_raw_len(
    const afw_memory_t *raw,
    afw_uri_octet_type_t mask,
    afw_xctx_t *xctx)
{
    afw_size_t len;
    const afw_octet_t *c, *end;

    for (len = 0,
        c = raw->ptr,
        end = c + raw->size;
        c < end;
        c++, len++)
    {
        if (!afw_uri_octet_test(*c, mask))
        {
            len += 2; 
        }
    }

    return len;
}


/* URI encode a string. */
AFW_DEFINE(const afw_utf8_t *)
afw_uri_encode(
    const afw_utf8_t *string,
    afw_uri_octet_type_t mask,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t len;
    afw_octet_t *s;
    const afw_octet_t *c, *end;
    afw_utf8_t *result;


    /* If string is 0 length, just return empty string. */
    if (!string || string->len == 0) {
        return afw_s_a_empty_string;
    }

    /* Get encoded length. */
    len = afw_uri_encode_len(string, mask, xctx);

    /* If len is the same as string, no encoding is required. */
    if (len == string->len) {
        return string;
    }

    /* Encode string and return it. */
    result = afw_pool_calloc_type(p, afw_utf8_t, xctx);
    s = afw_pool_malloc(p, len, xctx);
    result->s = (const afw_utf8_octet_t *)s;
    result->len = len;
    for (c = (const afw_octet_t *)string->s,
        end = c + string->len;
        c < end;
        c++)
    {
        if (afw_uri_octet_test(*c, mask))
        {
            *s++ = *c;
        } else {
            *s++ = '%';
            *s++ = afw_ascii_encode_hex_digit(*c / 16);
            *s++ = afw_ascii_encode_hex_digit(*c % 16);
        }
    }
    return result;

}



/* Create a URI encode a string. */
AFW_DEFINE(const afw_utf8_t *)
afw_uri_encode_create(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    afw_uri_octet_type_t mask,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_utf8_t *result;

    result = afw_utf8_create(s, len, p, xctx);
    return afw_uri_encode(result, mask, p, xctx);
}




/* URI encode a raw. */
AFW_DEFINE(const afw_utf8_t *)
afw_uri_encode_raw(
    const afw_memory_t *raw,
    afw_uri_octet_type_t mask,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t len;
    afw_octet_t *s;
    const afw_octet_t *c, *end;
    afw_utf8_t *result;

    result = afw_pool_calloc_type(p, afw_utf8_t, xctx);

    len = afw_uri_encode_raw_len(raw, mask, xctx);
    if (len > 0) {
        s = afw_pool_malloc(p, len, xctx);
        result->s = (const afw_utf8_octet_t *)s;
        result->len = len;
        for (c = raw->ptr,
            end = c + raw->size;
            c < end;
            c++)
        {
            if (afw_uri_octet_test(*c, mask))
            {
                *s++ = *c;
            }
            else {
                *s++ = '%';
                *s++ = afw_ascii_encode_hex_digit(*c / 16);
                *s++ = afw_ascii_encode_hex_digit(*c % 16);
            }
        }
    }

    return result;
}


/* URI encode a string using supplied afw_utf8_t. */
AFW_DEFINE(afw_size_t)
afw_uri_encode_to_preallocated(
    afw_utf8_octet_t *s,
    afw_size_t s_len,
    const afw_utf8_t *string,
    afw_uri_octet_type_t mask,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t len;
    const afw_octet_t *c, *end;

    len = afw_uri_encode_len(string, mask, xctx);
    if (len > s_len) return -1;
    if (len > 0) {        
        for (c = (const afw_octet_t *)string->s,
            end = c + string->len;
            c < end;
            c++)
        {
            if (afw_uri_octet_test(*c, mask))
            {
                *s++ = *c;
            }
            else {
                *s++ = '%';
                *s++ = afw_ascii_encode_hex_digit(*c / 16);
                *s++ = afw_ascii_encode_hex_digit(*c % 16);
            }
        }
    }

    return len;
}



/* URI encode a string using supplied afw_memory_t. */
AFW_DEFINE(afw_size_t)
afw_uri_encode_raw_to_preallocated(
    afw_utf8_octet_t *s,
    afw_size_t s_len,
    const afw_memory_t *raw,
    afw_uri_octet_type_t mask,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t len;
    const afw_utf8_octet_t *c, *end;

    len = afw_uri_encode_raw_len(raw, mask, xctx);
    if (len > s_len) return -1;
    if (len > 0) {        
        for (c = (const afw_utf8_octet_t *)raw->ptr, end = c + raw->size;
            c < end;
            c++)
        {
            if (afw_uri_octet_test(*c, mask))
            {
                *s++ = *c;
            }
            else {
                *s++ = '%';
                *s++ = afw_ascii_encode_hex_digit(*c / 16);
                *s++ = afw_ascii_encode_hex_digit(*c % 16);
            }
        }
    }

    return len;
}



/* % encoding should already be validate. */
AFW_DEFINE(const afw_utf8_t *)
afw_uri_decode(
    const afw_utf8_t *encoded,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_memory_t *decoded;

    decoded = afw_uri_decode_to_raw(encoded, p, xctx);

    if (!afw_utf8_is_nfc((const afw_utf8_octet_t *)
        decoded->ptr, decoded->size, p, xctx))
    {
        AFW_THROW_ERROR_Z(general,
            "Only URIs that can be decoded to NFC Unicode are supported",
            xctx);
    }

    return (const afw_utf8_t *)decoded;
}


/* Create a URI decoded string. */
AFW_DEFINE(const afw_utf8_t *)
afw_uri_decode_create(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_memory_t *decoded;

    decoded = afw_uri_decode_to_raw_create(s, len, p, xctx);

    if (!afw_utf8_is_nfc((const afw_utf8_octet_t *)
        decoded->ptr, decoded->size, p, xctx))
    {
        AFW_THROW_ERROR_Z(general,
            "Only URIs that can be decoded to NFC Unicode are supported",
            xctx);
    }

    return (const afw_utf8_t *)decoded;
}


/* Create a URI decoded a string. */
AFW_DEFINE(const afw_memory_t *)
afw_uri_decode_to_raw_create(
    const afw_utf8_octet_t *s,
    afw_size_t len,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_t *result;
    afw_utf8_octet_t *o;
    const afw_utf8_octet_t *c;
    afw_utf8_octet_t c1, c2;
    const afw_utf8_octet_t *end;
    afw_size_t percent_count;
    afw_size_t decoded_len;

    result = afw_pool_calloc_type(p, afw_memory_t, xctx);

    for (percent_count = 0, c = s, end = c + len;
        c < end; c++)
    {
        if (*c == '%') percent_count++;
    }

    if (percent_count == 0 || len == 0) {
        result->ptr = (const afw_octet_t *)s;
        result->size = len;
        return result;
    }

    result->size = decoded_len = len - 2 * percent_count;
    o = afw_pool_malloc(p, result->size, xctx);
    result->ptr = (const afw_octet_t *)o;

    for (c = s; decoded_len > 0; decoded_len--) {
        if (*c == '%') {
            c++;
            c1 = *c++;
            c2 = *c++;
            *o++ = afw_ascii_decode_hex_digit(c1) * 16 +
                afw_ascii_decode_hex_digit(c2);
        }
        else {
            *o++ = *c++;
        }
    }

    return result;
}



/* % encoding should already be validate. */
AFW_DEFINE(const afw_memory_t *)
afw_uri_decode_to_raw(
    const afw_utf8_t *encoded,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_memory_t *result;
    afw_utf8_octet_t *o;
    const afw_utf8_octet_t *c;
    const afw_utf8_octet_t *end;
    afw_utf8_octet_t c1, c2;
    afw_size_t len;
    afw_size_t percent_count;

    for (percent_count = 0, c = encoded->s, end = c + encoded->len;
        c < end; c++)
    {
        if (*c == '%') percent_count++;
    }

    if (percent_count == 0 || encoded->len == 0) {
        return (const afw_memory_t *)encoded;
    }

    result = afw_pool_malloc_type(p, afw_memory_t, xctx);
    result->size = len = encoded->len - 2 * percent_count;
    o = afw_pool_malloc(p, result->size, xctx);
    result->ptr = (const afw_octet_t *)o;

    for (c = encoded->s; len > 0; len--) {
        if (*c == '%') {
            c++;
            c1 = *c++;
            c2 = *c++;
            *o++ = afw_ascii_decode_hex_digit(c1) * 16 +
                afw_ascii_decode_hex_digit(c2);
        }
        else {
            *o++ = *c++;
        }
    }

    return result;
}



AFW_DEFINE(void)
afw_uri_parser_initialize(afw_uri_parser_t *parser,
    const afw_utf8_t *uri,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    memset(parser, 0, sizeof(afw_uri_parser_t));
    parser->uri = uri;
    parser->p = p;
    parser->c = (const afw_octet_t *)uri->s;
    parser->end = (const afw_octet_t *)uri->s + uri->len;
}



AFW_DEFINE(const afw_uri_parser_t *)
afw_uri_parser_create(const afw_utf8_t *uri,
    const afw_pool_t *p, afw_xctx_t *xctx)
{
    afw_uri_parser_t *parser;

    parser = afw_pool_malloc_type(p, afw_uri_parser_t, xctx);

    afw_uri_parser_initialize(parser, uri, p, xctx);

    return parser;
}



AFW_DEFINE(void)
afw_uri_parse_next_token(
    const afw_uri_parser_t *parser,
    afw_xctx_t *xctx)
{
    afw_uri_parser_t *self = (afw_uri_parser_t *)parser;
    afw_boolean_t is_encoded;
    const afw_utf8_t *decoded;
    afw_utf8_octet_t x1, x2;
    afw_uri_octet_type_t type;

    if (self->reuse) {
        self->reuse = false;
        return;
    }

    if (self->c == self->end) {
        memset(&self->token, 0, sizeof(afw_utf8_t));
        self->token.s = NULL;
        self->token.len = 0;
        self->is_reserved = true;
        return;
    }

    self->type = afw_uri_octet_type[*self->c];
    if (!self->type) {
        goto error;
    }

    self->token.s = (const afw_utf8_octet_t *)self->c;

    if (AFW_URI_OCTET_IS(self->type, RESERVED)) {
        self->token.len = 1;
        self->is_reserved = true;
        (self->c)++;
        return;
    }

    self->is_reserved = false;

    if (*self->c == '.' && self->consider_period_a_token) {
        self->token.len = 1;
        (self->c)++;
        return;
    }

    for (is_encoded = false; self->c != self->end; (self->c)++) {
        if (*self->c == '%') {
            if (self->c + 2 > self->end) {
                goto error;
            }
            is_encoded = true;
            x1 = afw_ascii_decode_hex_digit(*(self->c + 1));
            if (x1 < 0) {
                goto error;
            }
            x2 = afw_ascii_decode_hex_digit(*(self->c + 2));
            if (x2 < 0) {
                goto error;
            }
            self->c += 2;
        } else {
            if (*self->c == '.' && self->consider_period_a_token)
            {
                break;
            }
            type = afw_uri_octet_type[*self->c];
            if (AFW_URI_OCTET_IS(type, RESERVED)) {
                break;
            }
        }
    }
    self->token.len = self->c - (const afw_octet_t *)self->token.s;
    if (is_encoded) {
        decoded = afw_uri_decode(&self->token, self->p, xctx);
        self->token.s = decoded->s;
        self->token.len = decoded->len;
    }

    return;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Error parsing URI " AFW_UTF8_FMT_Q " at offset %d",
        AFW_UTF8_FMT_ARG(self->uri),
        (int)(self->c - (const afw_octet_t *)self->uri->s));
}



AFW_DEFINE(void)
afw_uri_parse_reuse_token(
    const afw_uri_parser_t *parser,
    afw_xctx_t *xctx)
{
    afw_uri_parser_t *self = (afw_uri_parser_t *)parser;

    if (self->reuse) {
        AFW_THROW_ERROR_Z(general, "Token already being reused", xctx);
    }

    self->reuse = true;  
}



static afw_size_t
impl_component_len(const afw_utf8_t *component)
{
    afw_size_t len;
    const afw_octet_t *s, *end;
    afw_octet_t o, x1, x2;
    afw_uri_octet_type_t type;

    for (len = 0, s = (const afw_octet_t *)component->s,
        end = s + component->len;
        s < end; s++)
    {
        if (*s == '%') {
            if (s + 2 > end) goto error;
            x1 = afw_ascii_decode_hex_digit(*(s + 1));
            if (x1 == 255) goto error;
            x2 = afw_ascii_decode_hex_digit(*(s + 2));
            if (x2 == 255) goto error;
            s += 2;
            o = x1 * 16 + x2;
            type = afw_uri_octet_type[o];
            if (AFW_URI_OCTET_IS(type, ENCODE_URI)) {
                len += 1;
            }
            else {
                len += 3;
            }
        }
        else {
            type = afw_uri_octet_type[*s];
            if (AFW_URI_OCTET_IS(type, INVALID)) {
                len += 3;
            }
            else {
                len += 1;
            }
        }
    }

    return len;

error:
    return -1;
}


/* Assumes impl_component_len() was called to validate. */
static afw_size_t
impl_component_encode(afw_utf8_octet_t *s, const afw_utf8_t *component)
{
    afw_size_t len;
    const afw_octet_t *c, *end;
    afw_octet_t o, x1, x2;
    afw_uri_octet_type_t type;

    for (len = 0,
        c = (const afw_octet_t *)component->s,
        end = c + component->len;
        c < end;
        c++)
    {
        if (*c == '%') {
            x1 = afw_ascii_decode_hex_digit(*(c + 1));
            x2 = afw_ascii_decode_hex_digit(*(c + 2));
            c += 2;
            o = x1 * 16 + x2;
            type = afw_uri_octet_type[o];
            if (AFW_URI_OCTET_IS(type, ENCODE_URI)) {
                *s++ = o;
                len++;
            }
            else {
                *s++ = '%';
                *s++ = afw_ascii_encode_hex_digit(o / 16);
                *s++ = afw_ascii_encode_hex_digit(o % 16);
                len += 3;
            }
        }
        else {
            type = afw_uri_octet_type[*c];
            if (AFW_URI_OCTET_IS(type, INVALID)) {
                *s++ = '%';
                *s++ = afw_ascii_encode_hex_digit(*c / 16);
                *s++ = afw_ascii_encode_hex_digit(*c % 16);
                len += 3;
            }
            else {
                *s++ = *c;
                len++;
            }
        }
    }

    return len;
}



static void
impl_set_normalized_uri(
    afw_uri_parsed_t *parsed,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_size_t len;
    afw_utf8_octet_t *s;

    if (parsed->path_parsed) {
        parsed->normalized_uri.s = parsed->path_parsed->normalized_path.s;
        parsed->normalized_uri.len = parsed->path_parsed->normalized_path.len;
        return;
    }

    /* Calculate length. */
    len = 0;
    if (parsed->scheme.len != 0) {
        len += parsed->scheme.len + 1 /* : */;
    }

    if (parsed->userinfo || parsed->host || parsed->port.len != 0) {
        len += 2; /* // */
        if (parsed->userinfo) {
            len += parsed->userinfo->len + 1 /* @ */;
        }
        if (parsed->host) {
            len += parsed->host->len;
        }
        if (parsed->port.len != 0) {
            len += 1 /* : */
    + parsed->port.len;
        }
    }

    if (parsed->original_path.len != 0) {
        len += impl_component_len(&parsed->original_path);
    }

    if (parsed->original_query.len != 0) {
        len += impl_component_len(&parsed->original_query) + 1 /* ? */;
    }

    if (parsed->original_fragment.len != 0) {
        len += impl_component_len(&parsed->original_fragment) + 1 /* ? */;
    }

    /* Produce normalized URI. */
    parsed->normalized_uri.s = s = afw_pool_malloc(p, len, xctx);
    parsed->normalized_uri.len = len;

    if (parsed->scheme.len != 0) {
        memcpy(s, parsed->scheme.s, parsed->scheme.len);
        s += parsed->scheme.len;
        *s++ = ':';
    }

    if (parsed->userinfo || parsed->host || parsed->port.len != 0) {
        *s++ = '/';
        *s++ = '/';
        if (parsed->userinfo) {
            memcpy(s, parsed->userinfo->s, parsed->userinfo->len);
            s += parsed->userinfo->len;
            *s++ = '@';
        }
        if (parsed->host) {
            memcpy(s, parsed->host->s, parsed->host->len);
            s += parsed->host->len;
        }
        if (parsed->port.len != 0) {
            *s++ = ':';
            memcpy(s, parsed->port.s, parsed->port.len);
            s += parsed->port.len;
        }
    }

    if (parsed->original_path.len != 0) {
        s += impl_component_encode(s, &parsed->original_path);
    }
    if (parsed->original_query.len != 0) {
        *s++ = '?';
        s += impl_component_encode(s, &parsed->original_query);
    }
    if (parsed->original_fragment.len != 0) {
        *s++ = '#';
        impl_component_encode(s, &parsed->original_fragment);
    }
}


/* Parse an URI in specific pool. */
AFW_DEFINE(const afw_uri_parsed_t *)
afw_uri_parse(
    const afw_utf8_t *uri,
    afw_boolean_t is_value_path,
    const afw_utf8_t *current_path,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    afw_uri_octet_type_t type;
    impl_state state;
    const afw_octet_t *c;
    const afw_octet_t *end;
    afw_uri_parsed_t *parsed;
    afw_utf8_octet_t x1, x2;
    int increment;

    if (!uri || uri->len == 0) {
        AFW_THROW_ERROR_Z(general,
            "afw_uri_parse() requires a uri.",
        xctx);
    }

    parsed = afw_pool_calloc_type(p, afw_uri_parsed_t, xctx);
    parsed->original_uri = afw_utf8_clone(uri, p, xctx);
    if (current_path) {
        parsed->current_path = afw_utf8_clone(current_path, p, xctx);
    }

    for (
        state = impl_state_initial,
        end = (const afw_octet_t *)uri->s + uri->len,
        c = (const afw_octet_t *)uri->s,
        increment = 0;

        state != impl_state_end;
        
        )
    {

        /* If at end, type is -1.  Code should check for end before checking type. */
        if (c == end) {
            type = -1;
            /* increment remains the same. */
        }

        /*
         * If char is '%', percent decode and advance cursor to last octet.
         * For example, %ab, *c will be 'b'.  The code below can still use *c
         * to check for special characters as long as it is not HEXDIGIT.  The
         * type is set to AFW_URI_OCTET_PCT_ENCODED.
         */
        else if (*c == '%') {
            if (c + 3 > end) goto error;
            x1 = afw_ascii_decode_hex_digit(*(c + 1));
            if (x1 < 0) goto error;
            x2 = afw_ascii_decode_hex_digit(*(c + 2));
            if (x2 < 0) goto error;
            type = AFW_URI_OCTET_PCT_ENCODED;
            increment = 3;
        }

        /* If not "%', lookup type. */
        else {
            type = afw_uri_octet_type[*c];
            if (!type) goto error;
            increment = 1;
        }

        /* Process based on state. */
        switch (state) {

        /* Initial state. */
        case impl_state_initial:

            /* If at end, reset c to start of uri and parse hier_part. */
            if (c == end) {
                c = (const afw_octet_t *)uri->s;
                state = impl_state_hier_part_begin;
                break;
            }

            /*
             * If ':', everything so far is scheme, so reset c to start of uri
             * and parse scheme.
             */
            if (*c == ':') {
                c = (const afw_octet_t *)uri->s;
                state = impl_state_scheme_begin;
                break;
            }

            /* If not a scheme continue character, reset c and parse hier part. */
            if (!AFW_URI_OCTET_IS(type, SCHEME_CONTINUE)) {
                c = (const afw_octet_t *)uri->s;
                state = impl_state_hier_part_begin;
                break;
            }

            c += increment;
            break;


        case impl_state_scheme_begin:

            if (!AFW_URI_OCTET_IS(type, SCHEME_START)) goto error;

            parsed->scheme.s = (const afw_utf8_octet_t *)c;
            c += increment;
            state = impl_state_scheme;
            break;


        case impl_state_scheme:
            
            if (*c == ':') {
                parsed->scheme.len = c - (const afw_octet_t *)parsed->scheme.s;
                c++;
                parsed->original_hier_part.s = (const afw_utf8_octet_t *)c;
                state = impl_state_hier_part_begin;
                break;
            }

            if (!AFW_URI_OCTET_IS(type, SCHEME_CONTINUE)) goto error;
            c += increment;
            break;


        case impl_state_hier_part_begin:

            if (c == end) {
                state = impl_state_end;
                break;
            }

            parsed->original_hier_part.s = (const afw_utf8_octet_t *)c;

            if (*c == '/') {
                c++;
                state = impl_state_hier_part_slash1;
                break;
            }

            state = impl_state_path_begin;
            break;


        case impl_state_hier_part_slash1:

            if (c == end) {
                c--;
                state = impl_state_path_begin;
                break;
            }

            if (*c == '/') {
                c++;
                state = impl_state_authority_begin;
                break;
            }

            c -= increment;
            state = impl_state_path_begin;
            break;


        case impl_state_authority_begin:

            if (c == end) goto error;

            parsed->original_authority.s = (const afw_utf8_octet_t *)c;
            state = impl_state_authority_determine;
            /* Fall thought to impl_state_authority_determine. */

        case impl_state_authority_determine:

            if (c == end || AFW_URI_OCTET_IS(type, GEN_DELIM)) {
                state = (c != end && *c == '@')
                    ? impl_state_authority_userinfo_begin
                    : impl_state_authority_host_begin;
                c = (const afw_octet_t *)parsed->original_authority.s;
                break;
            };

            c += increment;
            break;


        case impl_state_authority_userinfo_begin:

            parsed->original_userinfo.s = (const afw_utf8_octet_t *)c;
            state = impl_state_authority_userinfo;
            /* Fall though to impl_state_authority_userinfo. */

        case impl_state_authority_userinfo:

            if (*c == '@') {
                parsed->original_userinfo.len =
                    c - (const afw_octet_t *)parsed->original_userinfo.s;
                parsed->userinfo =
                    afw_uri_decode(&parsed->original_userinfo, p, xctx);
                c++;
                state = impl_state_authority_host_begin;
                break;
            }

            if (!AFW_URI_OCTET_IS(type, USERINFO)) goto error;
            c += increment;
            break;


        case impl_state_authority_host_begin:

            if (c == end) {
                state = impl_state_end;
                break;
            }

            parsed->original_host.s = (const afw_utf8_octet_t *)c;

            if (*c == '[') {
                c += increment;
                state = impl_state_authority_host_v6;
                break;
            }

            if (AFW_URI_OCTET_IS(type, DIGIT)) {
                c++;
                state = impl_state_authority_host_v4;
                break;
            }

            state = impl_state_authority_reg_name;
            /* Fall though to impl_state_authority_reg_name */

        case impl_state_authority_reg_name:

            if (c == end || *c == '/' || *c == ':') {
                parsed->original_host.len =
                    c - (const afw_octet_t *)parsed->original_host.s;
                parsed->host =
                    afw_uri_decode(&parsed->original_host, p, xctx);
                state = (c == end)
                    ? impl_state_end
                    : impl_state_authority_optional_port;
                break;
            }

            c += increment;
            break;


        case impl_state_authority_host_v4:

            if (c == end || *c == '/' || *c == ':') {
                parsed->original_host.len =
                    c - (const afw_octet_t *)parsed->original_host.s;
                parsed->host = &parsed->original_host; /* v4 not encoded. */
                state = impl_state_authority_optional_port;
                break;
            }

            /** @fixme Should syntax check v4 address. */
            c += increment;
            break;


        case impl_state_authority_host_v6:

            if (c == end) goto error;

            if (*c == ']') {
                parsed->original_host.len =
                    c - (const afw_octet_t *)parsed->original_host.s + 1;
                parsed->host = &parsed->original_host; /* v6 not encoded. */
                c += increment;
                state = impl_state_authority_optional_port;
                break;
            }

            /** @fixme Should syntax check v6 address. */
            c += increment;
            break;


        case impl_state_authority_optional_port:

            if (c == end) {
                state = impl_state_end;
                break;
            }

            if (*c == ':') {
                c += increment;
                parsed->port.s = (const afw_utf8_octet_t *)c;
                state = impl_state_authority_port;
                break;
            }

            state = impl_state_path_begin;
            break;


        case impl_state_authority_port:

            if (c == end || !AFW_URI_OCTET_IS(type, DIGIT)) {
                parsed->port.len =
                    c - (const afw_octet_t *)parsed->port.s;
                if (c != end && *c != '/') goto error;
                state = impl_state_path_begin;
                break;
            }

            c++;
            break;


        case impl_state_path_begin:

            if (c == end) {
                state = impl_state_end;
                break;
            }

            parsed->original_path.s = (const afw_utf8_octet_t *)c;
            state = impl_state_path;
            /* Fall though to impl_state_path. */

        case impl_state_path:

            if (c == end || *c == '?' || *c == '#') {
                parsed->original_path.len =
                    c - (const afw_octet_t *)parsed->original_path.s;
                if (c == end) {
                    state = impl_state_end;
                    break;
                }
                state = (*c == '?')
                    ? impl_state_query_begin
                    : impl_state_fragment_begin;
                c += increment;
                break;
            }

            c += increment;
            break;


        case impl_state_query_begin:

            parsed->original_query.s = (const afw_utf8_octet_t *)c;
            state = impl_state_query;
            /* Fall though to impl_state_query. */

        case impl_state_query:
            /** @fixme Needs further parsing. */
            if (c == end || *c == '#') {
                parsed->original_query.len =
                    c - (const afw_octet_t *)parsed->original_query.s;
                if (c == end) {
                    state = impl_state_end;
                    break;
                }
                state = impl_state_fragment_begin;
                c += increment;
                break;
            }

            if (!AFW_URI_OCTET_IS(type, QUERY)) {
                goto error;
            }
            c += increment;
            break;


        case impl_state_fragment_begin:
 
            parsed->original_fragment.s = (const afw_utf8_octet_t *)c;
            state = impl_state_fragment;
            /* Fall though to impl_state_fragment. */

        case impl_state_fragment:
            /** @fixme Needs further parsing. */
            if (c == end) {
                parsed->original_fragment.len =
                    c - (const afw_octet_t *)parsed->original_fragment.s;
                state = impl_state_end;
                break;
            }

            if (!AFW_URI_OCTET_IS(type, FRAGMENT)) goto error;
            c += increment;
            break;


        default:
            AFW_THROW_ERROR_Z(general, "Internal error", xctx);
        }

    }

    if (is_value_path &&
        parsed->scheme.len == 0 &&
        parsed->original_authority.len == 0 &&
        parsed->original_query.len == 0 &&
        parsed->original_fragment.len == 0 &&
        parsed->original_path.len != 0)
    {
        parsed->path_parsed = afw_object_path_parse(
            &parsed->original_path, parsed->current_path, NULL,
            p, xctx);
    }

    impl_set_normalized_uri(parsed, p, xctx);

    return parsed;

error:
    AFW_THROW_ERROR_FZ(general, xctx,
        "Error parsing URI " AFW_UTF8_FMT_Q " at offset %d",
        AFW_UTF8_FMT_ARG(uri), (int)(c - (const afw_octet_t *)uri->s));
}


/* Turn a parsed URI into an object representation. */
AFW_DEFINE(const afw_object_t *)
afw_uri_parsed_to_object(
    const afw_uri_parsed_t * parsed,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_object_t *result;
    afw_uri_parser_t parser;
    const afw_array_t *list;
    const afw_value_t *value;
    const afw_object_t *object;

    result = afw_object_create(p, xctx);

    if (parsed->original_uri) {
        afw_object_set_property_as_string(result,
            afw_s_originalURI, parsed->original_uri, xctx);
    }

    if (parsed->normalized_uri.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_normalizedURI, &parsed->normalized_uri, xctx);
    }

    if (parsed->scheme.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_scheme, &parsed->scheme, xctx);
    }

    if (parsed->original_hier_part.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_originalHierPart, &parsed->original_hier_part, xctx);
    }

    if (parsed->original_authority.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_originalAuthority, &parsed->original_authority, xctx);
    }

    if (parsed->authority) {
        afw_object_set_property_as_string(result,
            afw_s_authority, parsed->authority, xctx);
    }

    if (parsed->original_userinfo.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_originalUserinfo, &parsed->original_userinfo, xctx);
    }

    if (parsed->userinfo) {
        afw_object_set_property_as_string(result,
            afw_s_userinfo, parsed->userinfo, xctx);
    }

    if (parsed->original_host.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_originalHost, &parsed->original_host, xctx);
    }

    if (parsed->host) {
        afw_object_set_property_as_string(result,
            afw_s_host, parsed->host, xctx);
    }

    if (parsed->port.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_port, &parsed->port, xctx);
    }

    if (parsed->original_path.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_originalPath, &parsed->original_path, xctx);
    }

    if (parsed->original_path.len > 0) {
        afw_object_set_property_as_string(result, afw_s_path,
            afw_uri_decode(&parsed->original_path, p, xctx), xctx);
        afw_uri_parser_initialize(&parser, &parsed->original_path, p, xctx);
        list = afw_array_of_create(
            afw_data_type_string, p, xctx);
        for (;;) {
            afw_uri_parse_next_token(&parser, xctx);
            if (!parser.token.s) break;
            value = afw_value_create_unmanaged_string(&parser.token, p, xctx);
            afw_array_add_value(list, value, xctx);
        }
        afw_object_set_property_as_array(result,
            afw_s_pathTokens, list, xctx);
    }

    if (parsed->original_query.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_originalQuery, &parsed->original_query, xctx);
    }

    if (parsed->original_query.len > 0) {
        afw_object_set_property_as_string(result, afw_s_query,
            afw_uri_decode(&parsed->original_query, p, xctx), xctx);
        afw_uri_parser_initialize(&parser, &parsed->original_query, p, xctx);
        list = afw_array_of_create(
            afw_data_type_string, p, xctx);
        for (;;) {
            afw_uri_parse_next_token(&parser, xctx);
            if (!parser.token.s) break;
            value = afw_value_create_unmanaged_string(&parser.token, p, xctx);
            afw_array_add_value(list, value, xctx);
        }
        afw_object_set_property_as_array(result,
            afw_s_queryTokens, list, xctx);
    }

    if (parsed->original_fragment.len > 0) {
        afw_object_set_property_as_string(result,
            afw_s_originalFragment, &parsed->original_fragment, xctx);
    }

    if (parsed->original_fragment.len > 0) {
        afw_object_set_property_as_string(result, afw_s_fragment,
            afw_uri_decode(&parsed->original_fragment, p, xctx), xctx);
        afw_uri_parser_initialize(&parser, &parsed->original_fragment, p, xctx);
        list = afw_array_of_create(
            afw_data_type_string, p, xctx);
        for (;;) {
            afw_uri_parse_next_token(&parser, xctx);
            if (!parser.token.s) break;
            value = afw_value_create_unmanaged_string(&parser.token, p, xctx);
            afw_array_add_value(list, value, xctx);
        }
        afw_object_set_property_as_array(result,
            afw_s_fragmentTokens, list, xctx);
    }

    if (parsed->path_parsed) {
        object = afw_object_path_parsed_to_object(
            parsed->path_parsed, p, xctx);
        afw_object_set_property_as_object(result,
            afw_s_valuePath, object, xctx);
    }

    return result;
}



/* Turn a URI into an object representation. */
AFW_DEFINE(const afw_object_t *)
afw_uri_to_object(
    const afw_utf8_t *uri,
    afw_boolean_t is_value_path,
    const afw_utf8_t *current_path,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const  afw_uri_parsed_t *parsed;

    parsed = afw_uri_parse(uri, is_value_path, current_path, p, xctx);

    return afw_uri_parsed_to_object(parsed, p, xctx);
}



/* Set consider_period_a_token flag. */
AFW_DEFINE(void)
afw_uri_parse_set_consider_period_a_token(
    const afw_uri_parser_t *parser,
    afw_boolean_t consider_period_a_token,
    afw_xctx_t *xctx)
{
    afw_uri_parser_t *self = (afw_uri_parser_t *)parser;

    self->consider_period_a_token = consider_period_a_token;
}


/* Determine if two parsed URIs are equivalent. */
AFW_DEFINE(afw_boolean_t)
afw_uri_parsed_are_equivalent(
    const afw_uri_parsed_t *parsed1,
    const afw_uri_parsed_t *parsed2,
    afw_xctx_t *xctx)
{

    if (parsed1->scheme.len != parsed2->scheme.len ||
        (parsed1->scheme.len != 0 &&
        !afw_utf8_equal(&parsed1->scheme, &parsed2->scheme)))
    {
        goto not_equal;
    }

    if (parsed1->userinfo) {
        if (!parsed2->userinfo ||
            !afw_utf8_equal(parsed1->userinfo, parsed2->userinfo))
        {
            goto not_equal;
        }
    }
    else {
        if (parsed2->userinfo) goto not_equal;
    }

    if (parsed1->host) {
        if (!parsed2->host ||
            !afw_utf8_equal(parsed1->host, parsed2->host))
        {
            goto not_equal;
        }
    }
    else {
        if (parsed2->host) goto not_equal;
    }

    if (parsed1->port.len != parsed2->port.len ||
        (parsed1->port.len != 0 &&
        !afw_utf8_equal(&parsed1->port, &parsed2->port)))
    {
        goto not_equal;
    }


    if (parsed1->path_parsed) {
        if (!parsed2->path_parsed ||
            !afw_object_path_parsed_are_equivalent(
                parsed1->path_parsed, parsed2->path_parsed, xctx))
        {
            goto not_equal;
        }
    }
    else {
        if (parsed2->path_parsed) goto not_equal;
        /** @fixme compare decoded tokens instead. */
        if (parsed1->original_path.len != parsed2->original_path.len ||
            (parsed1->original_path.len != 0 &&
            !afw_utf8_equal(&parsed1->original_path, &parsed2->original_path)))
        {
            goto not_equal;
        }
    }

    /** @fixme compare query and fragment part. */

    return true;

not_equal:
    return false;
}


/* Determine if two URIs are equivalent. */
AFW_DEFINE(afw_boolean_t)
afw_uri_are_equivalent(
    const afw_utf8_t *uri1,
    const afw_utf8_t *uri2,
    afw_boolean_t is_value_path,
    const afw_utf8_t *current_path2,
    const afw_pool_t *p,
    afw_xctx_t *xctx)
{
    const afw_uri_parsed_t *parsed1;
    const afw_uri_parsed_t *parsed2;

    parsed1 = afw_uri_parse(uri1, is_value_path, NULL, p, xctx);
    parsed2 = afw_uri_parse(uri2, is_value_path, current_path2, p, xctx);
    return afw_uri_parsed_are_equivalent(parsed1, parsed2, xctx);
}


AFW_DEFINE(afw_boolean_t)
afw_uri_octet_test(
    afw_octet_t octet,
    afw_uri_octet_type_t mask)
{
    afw_uri_octet_type_t type;
    afw_boolean_t result;

    type = afw_uri_octet_type[octet];

    if (AFW_URI_OCTET_MASK_IS_BITWISE_NOT & mask) {
        result = (type & ~mask) == 0;
    }

    else {
        result = (type & mask) != 0;
    }

    return result;  
}
