#include "include/TapeSorter.h"
#include "include/FileTapeDevice.h"
#include "include/config.h"
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[]) {
  try {
    if (argc != 3) {
      std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << '\n';
      return 1;
    }
    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    std::string tempDir = "tmp";

    Config config("config.txt");

    size_t memoryLimit = config.getInt("memory_limit", 1048576);
    TapeDelays delays{
      static_cast<size_t>(config.getInt("read_delay_ms", 10)),
      static_cast<size_t>(config.getInt("write_delay_ms", 20)),
      static_cast<size_t>(config.getInt("move_delay_ms", 5)),
      static_cast<size_t>(config.getInt("rewind_delay_ms_per_position", 1))
  };

    std::filesystem::remove(outputFile);
    std::filesystem::create_directories(tempDir);

    // Создаём устройства
    FileTapeDevice inputTape(inputFile, delays, false);
    FileTapeDevice outputTape(outputFile, delays, true);

    // Создаём сортировщик
    TapeSorter sorter(memoryLimit, tempDir, delays);

    // Запуск сортировки
    sorter.sort(inputTape, outputTape);

    std::cout << "Sorting completed." << std::endl;

  } catch (const std::exception& e) {
    std::cerr << "Error occurred: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}
