#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


Adafruit_SSD1306 display;
WiFiUDP udp;
NTPClient NTPklijent(udp);
int timer;
String vreme = "";

void resiEkran(Adafruit_SSD1306* display) {
  display->clearDisplay();
  display->setCursor(0, 0);
}

void setup() {
  Serial.begin(115200);

  display = Adafruit_SSD1306(128, 32, &Wire, -1);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) {
    Serial.println("Greska!");
  }

  // EKRAN DEO

  display.clearDisplay();
  display.setTextSize(1.8);
  display.setTextColor(WHITE);
  display.display();
  
  //WIFI DEO

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin("Rezidencijaja", "sifraje123");
  display.clearDisplay();
  display.println("Povezujem se na Rezidencijaja!");
  display.display();
  while (!(WiFi.status() == WL_CONNECTED)) {
    display.print('.');
    display.display();
    delay(500);
  }
  resiEkran(&display);
  display.println("Povezan!");
  display.display();

  // SAT

  NTPklijent.begin();

  // VREME

  timer = 10;
  
}

void loop() {
  NTPklijent.update();
  resiEkran(&display);
  display.println(NTPklijent.getFormattedTime());
  display.display();
  timer++;

  if (timer > 9) {

    HTTPClient http;

    String URL = "http://api.weatherapi.com/v1/current.json?key=9df4266745e1429f9bf162556211912&q=Zemun&aqi=no";

    http.begin(URL);

    int httpKod = http.GET();

    if (httpKod == 200) {
      String response = http.getString();
      DynamicJsonDocument json(1024);
      deserializeJson(json, response);
      float temperatura = json["current"]["temp_c"];
      const char* uslovi = json["current"]["condition"]["text"];
      vreme = String(temperatura, 1) + " C " + uslovi; 
    }

    timer = 0;
  }
  display.println(vreme);
  display.display();
  delay(1000);
}