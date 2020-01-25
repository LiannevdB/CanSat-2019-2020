
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

const int DHT_DATA = A0;
const int MPU = 0x68;

// ------------------------------------------------------------------------------------------------------------------------------

dht DHT;
Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
SoftwareSerial mySerial(10,11);

int number = 0;
float temp, hum, pres, alt;
float baseAlt = 0;

// MPU6050
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float AccAngleX, AccAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float roll, pitch, yaw;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY, GyroErrorZ;
float elapsedTime, currentTime, previousTime;
int c = 0;
float acc_sens = 16384.0;
float gyro_sens = 131.0;

// ------------------------------------------------------------------------------------------------------------------------------

void setup() {

  Serial.begin(9600);
  mySerial.begin(9600);

  if (!bmp.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  // MPU6050
  Wire.beginTransmission(MPU);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU);
  Wire.write(0x1B);
  Wire.write(0x10);
  Wire.endTransmission(true);

  calculate_IMU_error();
  //print_IMU_error();
  //printDesc();
  
  
  

// ------------------------------------------------------------------------------------------------------------------------------

}

void loop() {
  DHT.read11(DHT_DATA);
  
  getData();
  MPU_Controller();
  printData();

  delay(1000);
}

// -------------------------------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------------------------------------------------------------


void getData() {
  temp = bmp.readTemperature();
  hum = DHT.humidity;
  pres = bmp.readPressure();
  alt = bmp.readAltitude(100);
}

void MPU_Controller() {
  readAcc();
  readGyro();
  calcAngles();
 
  //printMPU(); //Testing
}

void readAcc() {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  
  AccX = (Wire.read() << 8 | Wire.read()) / acc_sens;
  AccY = (Wire.read() << 8 | Wire.read()) / acc_sens;
  AccZ = (Wire.read() << 8 | Wire.read()) / acc_sens; 
}

void readGyro() {
  previousTime = currentTime;
  currentTime = millis();
  elapsedTime = (currentTime - previousTime) / 1000;

  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);

  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;

  GyroX = GyroX - GyroErrorX;
  GyroY = GyroY - GyroErrorY;
  GyroZ = GyroZ - GyroErrorZ;
}

void calcAngles() {
  AccAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI) + AccErrorX; // AccErrorY
  AccAngleY = (atan(-1 * AccX / sqrt(pow(AccY, 2) + pow(AccZ, 2))) * 180 / PI) + AccErrorY; // AccErrorX

  gyroAngleX = gyroAngleX + GyroX * elapsedTime;
  gyroAngleY = gyroAngleY + GyroY * elapsedTime;
  
  yaw = yaw + GyroZ * elapsedTime;
  roll = 0.96 * gyroAngleX + 0.04 * AccAngleX;
  pitch = 0.96 * gyroAngleY + 0.04 * AccAngleY;
}

void printMPU() {
  Serial.print("Yaw: ");
  Serial.println(yaw);
  Serial.print("Roll: ");
  Serial.println(roll);
  Serial.print("Pitch: ");
  Serial.println(pitch);

  mySerial.print("Yaw: ");
  mySerial.println(yaw);
  mySerial.print("Roll: ");
  mySerial.println(roll);
  mySerial.print("Pitch: ");
  mySerial.println(pitch);
}

void printData() {
  Serial.print(temp);
  Serial.print(",");
  Serial.print(hum);
  Serial.print(",");
  Serial.print(pres);
  Serial.print(",");
  Serial.print(alt);
  //Serial.println("/");
  
  mySerial.print(temp);
  mySerial.print(",");
  mySerial.print(hum);
  mySerial.print(",");
  mySerial.print(pres);
  mySerial.print(",");
  mySerial.print(alt);
  //mySerial.println(",");

  /* MPU DATA -- REMOVE PRINTLN PREV.

   Serial.print(yaw);
  Serial.print("/");
  Serial.print(roll);
  Serial.print("/");
  Serial.println(pitch);

  mySerial.print(yaw);
  mySerial.print("/");
  mySerial.print(roll);
  mySerial.print("/");
  mySerial.println(pitch);

  */
}

void printDesc() {
  Serial.println("temp/hum/pres/alt");
  mySerial.println("temp/hum/pres/alt");
  
  //Serial.println("temp/hum/pres/alt/yaw/roll/pitch");
  //mySerial.println("temp/hum/pres/alt/yaw/roll/pitch");
}

void calculate_IMU_error() {
  // We can call this funtion in the setup section to calculate the accelerometer and gyro data error. From here we will get the error values used in the above equations printed on the Serial Monitor.
  // Note that we should place the IMU flat in order to get the proper values, so that we then can the correct values
  // Read accelerometer values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0 ;
    // Sum all readings
    AccErrorX = AccErrorX + ((atan((AccY) / sqrt(pow((AccX), 2) + pow((AccZ), 2))) * 180 / PI));
    AccErrorY = AccErrorY + ((atan(-1 * (AccX) / sqrt(pow((AccY), 2) + pow((AccZ), 2))) * 180 / PI));
    c++;
  }
  //Divide the sum by 200 to get the error value
  AccErrorX = AccErrorX / 200;
  AccErrorY = AccErrorY / 200;
  c = 0;
  // Read gyro values 200 times
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyroX = Wire.read() << 8 | Wire.read();
    GyroY = Wire.read() << 8 | Wire.read();
    GyroZ = Wire.read() << 8 | Wire.read();
    // Sum all readings
    GyroErrorX = GyroErrorX + (GyroX / 131.0);
    GyroErrorY = GyroErrorY + (GyroY / 131.0);
    GyroErrorZ = GyroErrorZ + (GyroZ / 131.0);
    c++;
  }
  //Divide the sum by 200 to get the error value
  GyroErrorX = GyroErrorX / 200;
  GyroErrorY = GyroErrorY / 200;
  GyroErrorZ = GyroErrorZ / 200;
}

void print_IMU_error() {
  mySerial.print(AccErrorX);
  mySerial.print("/");
  mySerial.print(AccErrorY);
  mySerial.print("/");
  mySerial.print(GyroErrorX);
  mySerial.print("/");
  mySerial.print(GyroErrorY);
  mySerial.print("/");
  mySerial.println(GyroErrorZ);
}
