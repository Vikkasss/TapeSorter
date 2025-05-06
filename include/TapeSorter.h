

#ifndef TAPESORTER_H
#define TAPESORTER_H

#include "TapeDevice.h"
#include <memory>
#include <vector>

#include "FileTapeDevice.h"


class TapeSorter {
public:
  TapeSorter(size_t maxMemoryBytes, const std::string& tempDir, const TapeDelays& delays);

  void sort(TapeDevice& input, TapeDevice& output);
private:
  size_t maxMemoryElem;
  std::string tempDir;
  TapeDelays delays;

  std::vector<std::unique_ptr<TapeDevice>> splitAndSort(TapeDevice& input);
  void merge(std::vector<std::unique_ptr<TapeDevice>>& tapes, TapeDevice& output);

};


#endif //TAPESORTER_H
