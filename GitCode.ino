//including libraries

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define BLYNK_PRINT Serial 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>  
#include <SPI.h>
#include <BlynkSimpleEsp8266.h>
#include <SimpleTimer.h>
#include <DHT.h>



// Defining pin and sensor type
Adafruit_SSD1306 display(128, 64, &Wire, -1);
#define DHTPIN D4        
#define DHTTYPE DHT11    
#define FLAME D5
static const int RXPin = 4, TXPin = 5; // declaration of theese pins are not working correctly so earlier it was gpio 13 and 15 i.e d7 and d8 so i had to comment out 1 out of either oled or gps module
WidgetMap myMap(V0);                   // and use the other on d1 and d2 pin ,we need to work on codeing part of changing the data pins of either the gps and oled ,, till then you can only use 1 
int mq135 = A0;



//Defining cred
char auth[] = "";              //Your Project authentication key
char ssid[] = "";                                       // Name of your network (HotSpot or Router name)
char pass[] = "";            // wifi password
const char* password = "";   //Re enter your wifi password
String APIKEY = "";  // your api id
String CityID = "";          //Your City ID


//initializing
bool id = false;
static const uint32_t GPSBaud = 9600;
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
TinyGPSPlus gps; // The TinyGPS++ object
SoftwareSerial ss(RXPin, TXPin);
WiFiClient client;
char servername[] = "api.openweathermap.org";            // remote server we will connect to
String result;



//declaring variables
unsigned int move_index = 1;
int data = 0;
int temperature ;
int humidity ;
int a=0;
int b=0;
int x=0;
int y=0;
int z=0;



//declaring functions
void gpsinfo()
{ 
  if (gps.location.isValid() ) 
  {    
    float latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V3, String(latitude, 6));   
    Blynk.virtualWrite(V4, String(longitude, 6));  
    myMap.location(move_index, latitude, longitude, "GPS_Location");
  }
}  

void flmsen(){
  int fire = digitalRead(FLAME);// read FLAME sensor
  if( fire == LOW)
  {
    
    Blynk.notify("Fire Iminent!!!");
    x=1;
  }
}

void mqsen()
{
  data = analogRead(mq135); 
  Blynk.virtualWrite(V1, data); 
 
  if (data > 600 )
  {
    Blynk.notify("Fire Iminent!!!"); 
    y=1;
  }
}

void dhtsen()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
    
  }
  Blynk.virtualWrite(V5, h);  //V5 is for Humidity
  Blynk.virtualWrite(V6, t);  //V6 is for Temperature
  a=temperature-t;
  b=t-temperature;
  if (b>5) { 
     Blynk.notify("Fire Iminent!!!"); 
    z=1;
    }

}

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
  }
}

void firchck() {
  if ((x==1) && (y==1)){
    Blynk.notify("Fire Started");
    }
   else if ((x==1) && (z==1)){
    Blynk.notify("Fire Started");
    }
   else if ((y==1) && (z==1)) {
    Blynk.notify("Fire started");
    }
   
  }

void oled()                    
{
  if (client.connect(servername, 80))
  { //starts client connection, checks for connection
    client.println("GET /data/2.5/weather?id=" + CityID + "&units=metric&APPID=" + APIKEY);
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection failed");        //error message if no client connect
    Serial.println();
  }

  while (client.connected() && !client.available())
    delay(1);                                          //waits for data
  while (client.connected() || client.available())
  { //connected or data available
    char c = client.read();                     //gets byte from ethernet buffer
    result = result + c;
  }

  client.stop();                                      //stop client
  result.replace('[', ' ');
  result.replace(']', ' ');
  //Serial.println(result);
  char jsonArray [result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';
  StaticJsonDocument<1024> doc;
  DeserializationError  error = deserializeJson(doc, jsonArray);


  if (error) {
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return;
  }

  String location = doc["name"];
  String country = doc["sys"]["country"];
  int temperature = doc["main"]["temp"];
  int humidity = doc["main"]["humidity"];
  float pressure = doc["main"]["pressure"];
  int id = doc["id"];
  float Speed = doc["wind"]["speed"];
  int degree = doc["wind"]["deg"];
  float longitude = doc["coord"]["lon"];
  float latitude = doc["coord"]["lat"];

  Serial.println();
  Serial.print("Country: ");
  Serial.println(country);
  Serial.print("Location: ");
  Serial.println(location);
  Serial.print("Location ID: ");
  Serial.println(id);
  Serial.printf("Temperature: %d°C\r\n", temperature);
  Serial.printf("Humidity: %d %%\r\n", humidity);
  Serial.printf("Pressure: %.2f hpa\r\n", pressure);
  Serial.printf("Wind speed: %.1f m/s\r\n", Speed);
  Serial.printf("Wind degree: %d°\r\n", degree);
  Serial.printf("Longitude: %.2f\r\n", longitude);
  Serial.printf("Latitude: %.2f\r\n", latitude);

  display.clearDisplay();
  display.setCursor(0, 0);            // Start at top-left corner
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.print(" Location: ");
  display.print(country);
  display.print(" ");
  display.print("DDN");
 
  display.println();
  display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
  display.print("T: ");
  display.print(temperature);
  display.print((char)247);
  display.print("C     ");
  display.print("H: ");
  display.print(humidity);
  display.println("%  ");
  display.print("Pressure:");
  display.print(pressure);
  display.println("hpa");
  display.print("WS: ");
  display.print(Speed);
  display.print("m/s  ");
  display.print("WA: ");
  display.print(degree);
  display.println((char)247);
  display.print("Lat: ");
  display.print(latitude);
  display.print(" ");
  display.print("Lon: ");
  display.println(longitude);
  display.display();

}




void setup() {


   Serial.begin(9600);
   
  Serial.print("Connecting to ");
  WiFi.mode(WIFI_STA);   //   create wifi station
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3D for 128x64
  delay(200);
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  display.print("Connecting.");
  display.display();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.print(".");
    display.display();
  }

  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Connected ");
  display.println("IP Address: ");
  display.println(WiFi.localIP());
  display.display();
  delay(1000);
  display.clearDisplay();
  Blynk.begin(auth, ssid, pass);
   dht.begin();
   ss.begin(GPSBaud);
   timer.setInterval(1000L, dhtsen);
   timer.setInterval(1000L, mqsen);
   timer.setInterval(1000L, checkGPS);
   timer.setInterval(1000L, gpsinfo);
   oled();
  
}

void loop() {
  Blynk.run(); // Initiates Blynk
  timer.run(); // Initiates SimpleTimer
  dhtsen();
  mqsen();
  checkGPS();
  gpsinfo();
  firchck();
  delay(1000);
  ESP.deepSleep(15e6);
}
