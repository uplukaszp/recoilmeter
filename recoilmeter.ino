#include "I2Cdev.h"
#include "MPU6050.h"
#include "SDWriter.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 accelgyro;
SDWriter writer;

uint8_t yellowbtn = D4, greenbtn = D3;
int16_t ax, ay, az;
int16_t avgx = 0, avgy = 0, avgz = 0;

void setup() {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

  Serial.begin(38400);
  initializeInput();
  initializeAccel();
  initializeWriter();
}

void loop() {
  if (digitalRead(yellowbtn) == 0) {
    calibrate();
  }
  if (digitalRead(greenbtn) == 0) {
    writer.initFile();
    writer.writeHeader();
    measure();
    writer.closeFile();
  }
}

void initializeAccel() {
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  Serial.println("Testing device connections...");
  if (accelgyro.testConnection()) {
    Serial.println("MPU6050 connection successful");
    calibrate();
  } else {
    Serial.println("MPU6050 connection failed");
  }
}

void initializeInput() {
  pinMode(yellowbtn, INPUT_PULLUP);
  pinMode(greenbtn, INPUT_PULLUP);
}

void calibrate() {
  Serial.print("\nCalibrating...");
  long x, y, z;

  for (uint8_t i = 0; i < 255; i++) {
    accelgyro.getAcceleration(&ax, &ay, &az);
    x = (x * i + ax) / (i + 1);
    y = (y * i + ay) / (i + 1);
    z = (z * i + az) / (i + 1);
  }

  avgx = x;
  avgy = y;
  avgz = z;

  Serial.print("avgx:");
  Serial.print(avgx);
  Serial.print(",avgy:");
  Serial.print(avgy);
  Serial.print(",avgz:");
  Serial.print(avgz);
  Serial.println("");

  Serial.println("\nCalibration complete!");
}

void initializeWriter() {
  writer.init();
}

void measure() {
  unsigned long t0 = micros();
  uint16_t axarr[5000];
  uint16_t ayarr[5000];
  uint16_t azarr[5000];
  uint16_t timearr[5000];
  for (uint16_t i = 0; i < 5000; i++) {
    accelgyro.getAcceleration(&ax, &ay, &az);
    axarr[i] = ax;
    ayarr[i] = ay;
    azarr[i] = az;
    timearr[i] = micros() - t0;
    writer.writeData(micros() - t0, ax - avgx, ay - avgy, az - avgz);
  }

  for (uint16_t i = 0; i < 5000; i++) {
    writer.writeData(timearr[i], axarr[i] - avgx, ayarr[i] - avgy, azarr[i] - avgz);
  }
  Serial.println("measure time:");
  Serial.print(micros() - t0);
}
