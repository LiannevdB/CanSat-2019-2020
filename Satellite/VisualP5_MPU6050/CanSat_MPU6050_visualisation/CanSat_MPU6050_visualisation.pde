/*
    Arduino and MPU6050 IMU - 3D Visualization
*/

import processing.serial.*;
import java.awt.event.KeyEvent;
import java.io.IOException;

Serial myPort;

String data="";
float roll, pitch,yaw;

String serialPortLocation = "/dev/cu.usbserial-1410";

void setup() {
  size (640, 480, P3D);
  myPort = new Serial(this, serialPortLocation, 9600); // starts the serial communication
  myPort.bufferUntil('\n');
}

void draw() {
  text("Roll: " + int(roll) + "     Pitch: " + int(pitch), 50, 150);
  
  translate(width/2, height/2, 0);
  background(233);
  textSize(22);
  

  // Rotate the object
  rotateX(radians(-pitch)); //-pitch
  rotateZ(radians(-roll)); // -roll
  rotateY(radians(yaw));   // yaw
  
  // 3D 0bject
  textSize(30);  
  fill(0, 76, 153);
  box (150, 150, 150); // Draw box
  textSize(25);
  fill(255, 255, 255);
  //text("Project Meteor", 0, 0, 0);

  //delay(10);
  //println("ypr:\t" + angleX + "\t" + angleY); // Print the values to check whether we are getting proper values
}

// Read data from the Serial Port
void serialEvent (Serial myPort) { 
  // reads the data from the Serial Port up to the character '.' and puts it into the String variable "data".
  data = myPort.readStringUntil('\n');

  // if you got any bytes other than the linefeed:
  if (data != null) {
    data = trim(data);
    // split the string at "/"
    String items[] = split(data, '/');
    if (items.length > 1) {
      
      //--- Roll,Pitch in degrees
      roll = float(items[4]);
      pitch = float(items[5]);
      yaw = float(items[6]);
    }
  }
}
