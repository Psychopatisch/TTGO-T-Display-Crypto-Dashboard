#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include <SPI.h>

//USER INPUTS!!!
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
String serverName = "https://api.kraken.com/0/public/Ticker?pair=BTCEUR"; //Kraken API URL to Crypto datas -> [Enter Your Crypto Link here](exemple here "ADAEUR")
String jsonUrlSymbol = "XXBTZEUR"; //The Json File crypto name (you need to go to the serverName URL and read out the first element in the "result:" Json (example: for BTCEUR its "XXBTZEUR"))
String cryptoDisplayName = "BITCOIN"; //What should be Displayed as the Name of the crypto currency
int refreshDelay = 2500; //How often should it get and display the new data

//String currency="EUR"; //[Additional] Display price currency
//END


//String sensorReadings;
String price;


//TFT Display 

#define TFT_MOSI        19
#define TFT_SCLK        18
#define TFT_CS          5
#define TFT_DC          16
#define TFT_RST         23

#define TFT_BL          4 
#define ADC_EN          14
#define ADC_PIN         34
#define BUTTON_1        35
#define BUTTON_2        0

TFT_eSPI tft = TFT_eSPI(135, 240); 

#define TFT_AQUA  0x04FF
#define TFT_GRAY  0x8410

//Setting up values 

double lastValue = 0.00;
double value;
int high;
int low;
String sensorReadings;

void setup() {
  Serial.begin(115200);
  
  //initialize the TTGO T-Display Display
  tft.setRotation(1);  
  tft.fillScreen(TFT_BLACK);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
  tft.init();
  tft.fillScreen(TFT_BLACK);

  delay(1000);

  //Connect to WiFi
  WiFi.begin(ssid, password);
  delay(1000);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.drawString("Connecting ...", 40, 80, 1);
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  tft.fillScreen(TFT_BLACK);
  delay(1000);
  tft.setTextColor(TFT_GREEN);
  tft.drawString("WiFi connected", 27, 50, 2);
  
  
  //delay(3000);
  //tft.fillScreen(TFT_BLACK);
  //delay(500);
  //Serial.println("2");
  
}

void loop() {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){     
      sensorReadings = httpGETRequest(serverName);//Get Kraken Api server datas
      Serial.println(sensorReadings);
      //Get Json object from Kraken server
      StaticJsonDocument<768> doc;
      DeserializationError error = deserializeJson(doc, sensorReadings);
      
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        tft.drawString("Error with JSON", 105, 10, 5);
        return;
      }
      JsonObject result_crypto = doc["result"][jsonUrlSymbol];
      JsonArray result_crypto_a = result_crypto["a"];
      JsonArray result_crypto_h = result_crypto["h"];
      JsonArray result_crypto_l = result_crypto["l"];
      const char* result_crypto_a_0 = result_crypto_a[0];
      const char* result_crypto_h_0 = result_crypto_h[0];
      const char* result_crypto_l_0 = result_crypto_l[0];
      value = atof(result_crypto_a_0);
      high = atoi(result_crypto_h_0);
      low = atoi(result_crypto_l_0);
        
      tft.fillScreen(TFT_BLACK);

      tft.setTextSize(4);
      tft.setTextColor(TFT_ORANGE); //Change the Color of the crypto name (example: "TFT_GREEN")
      tft.drawString(cryptoDisplayName, 35, 10, 1); //Displays the name with (X, Y, TxtFont(1-2))
      
      //Displays the price in green or red if the price in the last delay period went up or down (feature)
      if (lastValue <= value){
        tft.setTextColor(TFT_GREEN);
        }   
      if (lastValue > value) {
       tft.setTextColor(TFT_RED);
       }
      lastValue = value;
      
      tft.setTextSize(3);
      tft.drawString(String(value), 15, 78, 2); //Displays the price with (X, Y, TxtFont(1-2))
    
      tft.setTextSize(2);
      tft.drawString("EUR", 199, 96, 1);
      
      tft.setTextSize(2);
      tft.setTextColor(TFT_GREEN);
      tft.drawString("H:" + String(high), 15, 55, 1);
      tft.setTextColor(TFT_RED);
      tft.drawString("L:" + String(low), 135, 55, 1);
      

      


    }
    else {
      tft.fillScreen(TFT_BLACK);
      Serial.println("WiFi Disconnected");
      tft.setTextSize(3);
      tft.drawString("WiFi Problem", 15, 60, 4);
    }
  
  delay(refreshDelay);
  
  }
    
//Connecting to server and returning the data

String httpGETRequest(String serverName) {
  HTTPClient http;
    
  http.begin(serverName);
  
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}
