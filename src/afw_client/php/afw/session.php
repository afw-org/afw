<?php
    require "request.php";

    require "./vendor/autoload.php";

    class session {

        protected $url;
        protected $params;
        protected $client;

        public function __construct($url = "/afw/", $params = []) {
            $this->url = $url;
            $this->params = $params;

            $this->params["headers"] = [
                "Content-Type"  => "application/json",
                "Accept" => "application/json"
            ];

            $this->client = new GuzzleHttp\Client(["base_uri" => $this->url]);
        }

        public function get_client() {
            return $this->client;
        }

        public function get_headers() {
            return $this->params["headers"];
        }

        public function set($name, $value) {
            $this->params[$name] = $value;
        }

        public function get($name) {
            if (isset($this->params[$name]))
                return $this->params[$name];
            else
                return null;
        }

        public function request() {
            return (new request($this));
        }
    }

?>
