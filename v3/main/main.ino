void setup() {
  Serial.begin(115200);

  setup_display();

  setupLEDs();
  setupWIFI();
  setupMQTT();
  config_hora();
  printLocalTime();
  setup_display();

}

void loop() {
  
  acionar_hora();
  setup_envio();
}
