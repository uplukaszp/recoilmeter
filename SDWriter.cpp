#include "SDWriter.h"

SDWriter::SDWriter() {
}

void SDWriter::init() {
  if (!sd.begin(CHIPSELECT, SD_SCK_MHZ(50))) {
    sd.initErrorHalt();
  }
}

void SDWriter::initFile() {
  const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
  char fileName[13] = FILE_BASE_NAME "00.csv";
  if (BASE_NAME_SIZE > 6) {
    sd.errorHalt("FILE_BASE_NAME too long");
  }
  while (sd.exists(fileName)) {
    if (fileName[BASE_NAME_SIZE + 1] != '9') {
      fileName[BASE_NAME_SIZE + 1]++;
    } else if (fileName[BASE_NAME_SIZE] != '9') {
      fileName[BASE_NAME_SIZE + 1] = '0';
      fileName[BASE_NAME_SIZE]++;
    } else {
      sd.errorHalt("Can't create file name");
    }
  }
  if (!file.open(fileName, O_WRONLY | O_CREAT | O_EXCL)) {
    sd.errorHalt("file.open");
  }
  Serial.print(F("Logging to: "));
  Serial.println(fileName);
}

void SDWriter::writeHeader() {
  file.println(F("micros,x,y,z"));
}

void SDWriter::writeData(unsigned long timestamp, int16_t ax, int16_t ay, int16_t az) {
  file.print(timestamp);
  file.print(',');
  file.print(ax);
  file.print(',');
  file.print(ay);
  file.print(',');
  file.println(az);
}

void SDWriter::closeFile() {
  if (!file.sync() || file.getWriteError()) {
    sd.errorHalt("write error");
  }
  file.close();
  Serial.println(F("Save done"));
}