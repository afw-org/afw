// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_curl_function_execute_* functions for smtp
 *
 * Copyright (c) 2010-2023 Clemson University
 *
 */

/**
 * @file afw_curl_function_smtp.c
 * @brief afw_curl_function_execute_* functions for smtp.
 */

#include "afw.h"
#include "afw_curl_internal.h"


/*
 * Adaptive function: smtp_send
 *
 * afw_curl_function_execute_smtp_send
 *
 * See afw_curl_function_bindings.h for more information.
 *
 * Sends an eMail using SMTP protocol with cURL.
 *
 * This function is not pure, so it may return a different result
 * given exactly the same parameters.
 *
 * Declaration:
 *
 * ```
 *   function smtp_send(
 *       url: string,
 *       mail_from: string,
 *       mail_recipients: array,
 *       payload: string,
 *       options?: (object _AdaptiveCurlOptions_)
 *   ): void;
 * ```
 *
 * Parameters:
 *
 *   url - (string) SMTP Server Url.
 *
 *   mail_from - (string) From Address.
 *
 *   mail_recipients - (array) To Addresses.
 *
 *   payload - (string) Email Payload, or body of the message.
 *
 *   options - (optional object _AdaptiveCurlOptions_) CURL Options.
 *
 * Returns:
 *
 *   (void)
 */
const afw_value_t *
afw_curl_function_execute_smtp_send(
    afw_function_execute_t *x)
{
    const afw_value_string_t     * url;
    const afw_value_string_t     * mail_from;
    const afw_value_array_t       * mail_recipients;
    const afw_value_string_t     * payload;
    const afw_value_object_t     * options;

    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(url, 1, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(mail_from, 2, string);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(mail_recipients, 3, array);
    AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(payload, 4, string);

    /* Optional options */
    options = NULL;
    if (AFW_FUNCTION_PARAMETER_IS_PRESENT(5)) {
        AFW_FUNCTION_EVALUATE_REQUIRED_DATA_TYPE_PARAMETER(options, 5, object);
    }

    /* use internal routine to do the send */
    afw_curl_internal_smtp_send(
        &url->internal,
        &mail_from->internal,
        mail_recipients->internal,
        &payload->internal,
        (options ? options->internal : NULL),
        x->p, x->xctx);

    return afw_value_undefined;
}
