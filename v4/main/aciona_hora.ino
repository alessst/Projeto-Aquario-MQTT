#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = -3600*3;

void config_hora(){
  return configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}
void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Falha ao obter a hora");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void acionar_hora(int hora_r_on = 8, int min_r_on = 0, int hora_r_off = 20, int min_r_off = 30, 
                  int hora_g_on = 10, int min_g_on = 30, int hora_g_off = 20, int min_g_off = 0, 
                  int hora_y_on = 12, int min_y_on = 30, int hora_y_off = 18, int min_y_off = 0){

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

