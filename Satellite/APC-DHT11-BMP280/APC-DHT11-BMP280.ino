
#include <SoftwareSerial.h>
#include <dht.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 6 
#define BMP_CS 7

int DHT_DATA = A0;

// ------------------------------------------------------------------------------------------------------------------------------

dht DHT;
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
SoftwareSerial mySerial(10,11);

int number = 0;
float temp, hum, pres, alt;
float baseAlt = 100;

// ------------------------------------------------------------------------------------------------------------------------------

void setup() {

  Serial.begin(9600);
  mySerial.begin(9600);

  if (!bmp.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

// ------------------------------------------------------------------------------------------------------------------------------

}

void loop() {
  DHT.read11(DHT_DATA);

  // get sensor data
  getData();

  // Testing APC220
  (number ++);
  Serial.print ("APC220   ");
  Serial.println (number);
  mySerial.print ("APC220   ");
  mySerial.println (number);

  // Print sensor data
  printSensorData();

  // EXRTA ENTER TO CLEAN UP
  mySerial.println();
  Serial.println();

  // Delay between 
  delay(2500);
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------------------------------------


void getData() {
  temp = bmp.readTemperature();
  hum = DHT.humidity;
  pres = bmp.readPressure();
  alt = bmp.readAltitude(baseAlt);
}

void printSensorData() {
  printTemp();
  printHum();
  printPres();
  printAlt();
}


void printHum() {
  Serial.print("Current Humidity: ");
  Serial.print(hum);
  Serial.print("%");
  Serial.println();
  
  mySerial.print("Current Humidity: ");
  mySerial.print(hum);
  mySerial.print("%");
  mySerial.println();  
}


void printTemp() {
  Serial.print("Current Temperature: ");
  Serial.print(temp);
  Serial.print("*C");
  Serial.println();
  
  mySerial.print("Current Temperature: ");
  mySerial.print(temp);
  mySerial.print("*C");
  mySerial.println();
}

void printPres() {
  Serial.print("Current Pressure: ");
  Serial.print(bmp.readPressure() / 1000);
  Serial.print(" kPa");
  Serial.println();
  
  mySerial.print("Current Pressure: ");
  mySerial.print(bmp.readPressure() / 1000);
  mySerial.print(" kPa");
  mySerial.println();
}

void printAlt() {
  Serial.print(F("Current Altitude: "));
  Serial.print(bmp.readAltitude(1000));
  Serial.print(" m");
  Serial.println();
  
  mySerial.print("Current Altitude: ");
  mySerial.print(bmp.readAltitude(1000));
  mySerial.print(" m");
  mySerial.println();
}
