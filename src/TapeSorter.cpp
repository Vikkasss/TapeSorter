

#include "../include/TapeSorter.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <queue>
namespace fs = std::filesystem;
#include "../include/FileTapeDevice.h"

TapeSorter::TapeSorter(size_t maxMemoryBytes,
                      const std::string &tempDir,
                      const TapeDelays& delays)
    : delays(delays),
      maxMemoryElem(maxMemoryBytes / sizeof(int)),
      tempDir(tempDir)
{
  if (maxMemoryBytes < sizeof(int)) {
    throw std::invalid_argument("Memory limit is less than the size of one element");
  }
  if (!fs::exists(tempDir)) {
    fs::create_directories(tempDir);
  }
}

void TapeSorter::sort(TapeDevice &input, TapeDevice &output) {
  auto tempTapes = splitAndSort(input);
  merge(tempTapes, output);

  tempTapes.clear();
  for (const auto& entry : fs::directory_iterator(tempDir)) {
    fs::remove(entry.path());
  }
}


void TapeSorter::merge(std::vector<std::unique_ptr<TapeDevice>> &tapes, TapeDevice &output) {
  using QueueElement = std::pair<int, size_t>; // (value, tapeIndex)
  std::priority_queue<QueueElement, std::vector<QueueElement>, std::greater<>> pq;

  for (size_t i = 0; i < tapes.size(); ++i) {
    tapes[i]->rewind();
    if (!tapes[i]->isAtEnd()) {
      int val = tapes[i]->read();
      pq.emplace(val, i);
    }
  }
  while (!pq.empty()) {
    auto [value, tapeIdx] = pq.top();
    pq.pop();

    output.write(value);
    output.moveForward();

    tapes[tapeIdx]->moveForward();
    if (!tapes[tapeIdx]->isAtEnd()) {
      int nextVal = tapes[tapeIdx]->read();
      pq.emplace(nextVal, tapeIdx);
    }
  }

  output.rewind();
}


std::vector<std::unique_ptr<TapeDevice>> TapeSorter::splitAndSort(TapeDevice &input) {
  std::vector<std::unique_ptr<TapeDevice>> tempTapes;
  std::vector<int> buffer;
  buffer.reserve(maxMemoryElem);

  input.rewind();
  while (true) {
    buffer.clear();

    // Чтение данных в буфер
    while (buffer.size() < maxMemoryElem && !input.isAtEnd()) {
      buffer.push_back(input.read());
      input.moveForward();
    }

    // Если буфер пуст (на ленте нет больше данных), выходим из цикла
    if (buffer.empty()) break;

    // Сортировка
    std::sort(buffer.begin(), buffer.end());

    // Создание временной ленты
    fs::path tempPath = fs::path(tempDir) / ("temp_" + std::to_string(tempTapes.size()) + ".bin");
    auto tempTape = std::make_unique<FileTapeDevice>(
        tempPath.string(),
        this->delays,
        true
    );

    tempTape->rewind();
    for (int val : buffer) {
      tempTape->write(val);
      tempTape->moveForward();
    }

    tempTape->rewind();
    tempTapes.push_back(std::move(tempTape));
  }

  return tempTapes;
}




