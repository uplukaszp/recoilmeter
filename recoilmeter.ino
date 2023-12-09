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
uint16_t axarr[5000];
uint16_t ayarr[5000];
uint16_t azarr[5000];
uint16_t timearr[5000];

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
  Serial.println(F("Initializing I2C devices..."));
  accelgyro.initialize();
  calibrate();
}

void initializeInput() {
  pinMode(yellowbtn, INPUT_PULLUP);
  pinMode(greenbtn, INPUT_PULLUP);
}

void calibrate() {
  Serial.println(F("Calibrating..."));
  long x, y, z;
  testAccelConnection();
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

  Serial.println(F("Calibration complete!"));
  tone(D0, 1000);
  delay(500);
  noTone(D0);
  delay(500);
}

void initializeWriter() {
  writer.init();
}

void measure() {
  Serial.println(F("Measuring..."));
  unsigned long t0 = micros();
  tone(D0, 1000);
  delay(500);
  noTone(D0);
  delay(500);
  testAccelConnection();

  for (uint16_t i = 0; i < 5000; i++) {
    accelgyro.getAcceleration(&ax, &ay, &az);
    axarr[i] = ax;
    ayarr[i] = ay;
    azarr[i] = az;
    timearr[i] = micros() - t0;
  }

  Serial.print("saving...");
  for (uint16_t i = 0; i < 5000; i++) {
    writer.writeData(timearr[i], axarr[i] - avgx, ayarr[i] - avgy, azarr[i] - avgz);
  }
  Serial.print(F("Measure data writting time: "));
  Serial.println(micros() - t0);
}

void testAccelConnection() {
  Serial.println("Testing device connections...");
  if (accelgyro.testConnection()) {
    Serial.println("MPU6050 connection successful");
    tone(D0, 1000);
    delay(500);
    noTone(D0);
    delay(500);

  } else {
    tone(D0, 500);
    delay(100);
    noTone(D0);
    delay(100);
    tone(D0, 500);
    delay(100);
    noTone(D0);
    Serial.println("MPU6050 connection failed");
    SysCall::halt();
  }
}
