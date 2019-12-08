#include <WiFi.h>
#include <HTTPClient.h>

using namespace std;

const char* ssid = CONFIG_WIFI_SSID;
const char* password = CONFIG_WIFI_PASSWORD;
  
void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }  
  Serial.println(WiFi.localIP());
}
   
void loop() {
 if (WiFi.status()== WL_CONNECTED) {
   HTTPClient http;   

   http.begin("http://jsonplaceholder.typicode.com/posts");
   http.addHeader("Content-Type", "text/plain");

  int httpResponseCode = http.POST("POSTING from ESP32 (kKEo)"); 
  if (httpResponseCode>0) {
    String response = http.getString();  
    Serial.println(httpResponseCode);   
    Serial.println(response);
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
 
   http.end();
 } else {
    Serial.println("Error in WiFi connection");   
 }
 
  delay(10000);
 
}