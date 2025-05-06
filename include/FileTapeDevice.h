#ifndef FILETAPEDEVICE_H
#define FILETAPEDEVICE_H

#include "TapeDevice.h"
#include <fstream>
#include <string>

struct TapeDelays {
  size_t readDelayMs;
  size_t writeDelayMs;
  size_t moveDelayMs;
  size_t rewindDelayPerPosMs;
};


class FileTapeDevice : public TapeDevice{
public:
  FileTapeDevice(const std::string& filename, const TapeDelays& delays, bool truncate);
  ~FileTapeDevice();

  int read() override;
  void write(int value) override;
  void rewind() override;
  void moveForward() override;
  void moveBackward() override;
  bool isAtEnd() const override;
  size_t getSize() const override;
  std::string getFileName() const override;

private:
  std::fstream file;
  size_t position;
  size_t size;
  TapeDelays delays;
  std::string filename;

  void updateSize();
  void applyDelay(size_t delayMs) const;
};



#endif //FILETAPEDEVICE_H
