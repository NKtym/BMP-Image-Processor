#pragma once
#include <cstdint>
#include <string>
#include <vector>

class BMP {
public:
  struct Pixel {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
  };
  BMP(const std::string &filename);
  void display() const;
  void drawLine(int x1, int y1, int x2, int y2);
  void save(const std::string &filename) const;
  int getWidth() const;
  int getHeight() const;
  ~BMP();

private:
  int width;
  int height;
  std::vector<Pixel> pixels;
  void readBMP(const std::string &filename);
  bool isBlack(const Pixel &pixel) const;
  bool isWhite(const Pixel &pixel) const;
  void setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue,
                uint8_t alpha);
};