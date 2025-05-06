
#include "../include/config.h"
#include <fstream>
#include <sstream>

Config::Config(const std::string& filename) {
  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line)) {
    size_t delimiter = line.find('=');
    if (delimiter != std::string::npos) {
      std::string key = line.substr(0, delimiter);
      int value = std::stoi(line.substr(delimiter + 1));
      params[key] = value;
    }
  }
}

int Config::getInt(const std::string& key, int defaultValue) const {
  auto it = params.find(key);
  return (it != params.end()) ? it->second : defaultValue;
}