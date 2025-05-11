# BitcoinTicker
Display current BTC price and wallet value on LilyGo/TTGO T-Display with esp32 and similar microcontrollers
(Public Domain)

This is a simple ticker that shows the current value of bitcoins owned, the current BTC price, and the time when the price was last updated.
Nothing fancy, no chart, just a simple way to keep an eye on your bitcoins fortune rollercoaster ride.
The wifi settings, refresh rate, and amount of owned Bitcoins can be update via a prompt on the serial terminal, and the brightness of the screen can be adjusted via the hardware buttons.
After the settings have been saved in flash memory, the display doesn't need to be connected to a computer or terminal to show the ticker. 
If it remains connected to a serial terminal, it could be used to log the price when it refreshes the data, bust most cryptocurrency websites offer historical charts, so it's not really needed.

I wrote the code for my own use, but decided to share it in case it is useful for someone else, so I made it a bit easier to use than the original which had hard-coded settings.
I use it on a TTGO T-Display with 240x160 1.14" LCD display, but the code can easily be adapted for any microcontroller and display.
It uses the kucoin API as the json data they provide has a convenient format, and provides the current time, so it doesn't need to connect to an NTP server.

If you find the ticker useful and would like to make a donation, please visit https://veniceinventors.com/cryptocurrencies.htm

Feel free to suggest features and especially report any issue you may find, so that I can make it better for everyone.

![Screen1](https://github.com/user-attachments/assets/24480755-f6ea-48a0-be2a-a0a3cc2916cc)
![Screen2](https://github.com/user-attachments/assets/5378664c-7bd0-4921-a697-4bf97645db39)
