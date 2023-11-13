#include <WiFi.h>

// WiFi Connection set
#define NET_SSID "VIVOFIBRA-2G"
#define NET_PASSWORD "19725349es"

// WiFi Connection set
//#define NET_SSID "Desktop_F5770510"
//#define NET_PASSWORD "neno1793BIA"

void setupWIFI() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  } else {
    // Connect with WiFi
    display.clearDisplay();
    display_print("Conectando-se...", NET_SSID, 5, 20);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(NET_SSID);
    display.display();
    WiFi.begin(NET_SSID, NET_PASSWORD);

    // Loop para checar a conexão

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    // Display connection info
    display.clearDisplay();
    display_print("WiFi conectado!", NET_SSID, 5, 20);
    display.display();
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP Address:");
    Serial.println(WiFi.localIP());
  }
}
