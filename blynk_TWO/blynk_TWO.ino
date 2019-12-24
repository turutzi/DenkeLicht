//Arduino Sketch for Toms "DenkeLicht"
//just put you Wifi Data and the API_Token from Blynk in the "own_data.h" an flash it on your Wemos D1
//and you need to install the "BLYNK Libary"

#include "own_data.h"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "SECRET_TOKEN_FROM_TWO";

char ssid[] = "SECRET_SSID";
char pass[] = "SECRET_PASS";

#define LED_PIN 4
#define BUTTON_PIN 5

unsigned long lightTimerExpires;
boolean lightTimerActive = false;

boolean sendLight = false;

WidgetBridge bridge1(V1);

BLYNK_WRITE(V5)
{
  if (param.asInt() == 1000)
  {
    digitalWrite(LED_PIN, HIGH);
    lightTimerActive = true;
    lightTimerExpires = millis() + (5 * 1000 * 60);
  }
}

BLYNK_CONNECTED() {
  bridge1.setAuthToken("SECRET_TOKEN_FROM_ONE");
}

ICACHE_RAM_ATTR void handleInterrupt()
{
  sendLight = true;
}

void setup()
{
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), handleInterrupt, RISING);

  pinMode(LED_PIN, OUTPUT);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();

  if (sendLight == true)
  {
    bridge1.virtualWrite(V4, 1000);
    if(digitalRead(LED_PIN) == true)
    {
      digitalWrite(LED_PIN, LOW);
    }
    else
    {
      digitalWrite(LED_PIN, HIGH);
    }
    delay(200);
    bridge1.virtualWrite(V4, 0);
    if(digitalRead(LED_PIN) == true)
    {
      digitalWrite(LED_PIN, LOW);
    }
    else
    {
      digitalWrite(LED_PIN, HIGH);
    }
    sendLight = false;
  }

  if (lightTimerActive && millis() > lightTimerExpires)
  {
    lightTimerActive = false;
    digitalWrite(LED_PIN, LOW);
  }
}
