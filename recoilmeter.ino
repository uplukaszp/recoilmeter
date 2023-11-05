#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;
uint8_t redbtn = D5, yellowbtn = D6, greenbtn = D7;
int16_t ax, ay, az;
int16_t avgx =0, avgy =0, avgz = 0;

void setup() {
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(38400);
    initializeInput();
    initializeAccel();
}

void loop() {
    if(digitalRead(redbtn) == 0) {
      calibrate();
    }
    if (digitalRead(greenbtn) == 0) {
      measure();
    }
}

void initializeAccel() {
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  Serial.println("Testing device connections...");
  if(accelgyro.testConnection()) {
    Serial.println("MPU6050 connection successful");
    calibrate();
  }
  else {
    Serial.println("MPU6050 connection failed");
  }
}

void initializeInput() {
  pinMode(redbtn, INPUT_PULLUP);
  pinMode(yellowbtn, INPUT_PULLUP);
  pinMode(greenbtn, INPUT_PULLUP);
}

void calibrate() {
  Serial.print("\nCalibrating...");
  long x,y,z;

  for(uint8_t i=0;i<255;i++) {
    accelgyro.getAcceleration(&ax, &ay, &az);
    x=(x*i + ax)/(i+1);
    y=(y*i + ay)/(i+1);
    z=(z*i + az)/(i+1);
  }

  avgx = x;
  avgy = y;
  avgz = z;

  Serial.print("avgx:"); Serial.print(avgx);
  Serial.print(",avgy:"); Serial.print(avgy);
  Serial.print(",avgz:"); Serial.print(avgz);
  Serial.println("");
  
  Serial.println("\nCalibration complete!");
}

void measure() {
    accelgyro.getAcceleration(&ax, &ay, &az);
    Serial.print("ax:"); Serial.print(ax-avgx);
    Serial.print(",ay:"); Serial.print(ay-avgy);
    Serial.print(",az:"); Serial.print(az-avgz);
    Serial.println("");
}
