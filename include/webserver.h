#ifndef WEBSERVER
#define WEBSERVER

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "ESP";
const char* password = "esp32cam";

const char* serverName = "http://anetid-001-site1.ctempurl.com/api/access";

class WebServer
{
    public:
    static void Setup()
    {
        WiFi.begin(ssid, password);
  	    while (WiFi.status() != WL_CONNECTED)
	    {
    	    delay(1000);
    	    Serial.println("Connecting to WiFi...");
  	    }

  	    Serial.println(WiFi.localIP());
    }

    static void Start()
    {

    }

    static String httpGETRequest(const char* serverName)
    {
        WiFiClient client;
        HTTPClient http;
        // Your IP address with path or Domain name with URL path
        http.begin(client, serverName);

        // Send HTTP POST request
        int httpResponseCode = http.GET();

        String payload = "{}";

        if(httpResponseCode > 0)
        {
            Serial.print("HTTP Response code: ");
            Serial.println(httpResponseCode);
            payload = http.getString();
        }
        else
        {
            Serial.print("Error code: ");
            Serial.println(httpResponseCode);
        }

        // Free resources
        http.end();

        return payload;
    }
};

#endif