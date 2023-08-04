<?php

    class request {

        protected $session;
        protected $payload;

        public function __construct($session) {
            $this->session = $session;
            $this->payload = [];

            $this->payload["adaptorId"] = $this->session->get("adaptorId");
        }

        public function add_action($action) {
            if (!isset($this->payload["actions"]))
                $this->payload["actions"] = [];

            array_push($this->payload["actions"], $action);
        }

        public function set($name, $value) {
            $this->payload[$name] = $value;
        }

        public function perform() {
            $client = $this->session->get_client();
            $postResponse = $client->request("POST", "", [
                "headers"       => $this->session->get_headers(),
                "json"          => $this->payload
            ]);

            return $postResponse;
        }

        public function get_result() {
            $response = $this->perform();

            $status = $response->getStatusCode();
            if ($status != 200) {
                throw new Exception("Error in response.");
            }

            $result = json_decode($response->getBody(), true);

            return $result["result"];
        }
    }

?>
