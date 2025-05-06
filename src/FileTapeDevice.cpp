#include "../include/FileTapeDevice.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <stdexcept>

FileTapeDevice::FileTapeDevice(const std::string &filename, const TapeDelays &delays, bool truncate) : filename(filename), delays(delays), position(0) {

  std::ios::openmode mode = std::ios::in | std::ios::out | std::ios::binary;
  if (truncate) {
    mode |= std::ios::trunc;
  }
  file.open(filename, mode);

  if (!file.is_open()) {
    std::ofstream tmp(filename, std::ios::binary);
    tmp.close();
    file.open(filename, mode);
  }

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }
  updateSize();
}

FileTapeDevice::~FileTapeDevice() {
  if(file.is_open()) {
    file.close();
  }
}

int FileTapeDevice::read() {
  if (position >= size) {
    throw std::out_of_range("Read position is out of tape bounds");
  }
  applyDelay(delays.readDelayMs);
  int value;
  file.seekg(position * sizeof(int));
  file.read(reinterpret_cast<char*>(&value), sizeof(int));

  if (!file) {
    throw std::runtime_error("Failed to read from tape");
  }
  return value;
}

void FileTapeDevice::write(int value) {
  applyDelay(delays.writeDelayMs);
  file.seekp(position * sizeof(int));
  file.write(reinterpret_cast<char*>(&value), sizeof(int));
  if (position >= size) {
    size = position + 1;
  }
  if (file.fail()) {
    throw std::runtime_error("Failed to write to tape");
  }
}

void FileTapeDevice::rewind() {
  applyDelay(delays.rewindDelayPerPosMs * position);
  position = 0;
}

size_t FileTapeDevice::getSize() const {
  return size;
}

std::string FileTapeDevice::getFileName() const {
  return filename;
}

void FileTapeDevice::moveBackward() {
  if (position > 0) {
    applyDelay(delays.moveDelayMs);
    --position;
  }
}

void FileTapeDevice::moveForward() {
  if (position < size) {
    applyDelay(delays.moveDelayMs);
    ++position;
  }
}

bool FileTapeDevice::isAtEnd() const {
  return position >= size;
}

void FileTapeDevice::updateSize() {
  if(file.seekg(0, std::ios::end)) {
    size = file.tellg() / sizeof(int);
    file.seekg(position * sizeof(int));
  } else {
    size = 0;
  }
}

void FileTapeDevice::applyDelay(size_t delayMs) const {
  if(delayMs > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
  }
}






