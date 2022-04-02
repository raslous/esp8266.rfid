#ifndef WEBSERVER
#define WEBSERVER

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <DNSServer.h>

const char* password = "mockumek";
const char* ssid = "mockumek";
const int port = 80;
DNSServer dnsServer;
AsyncWebServer server(port);

const char* serverName = "http://anetid-001-site1.ctempurl.com/api/access";
const char* apiAuth = "https://localhost:7085/api/auth?hex=";
// const char* apiAuth = "https://localhost:7085/api/auth?hex=C2%20D6%20F7%202C";

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
        server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
            request->send(200, "text/plain", "ready");
        });

        server.begin();
    }

    static String httpGetRequestAuth(const char* url, const char* query)
    {
        char buffer[64];
        strcpy(buffer, url);
        strcat(buffer, query);

        Serial.print("BUFFER: "); 
        Serial.println(buffer);

        return httpGETRequest(buffer);
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