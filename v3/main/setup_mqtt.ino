#include <WiFi.h>
#include <PubSubClient.h>

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


// Define net client
WiFiClient espClient;

// Define mqtt client
PubSubClient MQTT(espClient);


void setupMQTT() {
  // Config MQTT Broker connection

  MQTT.setServer(MQTT_BROKER, MQTT_PORT);
  MQTT.setCallback(mqtt_ifrj_callback);
 
  // Conn exec

  while (!MQTT.connected()) {
    Serial.print("- MQTT Setup: Tentando se conectar ao Broker MQTT: ");
    Serial.println(MQTT_BROKER);
    display.clearDisplay();
    display_print("Conectando MQTT...", MQTT_BROKER, 5, 20);
    display.display();

    if (MQTT.connect(MQTT_ID)) {
      Serial.println("- MQTT Setup: Conectado com sucesso");
      MQTT.subscribe(MQTT_LED_TOPIC);
      display.clearDisplay();
      display_print("Conectado MQTT ! ", "WIFI Conectado !", 5, 20);
      display.display();
    } else {
      Serial.println("- MQTT Setup: Falha ao se conectar, tentando novamente em 2s");
      char exemplo[10] = "ERROR";
      display.clearDisplay();
      display_print("Falha ao conectar no MQTT ! ", exemplo, 5, 20);
      display.display();
      delay(2000);
    }
  }
}

void setup_envio(){
  int dado = 0;
  valor_sensor();
  char ldr_str[5];
  sprintf(ldr_str, "%d", analogValue);
  String rString = red ? "true" : "false";
  String gString = green ? "true" : "false";
  String yString = yellow ? "true" : "false";
  MQTT.publish(MQTT_LDR, ldr_str);
  MQTT.publish(MQTT_LED_TOPIC_STATUS_R, rString.c_str());
  MQTT.publish(MQTT_LED_TOPIC_STATUS_G, gString.c_str());
  MQTT.publish(MQTT_LED_TOPIC_STATUS_Y, yString.c_str());
  MQTT.loop();

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



