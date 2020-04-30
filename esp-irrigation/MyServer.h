/*
  Rest.h - Library for exposing important things via REST API.
*/
#ifndef MyRest_h
#define MyRest_h

const int WEB_SERVER_PORT = 80;

const String TEXT_PLAIN = "text/plain";
const String TEXT_HTML = "text/html";
const String TEXT_JSON = "text/json";

const size_t JSON_DOC_CAPACITY = JSON_OBJECT_SIZE(1);


void httpJsonResponseHeader(WiFiClient client, int code, String response) {
  client.println("HTTP/1.1 " + String(code) + " " + response);
  client.println("Content-Type: "+TEXT_JSON);
  client.println("");                             //  do not forget this one
}

#endif
