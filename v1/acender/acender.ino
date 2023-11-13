//Inclusao das bibliotecas
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>  
#include <DallasTemperature.h>

const byte SCREEN_WIDTH = 128;  // OLED display width, in pixels
const byte SCREEN_HEIGHT = 64;  // OLED display height, in pixels

#define PINO_RELE_MANHA 18  //pino definido para conexao com o relé da lampada azul de manha
#define PINO_RELE_TARDE 19  //pino definido para conexao com o relé da lampada azul de tarde
#define PINO_RELE_NOITE 5  //pino definido para conexao com o relé da lampada branca de noite
#define PINO_RELE_TEMP 23  // pino definido para conexao com o relé da lampada de temperatura
#define PINO_DADOS_TEMP 4 //pino de dados do sensor está ligado na porta 2 do Arduino

// variaveis para acionar relé da lampada azul de manha
const int HORA_AZUL_MANHA_ON = 12;
const int MINUTO_AZUL_MANHA_ON = 36;
const int HORA_AZUL_MANHA_OFF = 12;
const int MINUTO_AZUL_MANHA_OFF = 42;

// variaveis para acionar relé da lampada azul de tarde
const int HORA_AZUL_TARDE_ON = 12;
const int MINUTO_AZUL_TARDE_ON = 37;
const int HORA_AZUL_TARDE_OFF = 12;
const int MINUTO_AZUL_TARDE_OFF = 41;

// variaveis para acionar relé da lampada branca de noite
const int HORA_BRANCA_NOITE_ON = 12;
const int MINUTO_BRANCA_NOITE_ON = 38;
const int HORA_BRANCA_NOITE_OFF = 12;
const int MINUTO_BRANCA_NOITE_OFF = 40;

//Variaveis para display 
const int TAMANHO_LETRA = 1;
const int TAMANHO_HORA = 2;
const int POSICAO_Y = 16;
const int POSICAO_HORA = 1;

// configurações dos sensor de temperatura
const int ALARME_TEMP = 30;
OneWire oneWire(PINO_DADOS_TEMP);  /*Protocolo OneWire*/
DallasTemperature sensors(&oneWire); /*encaminha referências OneWire para o sensor*/



// Declaração para um monitor SSD1306 conectado a I2C (SDA, pinos SCL)
const int OLED_RESET = -1;  // Pino de reset # (ou -1 se compartilhar o pino de reset do Arduino)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//As linhas de codigo a seguir devem ser comentadas, ou descomentadas, de acordo com o modelo de RTC utilizado (DS1307 ou DS3132)
RTC_DS1307 rtc;  //Objeto rtc da classe DS1307
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};  //Dias da semana

void setup() {
  
  // inicialize com o endereço I2C 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  Serial.begin(115200);                        //Inicializa a comunicacao serial


  //rtc.adjust(DateTime(2023, 5, 1, 21, 35, 0));  //Ajusta o tempo do RTC para a data e hora definida pelo usuario.
  delay(10);                                      //100 Milissegundos
  pinMode (PINO_RELE_TEMP, OUTPUT); 
  pinMode(PINO_RELE_MANHA, OUTPUT);
  pinMode(PINO_RELE_TARDE, OUTPUT);
  pinMode(PINO_RELE_NOITE, OUTPUT);
  digitalWrite(PINO_RELE_MANHA, HIGH);
  digitalWrite(PINO_RELE_TARDE, HIGH);
  digitalWrite(PINO_RELE_NOITE, HIGH);
  digitalWrite(PINO_RELE_TEMP, LOW);
  
  // Limpe o buffer
  display.clearDisplay();
    
}

void loop() {
 
  sensors.requestTemperatures(); /* Envia o comando para leitura da temperatura */
  /********************************************************************/
  Serial.print("A temperatura é: "); /* Printa "A temperatura é:" */
  Serial.println(sensors.getTempCByIndex(0)); /* Endereço do sensor */

  delay(1000);                                       //1 Segundo

  dados_temperatura();

}

void controla_luz(DateTime tempo, const int hora_on, const int minuto_on, const int hora_off, const int minuto_off, int pino_rele){
  if ((tempo.hour() == hora_on) && (tempo.minute() == minuto_on)) {
    digitalWrite(pino_rele, HIGH);
  }
  if ((tempo.hour() == hora_off) && (tempo.minute() == minuto_off)) {
    digitalWrite(pino_rele, LOW);
  }
}

