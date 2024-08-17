<?php

// See the 'COPYING' file in the project root for licensing information.
/*
 * PHP Bindings for smtp
 *
 * Copyright (c) 2010-2024 Clemson University
 *
 */

/**
 * @file smtp.php
 * @ingroup afw_bindings_php
 * @brief Class definition for function category smtp
 */
class smtp
{
    private $session;

    /**
     * Constructs the smtp class, using a session.
     */
    public function __construct($session = null)
    {
        $this->session = $session;
    }

    /**
     * smtp_send()
     *
     * Sends an eMail using SMTP protocol with cURL.
     *
     * @param string $url SMTP Server Url.
     * @param string $mail_from From Address.
     * @param array $mail_recipients To Addresses.
     * @param string $payload Email Payload, or body of the message.
     * @param object $options CURL Options
     *
     * @return void
     */
    public function smtp_send(, $url, $mail_from, $mail_recipients, $payload, $options = null)
    {
        $request = $this->$session->request();

        $request->set("function", "smtp_send");

        /* pass along required parameters to the request payload */
        $request->set("url", $url);
        $request->set("mail_from", $mail_from);
        $request->set("mail_recipients", $mail_recipients);
        $request->set("payload", $payload);

        /* pass along any optional parameters to the request payload */
        if ($options != null)
            $request->set('options', $options);

        return $request->get_result();
    }

}

?>
