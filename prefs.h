// prefs.h (Bitcoin Ticker)

#include <Preferences.h>
Preferences prefs;

String reqString(String s) { // Serial prompt
  Serial.print(s);
  while (Serial.available() == 0) { delay(200); } 
  s = Serial.readStringUntil(10); s.trim();
  return(s);
}

void reqIP(String s, byte * Ip) { // Request IP
  Serial.print(s);
  while (Serial.available() == 0) { delay(200); }
  s = Serial.readStringUntil(10); s.trim();
  byte d1 = s.indexOf('.');
  byte d2 = s.indexOf('.', d1 + 1 );
  byte d3 = s.indexOf('.', d2 + 1 );
  String ips[4];
  ips[0]=s.substring(0,d1);
  ips[1]=s.substring(d1+1,d2);
  ips[2]=s.substring(d2+1,d3);
  ips[3]=s.substring(d3+1);
  for(byte i=0;i<4;i++) { Ip[i]=ips[i].toInt(); }
}

void newPrefs() { // Called when no settings have been saved yet
  displayMessage("USE SERIAL TERMINAL TO CONFIGURE TICKER");
  Serial.println("Initial ticker configuration:");
  p.ssid = reqString("SSID: ");
  Serial.println(p.ssid);
  p.pass = reqString("Password: ");
  Serial.println(p.pass);
  reqIP("Ticker IP (e.g. 192.168.0.2 or press ENTER for DHCP): ", p.ip);
  if (p.ip[0]) {
    Serial.printf("%d.%d.%d.%d\n",p.ip[0],p.ip[1],p.ip[2],p.ip[3]);
    reqIP("Router IP (e.g. 192.168.0.1): ", p.gw);
    if (p.gw[0]) Serial.printf("%d.%d.%d.%d\n", p.gw[0],p.gw[1],p.gw[2],p.gw[3]);
    else Serial.printf("Invalid gateway IP: %d.%d.%d.%d, network might not work!\n", p.gw[0],p.gw[1],p.gw[2],p.gw[3]);
  } else { p.ip[0]=(0,0,0,0); p.gw[0]=(0,0,0,0); Serial.println("DHCP"); }
  p.tz = (int8_t) reqString("Timezone (i.e. -8 for PST or -7 for PDT): ").toInt();
  Serial.println(p.tz);
  p.mins = (uint32_t) reqString("BTC price refresh frequency (minutes): ").toInt();
  Serial.println(p.mins);
  p.btc = reqString("Bitcoins in wallet: ").toFloat();
  Serial.printf("%0.7f\n",p.btc);
  p.set = true;
}

void loadPrefs() {
  p.btc  = prefs.getFloat("coins");
  p.tz   = prefs.getChar("timezone");
  p.mins = prefs.getUInt("minutes");
  p.ssid = prefs.getString("ssid");
  p.pass = prefs.getString("pass");
  prefs.getBytes("ip", p.ip, sizeof(p.ip));
  prefs.getBytes("gw", p.gw, sizeof(p.gw));
  Serial.printf("Coins: %f, TZ: %d, ssid: %s, pass: %s, IP: %d.%d.%d.%d, GW: %d.%d.%d.%d\n", 
    p.btc, p.tz, p.ssid, p.pass, p.ip[0],p.ip[1],p.ip[2],p.ip[3], p.gw[0],p.gw[1],p.gw[2],p.gw[3]);
}

bool savePrefs() { bool err=false;
//  p = {0.00137895, -8, "offline", "aassuuss", {192,168,1,234}, {192,168,1,1} };
  if (!prefs.putBool("set", p.set)) err=true;
  if (!prefs.putFloat("coins", p.btc)) err=true;
  if (!prefs.putChar("timezone", p.tz)) err=true;
  if (!prefs.putUInt("minutes", p.mins)) err=true;
  if (!prefs.putString("ssid", p.ssid)) err=true;
  if (!prefs.putString("pass", p.pass)) err=true;
  if (!prefs.putBytes("ip", p.ip, sizeof(p.ip))) err=true;
  if (!prefs.putBytes("gw", p.gw, sizeof(p.gw))) err=true;
  return (err);
}

void prefs2Vars() {}

void setupPrefs() {
  prefs.begin("prefs"); // namespace
  p.set = prefs.getBool("set");
  if (!p.set) newPrefs();
  else loadPrefs();
  prefs2Vars();
}

void loopPrefs() { // Called by main loop
  if (prompt) {    // Display menu if needed
    Serial.printf("\nBTC Ticker (IP: %s GW: %s)\n", myIP, myGW);
    Serial.printf("[1] Set Bitcoins (%0f)\n", p.btc);
    Serial.printf("[2] Adjust timezone (%d)\n", p.tz);
    Serial.printf("[3] Change BTC price refresh frequency (%ld mins)\n", p.mins);
    Serial.printf("[4] Configure WiFi (%s/%s)\n", p.ssid, (p.ip[0]) ? myIP : "DHCP");
    Serial.println("[5] Save all changes");
    Serial.println("[6] Clear all settings");
    Serial.println("[7] Reboot");
    Serial.println("[8] Refresh display");
    prompt=false;
  }

  while (Serial.available() > 0) { // Handle menu and settings input from user
    String inChar = Serial.readStringUntil(10); inChar.trim();
    switch (inChar.toInt()) {
      case 1: {
        float btc = reqString("Bitcoins in wallet: ").toFloat();
        if (btc) p.btc=btc; else p.btc=0;
        Serial.println(btc);
        break; }
      case 2: {
        int8_t tz = (int8_t) reqString("Timezone (i.e. -8 for PST or -7 for PDT): ").toInt();
        if (tz) p.tz=tz; else p.tz=0;
        Serial.println(p.tz);
        break; }
      case 3: {
        uint32_t mins = (uint32_t) reqString("BTC price refresh frequency (minutes): ").toInt();
        if (mins) p.mins=mins; else p.mins=60;
        Serial.println(p.mins);
        break; }
      case 4:
        p.ssid = reqString("SSID: ");
        Serial.println(p.ssid);
        p.pass = reqString("Password: ");
        Serial.println(p.pass);
        reqIP("Ticker IP (e.g. 192.168.0.2 or press ENTER for DHCP): ", p.ip);
        if (p.ip[0]) {
          Serial.printf("%d.%d.%d.%d\n",p.ip[0],p.ip[1],p.ip[2],p.ip[3]);
          reqIP("Router IP (e.g. 192.168.0.1): ", p.gw);
          if (p.gw[0])
            Serial.printf("%d.%d.%d.%d\n",p.gw[0], p.gw[1],p.gw[2],p.gw[3]);
          else
            Serial.printf("Invalid gateway IP: %d.%d.%d.%d, network might not work!\n", p.gw[0],p.gw[1],p.gw[2],p.gw[3]);
        } else {
          p.ip[0]=(0,0,0,0); p.gw[0]=(0,0,0,0);
          Serial.println("DHCP");
        }
        break;
      case 5:
        if (savePrefs()) Serial.println("Error saving settings!");
        else Serial.println("Settings saved");
        break;
      case 6:
        if (prefs.clear()) Serial.println("Settings cleared");
        else Serial.println("Error: Unable to clear settings!");
        break;
      case 7:
        ESP.restart();
        break;
      case 8:
        if (getData()) {
          displayCalc();
          displayRefresh();
        }
        break;
    }
    prompt=true;
  }
}
