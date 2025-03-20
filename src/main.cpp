#include "BMP.hpp"
#include <iostream>

int main() {
  std::string filename;
  std::cout << ">> Enter input BMP file name: ";
  std::cin >> filename;
  try {
    BMP image(filename);
    image.display();
    image.drawLine(0, 0, image.getWidth() - 1, image.getHeight() - 1);
    image.drawLine(0, image.getHeight() - 1, image.getWidth() - 1, 0);
    std::cout << '\n';
    std::cout << "Modified Image:\n";
    image.display();
    std::cout << '\n';
    std::cout << ">> Enter output BMP file name: ";
    std::cin >> filename;
    image.save(filename);
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}