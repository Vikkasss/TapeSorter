#include <gtest/gtest.h>
#include "../include/FileTapeDevice.h"
#include <filesystem>

class FileTapeDeviceTest : public ::testing::Test {
protected:
  std::string testFile = "test_device.bin";
  TapeDelays noDelays{0,0,0,0};

  void TearDown() override {
    std::filesystem::remove(testFile);
  }
};

TEST_F(FileTapeDeviceTest, ConstructorCreatesFileIfNotExist) {
  ASSERT_FALSE(std::filesystem::exists(testFile));

  FileTapeDevice device(testFile, noDelays, false);
  EXPECT_TRUE(std::filesystem::exists(testFile));
  EXPECT_EQ(device.getSize(), 0);
}

TEST_F(FileTapeDeviceTest, ConstructorTruncatesFileWhenRequested) {
  {
    std::ofstream out(testFile, std::ios::binary);
    int32_t dummy = 42;
    out.write(reinterpret_cast<char*>(&dummy), sizeof(dummy));
  }
  EXPECT_GT(std::filesystem::file_size(testFile), 0);
  FileTapeDevice device(testFile, noDelays, true);
  EXPECT_EQ(std::filesystem::file_size(testFile), 0);
}

TEST_F(FileTapeDeviceTest, CanWriteAndReadBack) {
  FileTapeDevice device(testFile, noDelays, true);
  device.write(42);
  device.rewind();
  EXPECT_EQ(device.read(), 42);
}

TEST_F(FileTapeDeviceTest, CheckSizeIncreasesAfterWrite) {
  FileTapeDevice device(testFile, noDelays, true);
  EXPECT_EQ(device.getSize(), 0);
  device.write(42);
  EXPECT_EQ(device.getSize(), 1);
}

TEST_F(FileTapeDeviceTest, ReadOutOfBoundsThrows) {
  FileTapeDevice device(testFile, noDelays, true);
  EXPECT_THROW(device.read(), std::out_of_range);
}

TEST_F(FileTapeDeviceTest, RewindResetsPosition) {
  FileTapeDevice device(testFile, noDelays, true);
  device.write(1);
  device.moveForward();
  device.write(2);
  device.rewind();
  EXPECT_EQ(device.read(), 1);
}


TEST_F(FileTapeDeviceTest, IsAtEndReturnsTrueWhenPositionAtEnd) {
  FileTapeDevice device(testFile, noDelays, true);
  device.write(5);        // позиция 0
  device.moveForward();        // позиция 1
  EXPECT_TRUE(device.isAtEnd());
}
TEST_F(FileTapeDeviceTest, GetFileNameReturnsCorrectName) {
  FileTapeDevice device(testFile, noDelays, true);
  EXPECT_EQ(device.getFileName(), testFile);
}

TEST_F(FileTapeDeviceTest, MoveForwardAndBackward) {
  FileTapeDevice device(testFile, noDelays, true);
  device.write(10); device.moveForward();
  device.write(20); device.moveForward();
  device.write(30);

  device.rewind();        // позиция 0
  device.moveForward();   // позиция 1
  EXPECT_EQ(device.read(), 20);

  device.moveBackward();  // позиция 0
  EXPECT_EQ(device.read(), 10);
}

