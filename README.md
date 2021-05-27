# TTGO-T-Display-Crypto-Display-using-Kraken-API!

### My Arduino crypto display displays a cryptos name, 24h high, 24h low and current price. Addional I implemented that if the current price is higher than the price from the before request (default period of 2.5 seconds), the text gets displayed green, and the other way around with the text in red if the price value went down in the last period. 

![image](https://user-images.githubusercontent.com/64910238/119895422-08b6b000-bf3e-11eb-81e5-da96b2bd9a86.png)


## 1. Get the necessary Arduino libraries

- [ArduinoJson](https://www.ardu-badge.com/ArduinoJson/6.10.1) install guide
- Download the [TFT_eSPI library](https://github.com/Xinyuan-LilyGO/TTGO-T-Display) repository and change the `User_Setup_Select` file by commenting `#include <User_Setup.h>` and uncommenting `#include <User_Setups/Setup25_TTGO_T_Display.h>` to get the right driver for your display. After just move the TFT_eSPI folder into the Arduino library folder under the Documents as described in the tutorial.

## 2. Get the ESP32 board driver

- [ESP32 driver install guide](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/)

## 3. Get the Arduino sketch

- Download this repository, unzip, go into the CryptoDisplay folder and open CryptoDisplay.ino file with Arduino

## 4. Configure the variables in the Arduino file

- Insert your networks ssid/name
- Insert your networks password
- If you want to display another crypto than the default Bitcoin you need to change `serverName`, `jsonUrlSymbol` and `cryptoDisplayName` (example for Cardano: 

`String serverName = "https://api.kraken.com/0/public/Ticker?pair=ADAEUR";`

`String jsonUrlSymbol = "ADAEUR";`

`String cryptoDisplayName = "CARDANO";`

If you change the crypto currency you have to format the x coordinates to center the values and co. at the `tft.drawString(txt, x, y, font)` parts where it displays the texts

- 
- Maybe change the data refresh rate 

## 5. Board upload settings

- Go to `Tools > Board > ESP32 Arduino` and select `TTGO LoRa32-OLED V1`
- Upload Speed: `115200`
- Flash Frequency: `40MHz`
- Core Debug Level: `None`
- Select the port and upload 
