// wifi.h  (Bitcoin Ticker)

#include <WiFiUdp.h>
#include <HTTPClient.h>

IPAddress nm(255,255,255,0);// NETMASK
const String endpoint ="https://api.kucoin.com/api/v1/market/stats?symbol=BTC-USDT";
bool online = false;
void getIPs();

void setupWifi() {
  int loop = 1;
  Serial.printf("Connecting to %s ", p.ssid);
  tft.printf("Connecting to %s ", p.ssid);
  if (p.ip[0]) WiFi.config(p.ip, p.gw, nm, p.gw); 
  WiFi.begin(p.ssid, p.pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100); Serial.print("."); tft.print("."); loop++;
    if (loop == 100) { Serial.println("FAILED"); tft.print("\nFAILED"); return; }
  }
  myIP = WiFi.localIP().toString(); 
  myGW = WiFi.gatewayIP().toString();  
  Serial.println(String("OK\nIP: "+myIP+" GW: "+myGW)); 
  tft.println(String("OK\nIP: "+myIP+" GW: "+myGW)); online = true;
}

void wifi_off() { if (online) { WiFi.disconnect(true); WiFi.mode(WIFI_OFF); online = false; } }

void wifi_on() { 
  if (!online) { int loop = 1;
    WiFi.mode(WIFI_STA);
    WiFi.begin(p.ssid, p.pass);
    while (WiFi.status() != WL_CONNECTED) { delay(100); loop++;
      if (loop == 100) { Serial.println("\nFAILED TO CONNECT TO WIFI"); return; }
    }
    online = true;
    myIP = WiFi.localIP().toString();
  }
}

bool getData() { // Get current bitcoin price
  if (WiFi.status() != WL_CONNECTED) {
    online = false;
    wifi_on();
    delay(500); // Give 1/2 seconds for WiFi to settle
  }
  if (online) {
    HTTPClient http;
    http.begin(endpoint); //Specify the URL
    int httpCode = http.GET();  //Make the request
    wifi_off();
    if (httpCode == 200) { //Check for the returning code
      String payload = http.getString();
      char inp[payload.length()];
      payload.toCharArray(inp,payload.length());
      deserializeJson(doc,inp);
      String      v=doc["data"]["last"];
      uint32_t high=doc["data"]["high"];
      uint32_t  low=doc["data"]["low"];
      uint64_t    t=doc["data"]["time"];
      uint32_t   t1=(t/1000)+(p.tz*3600);
      char hh[4]; sprintf(hh,"%02d",hour(t1));
      char mm[4]; sprintf(mm,"%02d",minute(t1));
      hhmm = String(String(hh)+":"+String(mm));
      char wa[32]; float wall;
      current=v.toInt();
      wall = current*p.btc;
      sprintf(wa, "%0.2f", wall); wallet = String(wa);
      Serial.printf("%s Wallet: $%s, BTC: $%s\n", hhmm, wallet, v);
      return(true);
    } // if(httpcode == 200)
    else Serial.printf("http.GET(URL) failed! httpCode=%d\n", httpCode); 
  } // if(online)
  return(false); // let caller know that something went wrong
} // getData()

void getIPs() { myIP = WiFi.localIP().toString(); }
