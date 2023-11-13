#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const byte SCREEN_WIDTH = 128;  // OLED display width, in pixels
const byte SCREEN_HEIGHT = 64;  // OLED display height, in pixels



// Declaração para um monitor SSD1306 conectado a I2C (SDA, pinos SCL)
const int OLED_RESET = -1;  // Pino de reset # (ou -1 se compartilhar o pino de reset do Arduino)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



void setup_display() {
 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  display.clearDisplay();
}

void display_print(char texto[20], char texto_2[20], int num_1, int num_2){
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, num_1);
  display.println(texto);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, num_2);
  display.println(texto_2);


}