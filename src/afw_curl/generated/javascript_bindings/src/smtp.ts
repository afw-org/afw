// See the 'COPYING' file in the project root for licensing information.
/*
 * Javascript Bindings for smtp
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */


/**
 * @file smtp.ts
 * @ingroup afw_bindings_javascript
 * @brief This file contains the Javascript bindings for the smtp category.
 */

interface IAnyObject {
    [prop : string] : any;
}

/**
 * Sends an eMail using SMTP protocol with cURL.
 * 
 * @param {string} url - SMTP Server Url.
 * 
 * @param {string} mail_from - From Address.
 * 
 * @param {array} mail_recipients - To Addresses.
 * 
 * @param {string} payload - Email Payload, or body of the message.
 * 
 * @param {object} options - CURL Options
 * 
 * @returns {void}
 */
export function afwSmtpSend(client : any, url : string, mail_from : string, mail_recipients : any[], payload : string, options? : object) : any {

    let _action : IAnyObject = {};

    _action["function"] = "smtp_send";
    _action["url"] = url;
    _action["mail_from"] = mail_from;
    _action["mail_recipients"] = mail_recipients;
    _action["payload"] = payload;

    if (options !== undefined)
        _action["options"] = options;

    return client.perform(_action);
}

