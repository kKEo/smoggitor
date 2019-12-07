#include <WiFi.h>
#include <HTTPSServer.hpp>
#include <SSLCert.hpp>
#include <HTTPRequest.hpp>
#include <HTTPResponse.hpp>

using namespace httpsserver;

const char* ssid = CONFIG_WIFI_SSID;
const char* password = CONFIG_WIFI_PASSWORD;
  
SSLCert * cert;
HTTPSServer * secureServer;
   
void setup() {
  Serial.begin(9600);
  Serial.println("Creating certificate...");
     
  cert = new SSLCert();
  int createCertResult = createSelfSignedCert(
    *cert, KEYSIZE_2048, "CN=myesp.local,O=acme,C=US");
     
  if (createCertResult != 0) {
    Serial.printf("Error generating certificate");
    return; 
  }
  Serial.println("Certificate created with success");

  WiFi.begin(ssid, p4ss);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }  
    
  Serial.println(WiFi.localIP());
  
  secureServer = new HTTPSServer(cert);
   
  ResourceNode * notFoundRoute = new ResourceNode("/notfound", "GET", [](HTTPRequest * req, HTTPResponse * res){
    res->setStatusCode(404);
    res->println("Not found");
  });
 
  ResourceNode * createdRoute = new ResourceNode("/created", "GET", [](HTTPRequest * req, HTTPResponse * res){
    res->setStatusCode(201);
    res->println("Created");
  });
   
  secureServer->registerNode(notFoundRoute);
  secureServer->registerNode(createdRoute);
 
  secureServer->start();
     
  if (secureServer->isRunning()) {
    Serial.println("Server ready");
  } else {
    Serial.println("Server could not be started");
  }
}
   
void loop() {
  secureServer->loop();  
  delay(10);
}