#ifndef SDWriter_h
#define SDWriter_h

#include <SPI.h>
#include "SdFat.h"

#define FILE_BASE_NAME "Data"
#define CHIPSELECT SS

using namespace sdfat;

class SDWriter {
public:
  SDWriter();
  void init();
  void writeHeader();
  void initFile();
  void writeData(unsigned long timestamp, int16_t ax, int16_t ay, int16_t az);
  void closeFile();

private:
  SdFat sd;
  SdFile file;
};

#endif