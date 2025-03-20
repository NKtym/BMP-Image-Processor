#include "BMP.hpp"
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

BMP::BMP(const std::string &filename) { readBMP(filename); }

void BMP::display() const {
  for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
      const Pixel &pixel = pixels[y * width + x];
      if (isBlack(pixel)) {
        std::cout << "#";
      } else if (isWhite(pixel)) {
        std::cout << ".";
      } else {
        throw std::runtime_error("Unsupported pixel color");
      }
    }
    std::cout << '\n';
  }
}

void BMP::drawLine(int x1, int y1, int x2, int y2) {
  int dx = std::abs(x2 - x1);
  int dy = std::abs(y2 - y1);
  int sx, sy;
  if (x1 < x2)
    sx = 1;
  else
    sx = -1;
  if (y1 < y2)
    sy = 1;
  else
    sy = -1;
  int err = dx - dy;

  while (true) {
    setPixel(x1, y1, 0, 0, 0, 255);
    if (x1 == x2 && y1 == y2)
      break;
    int e2 = 2 * err;
    if (e2 > -dy) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dx) {
      err += dx;
      y1 += sy;
    }
  }
}

void BMP::save(const std::string &filename) const {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open())
    throw std::runtime_error("Failed to open file to save BMP");
  uint16_t bfType = 0x4D42;
  file.write(reinterpret_cast<const char *>(&bfType), sizeof(bfType));
  uint32_t bfSize =
      14 + 40 + static_cast<uint32_t>(pixels.size() * sizeof(Pixel));
  file.write(reinterpret_cast<const char *>(&bfSize), sizeof(bfSize));
  uint16_t bfReserved = 0;
  file.write(reinterpret_cast<const char *>(&bfReserved), sizeof(bfReserved));
  file.write(reinterpret_cast<const char *>(&bfReserved), sizeof(bfReserved));
  uint32_t bfOffBits = 14 + 40;
  file.write(reinterpret_cast<const char *>(&bfOffBits), sizeof(bfOffBits));
  uint32_t biSize = 40;
  file.write(reinterpret_cast<const char *>(&biSize), sizeof(biSize));
  file.write(reinterpret_cast<const char *>(&width), sizeof(width));
  file.write(reinterpret_cast<const char *>(&height), sizeof(height));
  uint16_t biPlanes = 1;
  file.write(reinterpret_cast<const char *>(&biPlanes), sizeof(biPlanes));
  uint16_t biBitCount = 32;
  file.write(reinterpret_cast<const char *>(&biBitCount), sizeof(biBitCount));
  uint32_t biCompression = 0;
  file.write(reinterpret_cast<const char *>(&biCompression),
             sizeof(biCompression));
  uint32_t biSizeImage = static_cast<uint32_t>(pixels.size() * sizeof(Pixel));
  file.write(reinterpret_cast<const char *>(&biSizeImage), sizeof(biSizeImage));
  int32_t biXPelsPerMeter = 0;
  int32_t biYPelsPerMeter = 0;
  file.write(reinterpret_cast<const char *>(&biXPelsPerMeter),
             sizeof(biXPelsPerMeter));
  file.write(reinterpret_cast<const char *>(&biYPelsPerMeter),
             sizeof(biYPelsPerMeter));
  uint32_t biClrUsed = 0;
  uint32_t biClrImportant = 0;
  file.write(reinterpret_cast<const char *>(&biClrUsed), sizeof(biClrUsed));
  file.write(reinterpret_cast<const char *>(&biClrImportant),
             sizeof(biClrImportant));
  file.write(reinterpret_cast<const char *>(pixels.data()),
             pixels.size() * sizeof(Pixel));
  if (!file)
    throw std::runtime_error("Writing BMP file");
}

int BMP::getWidth() const { return width; }
int BMP::getHeight() const { return height; }

void BMP::readBMP(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open())
    throw std::runtime_error("Failed to open file to read BMP");
  char bfType[2];
  file.read(bfType, 2);
  if (bfType[0] != 'B' || bfType[1] != 'M') {
    throw std::runtime_error("The file is not BMP");
  }
  uint32_t bfSize;
  file.read(reinterpret_cast<char *>(&bfSize), sizeof(bfSize));
  uint16_t bfReserved1, bfReserved2;
  file.read(reinterpret_cast<char *>(&bfReserved1), sizeof(bfReserved1));
  file.read(reinterpret_cast<char *>(&bfReserved2), sizeof(bfReserved2));
  uint32_t bfOffBits;
  file.read(reinterpret_cast<char *>(&bfOffBits), sizeof(bfOffBits));
  uint32_t biSize;
  file.read(reinterpret_cast<char *>(&biSize), sizeof(biSize));
  if (biSize != 40) {
    throw std::runtime_error("Unsupported DIB format(40 bit)");
  }
  uint16_t biPlanes, biBitCount;
  file.read(reinterpret_cast<char *>(&width), sizeof(width));
  file.read(reinterpret_cast<char *>(&height), sizeof(height));
  file.read(reinterpret_cast<char *>(&biPlanes), sizeof(biPlanes));
  file.read(reinterpret_cast<char *>(&biBitCount), sizeof(biBitCount));
  if (biBitCount != 32)
    throw std::runtime_error("The BPM file is not 32 bit");
  uint32_t biCompression;
  file.read(reinterpret_cast<char *>(&biCompression), sizeof(biCompression));
  if (biCompression != 0)
    throw std::runtime_error("Compression is not supported");
  uint32_t biSizeImage;
  file.read(reinterpret_cast<char *>(&biSizeImage), sizeof(biSizeImage));
  int32_t biXPelsPerMeter, biYPelsPerMeter;
  file.read(reinterpret_cast<char *>(&biXPelsPerMeter),
            sizeof(biXPelsPerMeter));
  file.read(reinterpret_cast<char *>(&biYPelsPerMeter),
            sizeof(biYPelsPerMeter));
  uint32_t biClrUsed, biClrImportant;
  file.read(reinterpret_cast<char *>(&biClrUsed), sizeof(biClrUsed));
  file.read(reinterpret_cast<char *>(&biClrImportant), sizeof(biClrImportant));
  pixels.resize(width * height);
  file.read(reinterpret_cast<char *>(pixels.data()),
            pixels.size() * sizeof(Pixel));
  if (!file)
    throw std::runtime_error("Reading pixel data");
}
bool BMP::isBlack(const Pixel &pixel) const {
  return pixel.red == 0 && pixel.green == 0 && pixel.blue == 0;
}
bool BMP::isWhite(const Pixel &pixel) const {
  return pixel.red == 255 && pixel.green == 255 && pixel.blue == 255;
}
void BMP::setPixel(int x, int y, uint8_t red, uint8_t green, uint8_t blue,
                   uint8_t alpha) {
  if (x < 0 || x >= width || y < 0 || y >= height)
    return;
  pixels[y * width + x] = {blue, green, red, alpha};
}

BMP::~BMP() {}