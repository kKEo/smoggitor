#include <WiFi.h>
#include <HTTPServer.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

using namespace httpsserver;
using namespace std;

const char* ssid = CONFIG_WIFI_SSID;
const char* password = CONFIG_WIFI_PASSWORD;
  
HTTPServer * httpServer;
   
void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }  
    
  Serial.println(WiFi.localIP());
  
  httpServer = new HTTPServer();

  ResourceNode * notFoundRoute = new ResourceNode("/notfound", "GET", [](HTTPRequest * req, HTTPResponse * res){
    res->setStatusCode(404);
    res->println("Not found");
  });
 
  ResourceNode * createdRoute = new ResourceNode("/created", "GET", [](HTTPRequest * req, HTTPResponse * res){
    const string status = req->getParams()->getRequestParameter("status");
    res->setStatusCode(201);
    res->setHeader("Content-Type", "application/json");
    string response =  "{\"status\": \"" + status + "\"}";
    res->printStd(response);
  });
   
  httpServer->registerNode(notFoundRoute);
  httpServer->registerNode(createdRoute);
 
  httpServer->start();
     
  if (httpServer->isRunning()) {
    Serial.println("Server ready");
  } else {
    Serial.println("Server could not be started");
  }
}
   
void loop() {
  httpServer->loop();  
  delay(10);
}
