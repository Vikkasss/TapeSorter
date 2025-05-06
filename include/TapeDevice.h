

#ifndef TAPEDEVICE_H
#define TAPEDEVICE_H
#include <cstddef>
#include <string>


class TapeDevice {
public:
  virtual int read() = 0;
  virtual void write(int value) = 0;
  virtual void rewind() = 0;
  virtual void moveForward() = 0;
  virtual void moveBackward() = 0;
  virtual bool isAtEnd() const = 0;
  virtual size_t getSize() const = 0;
  virtual std::string getFileName() const = 0;
  virtual ~TapeDevice() = default;

};



#endif //TAPEDEVICE_H
