#include <WiFiNINA.h> 
void setup() { 
  Serial.begin(9600); 
  //Changed this if statement to setup instead of loop, since it is only needed to check once
  if (WiFi.status() == WL_NO_MODULE) { 
    Serial.println("WiFi module not found!"); 
  } 
} 

void loop() { 
    Serial.println("Scanning available networks..."); 
    uint8_t numNetworks = WiFi.scanNetworks(); 
    for (uint8_t i = 0; i < numNetworks; i++) {   //corrected the declaration to i = 0
    Serial.print("Network: "); 
    Serial.print(WiFi.SSID(i)); 
    Serial.print(" | Signal Strength: "); 
    Serial.print(WiFi.RSSI(i)); 
    Serial.print(" dBm | Encryption: "); 
    Serial.println(WiFi.encryptionType(i)); 
  } 
  delay(30000); //Replaced redudant delay statements and combined them into 1
}
