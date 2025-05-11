/* Bitcoin Ticker 
 * (https://github.com/VeniceInventors/BitcoinTicker)
 * 
 * If Arduino IDE is unable to upload the sketch on TTGO T7-Display,
 * from the esp32 IDF tools folder, use the shell command:
 * esptool.exe write_flash_status --non-volatile 0
 *
 * Button 0 (upper button when USB on left side)
 * - short press refreshes price
 * - long press refresh serial menu
 * Button 35 short press cycles screen brightness
 *
 */

#include "globals.h"
#include "display.h"
#include "prefs.h"
#include "wifi.h"

void setup() {
  Serial.begin(115200);
  setupDisplay();
  setupPrefs();
  setupWifi();
  delay(1000);
  getData();
  displayRefresh();
}

void loop() {
  loopDisplay();
  loopPrefs();
  delay(200);
}
