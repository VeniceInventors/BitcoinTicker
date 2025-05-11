// display.h  (Bitcoin Ticker)

#include "tftespi.h" // using local copy to avoid editing library
#include <TimeLib.h>
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;  
int debounce=0;
int brightness[5]={40,80,120,160,200};
int bright=1;

void setupDisplay() {
  pinMode(35, INPUT_PULLUP);
  pinMode( 0, INPUT_PULLUP);
  tft.init();
  tft.setRotation(3);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.setTextWrap(1,1);
  tft.setTextPadding(240);
  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, brightness[bright]);
}

void displayCalc() { // Format json data for display
  String      v=doc["data"]["last"];
  uint32_t high=doc["data"]["high"];
  uint32_t  low=doc["data"]["low"];
  uint64_t    t=doc["data"]["time"];
  uint32_t   t1=(t/1000)+(p.tz*3600);
  char hh[4]; sprintf(hh,"%02d",hour(t1));
  char mm[4]; sprintf(mm,"%02d",minute(t1));
  hhmm = String(String(hh)+":"+String(mm));
  char wa[32]; float wall;
  current = v.toInt();
  wall = current*p.btc;
  sprintf(wa, "%0.2f", wall); wallet = String(wa);
  Serial.printf("%s Wallet: $%s, BTC: $%s\n", hhmm, wallet, v); 
}

void displayRefresh() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW,TFT_BLACK); tft.setTextSize(wallet.length() > 6 ? 4 : 5);
  tft.drawString("$"+String(wallet),4,4);
  tft.setTextColor(TFT_ORANGE,TFT_BLACK); tft.setTextSize(3);
  tft.drawString("BTC: $"+String(current),4,64);
  tft.setTextColor(TFT_WHITE,TFT_NAVY);      tft.setTextSize(2);
  tft.drawString("UPDATED: "+hhmm,4,110);
}

void loopDisplay() { // called by main loop
  if (millis() > currentTime + (p.mins*60000)) {
    Serial.println("Refreshing BTC data");
    if (getData()) { displayRefresh(); }
    currentTime = millis(); 
  }  
  if (digitalRead(35) == 0 && debounce == 0) { // Button 35 controls screen brightness
    debounce = 1; bright++; 
    if (bright == 6) bright = 0; ledcWrite(pwmLedChannelTFT, brightness[bright]);
    Serial.printf("brightnesss: %d\n",bright); delay(500); debounce = 0;
  }
  if (digitalRead(0) == 0 && debounce == 0) { // Button 0 refreshes price or prefs menu
    while (digitalRead(0) == 0) { debounce++; delay(100); }
    if (debounce < 10) { // Short press, request manual refresh
      getData();
      displayRefresh();
    }
    else { prompt = true; } // Long press, request serial prompt
    delay(500); debounce = 0;
  }
}

void displayMessage(String message) { tft.print(message); }
