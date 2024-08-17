// See the 'COPYING' file in the project root for licensing information.
/*
 * afw_curl_function_execute_* functions for smtp
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file afw_curl_function_smtp.c
 * @brief afw_curl_function_execute_* functions for smtp.
 */

#include "afw.h"



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
    /** @todo Add code. */
    AFW_THROW_ERROR_Z(general, "Not implemented", x->xctx);
}