void display_time(DateTime tempo){
  display.setTextSize(TAMANHO_LETRA);
  display.setTextColor(WHITE);
  display.setCursor(0,POSICAO_Y);
  display.println(tempo.day(), DEC);
  display.setTextSize(TAMANHO_LETRA);
  display.setTextColor(WHITE);
  display.setCursor(10,POSICAO_Y);
  display.println("|");
  display.setTextSize(TAMANHO_LETRA);
  display.setTextColor(WHITE);
  display.setCursor(14,POSICAO_Y);
  display.println(tempo.month(), DEC);
  display.setTextSize(TAMANHO_LETRA);
  display.setTextColor(WHITE);
  display.setCursor(26,POSICAO_Y);
  display.println("|");
  display.setTextSize(TAMANHO_LETRA);
  display.setTextColor(WHITE);
  display.setCursor(32,POSICAO_Y);
  display.println(tempo.year(), DEC);
  display.setTextSize(TAMANHO_LETRA);
  display.setTextColor(WHITE);
  display.setCursor(57,POSICAO_Y);
  display.println("-");
  display.setTextSize(TAMANHO_LETRA);
  display.setTextColor(WHITE);
  display.setCursor(64,POSICAO_Y);
  display.println(diasDaSemana[tempo.dayOfTheWeek()]);
//---------------------------------------------------------------------------------------///
  display.setTextSize(TAMANHO_HORA);
  display.setTextColor(WHITE);
  display.setCursor(0,POSICAO_HORA);
  display.println(tempo.hour(), DEC);
  display.setTextSize(TAMANHO_HORA);
  display.setTextColor(WHITE);
  display.setCursor(20,POSICAO_HORA);
  display.println(":");
  display.setTextSize(TAMANHO_HORA);
  display.setTextColor(WHITE);
  display.setCursor(29,POSICAO_HORA);
  display.println(tempo.minute(), DEC);
  display.setTextSize(TAMANHO_HORA);
  display.setTextColor(WHITE);
  display.setCursor(50,POSICAO_HORA);
  display.println(":");
  display.setTextSize(TAMANHO_HORA);
  display.setTextColor(WHITE);
  display.setCursor(58,POSICAO_HORA);
  display.println(tempo.second(), DEC);

}
void dados_temperatura(){
  if (sensors.getTempCByIndex(0) > ALARME_TEMP){
      digitalWrite(PINO_RELE_TEMP, HIGH);      
  }else{
      digitalWrite(PINO_RELE_TEMP,LOW);
}
}
void display_temperatura(){
  display.setTextSize(TAMANHO_LETRA);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.println("T:");
  display.setTextSize(TAMANHO_LETRA);
  display.setTextColor(WHITE);
  display.setCursor(10,28);
  display.println(sensors.getTempCByIndex(0));
}
void debug(DateTime tempo){
  
  /********************************************************************/
  Serial.print("A temperatura é: "); /* Printa "A temperatura é:" */
  Serial.println(sensors.getTempCByIndex(0)); /* Endereço do sensor */
  
  Serial.print("Data: ");
  Serial.print(tempo.day(), DEC);                    //Imprime dia
  Serial.print('/');                                 //Imprime barra
  Serial.print(tempo.month(), DEC);                  //Imprime mes
  Serial.print('/');                                 //Imprime barra
  Serial.print(tempo.year(), DEC);                   //Imprime ano
  Serial.print(" / Dia da semana: ");                //Imprime texto
  Serial.print(diasDaSemana[tempo.dayOfTheWeek()]);  //Imprime dia da semana
  Serial.print(" / Horas: ");                        //Imprime texto
  Serial.print(tempo.hour(), DEC);                   //Imprime hora
  Serial.print(':');                                 //Imprime dois pontos
  Serial.print(tempo.minute(), DEC);                 //Imprime os minutos
  Serial.print(':');                                 //Imprime dois pontos
  Serial.print(tempo.second(), DEC);                 //Imprime os segundos
  Serial.println();                                  //Quebra de linha
  //---------------------------------------------------------------------------------//
}