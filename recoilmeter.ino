#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 accelgyro;

int16_t ax, ay, az;

void setup() {
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(38400);
    initializeAccel();
}

void loop() {
    // accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    // Serial.print("ax:"); Serial.print(ax);
    // Serial.print(",ay:"); Serial.print(ay);
    // Serial.print(",az:"); Serial.print(az);
    // Serial.println("");
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

void calibrate() {
  Serial.println("Calibration procedure");
  Serial.println("Offsets before calibration:");
  accelgyro.PrintActiveOffsets();

  Serial.print("\nCalibrating...");
  accelgyro.CalibrateAccel(5);
  
  Serial.println("\nOffsets after calibration");
  accelgyro.PrintActiveOffsets();
  
  Serial.println("\nCalibration complete!");
}
