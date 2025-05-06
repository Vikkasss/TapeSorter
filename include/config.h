#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <unordered_map>

class Config {
public:
  Config(const std::string& filename);
  int getInt(const std::string& key, int defaultValue) const;

private:
  std::unordered_map<std::string, int> params;
};

#endif // CONFIG_H