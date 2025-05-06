#include <gtest/gtest.h>
#include "../include/TapeSorter.h"
#include "../include/FileTapeDevice.h"
#include <filesystem>
namespace fs = std::filesystem;

class TapeSorterFullTest : public ::testing::Test {
protected:
  std::string inputFile = "test_input.bin";
  std::string outputFile = "test_output.bin";
  std::string tempDir = "test_tmp";
  TapeDelays noDelays{0, 0, 0, 0};

  void SetUp() override {
    std::filesystem::remove(inputFile);
    std::filesystem::remove(outputFile);
    std::filesystem::remove_all(tempDir);
    std::filesystem::create_directory(tempDir);
  }

  void TearDown() override {
    std::filesystem::remove(inputFile);
    std::filesystem::remove(outputFile);
    std::filesystem::remove_all(tempDir);
  }
};

TEST_F(TapeSorterFullTest, SortsCorrectly) {
  FileTapeDevice input(inputFile, noDelays, true);
  input.write(3); input.moveForward();
  input.write(1); input.moveForward();
  input.write(4); input.moveForward();
  input.write(5); input.moveForward();
  input.write(2);

  FileTapeDevice outTape(outputFile, noDelays, true);
  TapeSorter sorter(3 * sizeof(int), tempDir, noDelays);
  sorter.sort(input, outTape);

  std::vector<int> expected = {1, 2, 3, 4, 5};
  std::vector<int> result;

  outTape.rewind();
  while (!outTape.isAtEnd()) {
    result.push_back(outTape.read());
    outTape.moveForward();
  }
  EXPECT_EQ(result, expected);
}

TEST_F(TapeSorterFullTest, HandlesEmptyInput) {
  FileTapeDevice input(inputFile, noDelays, true);
  input.rewind();

  FileTapeDevice inTape(inputFile, noDelays, false);
  FileTapeDevice outTape(outputFile, noDelays, true);
  TapeSorter sorter(4 * sizeof(int), tempDir, noDelays);
  sorter.sort(inTape, outTape);

  outTape.rewind();
  EXPECT_TRUE(outTape.isAtEnd());
}

TEST_F(TapeSorterFullTest, ThrowsOnTooSmallMemory) {
  EXPECT_THROW({
    TapeSorter sorter(1, tempDir, noDelays);
  }, std::invalid_argument);
}

TEST_F(TapeSorterFullTest, CreatesTempDirectoryIfMissing) {
  std::filesystem::remove_all(tempDir);
  EXPECT_FALSE(std::filesystem::exists(tempDir));

  TapeSorter sorter(4 * sizeof(int), tempDir, noDelays);

  EXPECT_TRUE(std::filesystem::exists(tempDir));
}

TEST_F(TapeSorterFullTest, SortsSingleElementCorrectly) {
  FileTapeDevice input(inputFile, noDelays, true);
  input.write(32);
  input.rewind();

  FileTapeDevice outTape(outputFile, noDelays, true);
  TapeSorter sorter(1 * sizeof(int), tempDir, noDelays);
  sorter.sort(input, outTape);

  std::vector<int> expected = {32};
  std::vector<int> result;

  while (!outTape.isAtEnd()) {
    result.push_back(outTape.read());
    outTape.moveForward();
  }

  EXPECT_EQ(result, expected);
}
