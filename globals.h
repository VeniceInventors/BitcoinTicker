// global.h  (Bitcoin Ticker)

/* DISPLAY */
uint32_t current=0; // current BTC price
String hhmm, wallet;

/* PREFS */
struct pref_t {
  bool set;           // for faster check if we have settings
  float btc;          // coins in wallet
  uint32_t mins;      // refresh frequency
  int8_t tz;          // timezone
  String ssid, pass;  // SSID, password
  byte ip[4], gw[4];  // Ticker IP, Gateway IP
};
struct pref_t p;
String myIP, myGW; 
bool prompt=true;

/* WIFI */
#include <WiFi.h>
#include <ArduinoJson.h> 
StaticJsonDocument<2048> doc; // use 2KB buffer for json data
void getData();
unsigned long currentTime=0;
unsigned long refresh=3600000; // default 60 minutes
