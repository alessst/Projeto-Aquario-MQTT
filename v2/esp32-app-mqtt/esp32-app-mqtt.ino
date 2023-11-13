#include <WiFi.h>
#include <PubSubClient.h>
#include <ESPmDNS.h>
#include "time.h"

// WiFi Connection set
#define NET_SSID "Desktop_F5770510"
#define NET_PASSWORD "neno1793BIA"

// MQTT Connection set
#define MQTT_ID "alessandro_teste_mills"
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_MILLIS_TOPIC "teste_mills_tome"
#define MQTT_LED_TOPIC "teste_led_tome"
#define MQTT_LED_TOPIC_STATUS_R "teste_led_tome_status_red"
#define MQTT_LED_TOPIC_STATUS_G "teste_led_tome_status_green"
#define MQTT_LED_TOPIC_STATUS_Y "teste_led_tome_status_yellow"
#define MQTT_LDR "teste_ldr_tome"

#define LIGHT_SENSOR_PIN 36
#define LED_R 4
#define LED_G 15
#define LED_Y 2
#define BUTTON 23

bool red = 0;
bool green = 0;
bool yellow = 0;
bool estadoButton = 0;
bool estadoAnterior = 0;
int analogValue = 0;

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = -3600*3;

// Define net client
WiFiClient espClient;

// Define mqtt client
PubSubClient MQTT(espClient);
char millis_str[10] = "";
char ldr_str[5] = "";

//----------------------------------------------------------------------------------------------------------------//
int hora_r_on   = 19 ;
int min_r_on    = 31;
int hora_g_on   = 15;
int min_g_on    = 24;
int hora_y_on   = 15;
int min_y_on    = 26;
//***********************************************************************************************************//
int hora_r_off   = 19;
int min_r_off    = 33;
int hora_g_off   = 15;
int min_g_off    = 32;
int hora_y_off   = 15;
int min_y_off    = 34;
//--------------------------------------------------------------------------------------------------------------//

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Falha ao obter a hora");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}


void setupWIFI() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  } else {
    // Connect with WiFi

    Serial.println();
    Serial.print("Connecting to");
    Serial.println(NET_SSID);

    WiFi.begin(NET_SSID, NET_PASSWORD);

    // Loop para checar a conexão

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }

    // Display connection info

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP Address:");
    Serial.println(WiFi.localIP());
  }
}

void setupMQTT() {
  // Config MQTT Broker connection

  MQTT.setServer(MQTT_BROKER, MQTT_PORT);
  MQTT.setCallback(mqtt_ifrj_callback);

  // Conn exec

  while (!MQTT.connected()) {
    Serial.print("- MQTT Setup: Tentando se conectar ao Broker MQTT: ");
    Serial.println(MQTT_BROKER);

    if (MQTT.connect(MQTT_ID)) {
      Serial.println("- MQTT Setup: Conectado com sucesso");
      MQTT.subscribe(MQTT_LED_TOPIC);
    } else {
      Serial.println("- MQTT Setup: Falha ao se conectar, tentando novamente em 2s");
      delay(2000);
    }
  }
}

void setup(void) {
  // Set baudrate of serial com
  Serial.begin(115200);

  // Pinmode
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // Call setup wifi
  setupWIFI();
  // Call setup mqtt
  setupMQTT();
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

}

void loop(void) {
  acionar_hora();
  analogValue = analogRead(LIGHT_SENSOR_PIN);
  //sprintf(millis_str, "%d", millis());  // store printf of data (millis)
  sprintf(ldr_str, "%d", analogValue);  // store printf of data (millis)
  String rString = red ? "true" : "false";
  String gString = green ? "true" : "false";
  String yString = yellow ? "true" : "false";
  MQTT.publish(MQTT_LDR, ldr_str);
  //MQTT.publish(MQTT_MILLIS_TOPIC, millis_str);  // publish in topic
  MQTT.publish(MQTT_LED_TOPIC_STATUS_R, rString.c_str());
  MQTT.publish(MQTT_LED_TOPIC_STATUS_G, gString.c_str());
  MQTT.publish(MQTT_LED_TOPIC_STATUS_Y, yString.c_str());
  
  // Serial.println(ldr_str);
  setupWIFI();
  setupMQTT();
  MQTT.loop();

  
  //acender_led();
}

// Callback function
// Called when data is received in one of topics
void mqtt_ifrj_callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  Serial.print("- MQTT Callback Topic: ");
  Serial.println(topic);


  //obtem a string do payload recebido
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }
  //Controlando LED VERMELHO
  if (msg.equals("R1") ) {
    red = !red;
    digitalWrite(LED_R, red);
  }
  //Controlando LED VERDE
  if (msg.equals("V1")) {
    green = !green;
    digitalWrite(LED_G, green);
  }
  //Controlando LED AMARELO
  if (msg.equals("A1") ) {
    yellow = !yellow;
    digitalWrite(LED_Y, yellow);
  }
}

void acender_led() {
  estadoButton = digitalRead(BUTTON);

  if (estadoButton != estadoAnterior) {
    if (estadoButton == LOW) {
      red = !digitalRead(LED_R);
      digitalWrite(LED_R, red);
    }
    delay(50);  // Debounce - pequeno atraso para evitar flutuações rápidas do botão
  }

  estadoAnterior = estadoButton;
}
void acionar_hora(){

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Falha ao obter a hora");
    return;
  }
  if (timeinfo.tm_hour == hora_r_on && timeinfo.tm_min == min_r_on ) {
    digitalWrite(LED_R, HIGH);
    red = digitalRead(LED_R);
   }  
  if (timeinfo.tm_hour == hora_r_off && timeinfo.tm_min == min_r_off ) {
    digitalWrite(LED_R, LOW);
    red = digitalRead(LED_R);
   } 
   //------------------------------------------------------------------//
  if (timeinfo.tm_hour == hora_g_on  && timeinfo.tm_min == min_g_on  ) {
    digitalWrite(LED_G, HIGH);
    green = digitalRead(LED_G);
  }
  if (timeinfo.tm_hour == hora_g_off  && timeinfo.tm_min == min_g_off  ) {
    digitalWrite(LED_G, LOW);
    green = digitalRead(LED_G);
  }
  //--------------------------------------------------------------------//
  if (timeinfo.tm_hour == hora_y_on  && timeinfo.tm_min == min_y_on ) {
    digitalWrite(LED_Y, HIGH);
    yellow = digitalRead(LED_Y);
  }
  if (timeinfo.tm_hour == hora_y_off  && timeinfo.tm_min == min_y_off ) {
    digitalWrite(LED_Y, LOW);
    yellow = digitalRead(LED_Y);
  }
  delay(50);

  }

