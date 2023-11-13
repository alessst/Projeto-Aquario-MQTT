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

void setupLEDs() {
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_Y, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void valor_sensor(){
  analogValue = analogRead(LIGHT_SENSOR_PIN);
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