#include "../include/Generators.hpp"


std::vector<GLfloat> genCube() {
  std::vector<GLfloat> result;
  for(std::size_t z = 0; z < 2; z++) {
    for(std::size_t y = 0; y < 2; y++) {
      for(std::size_t x = 0; x < 2; x++) {
        using std::pow;
        result.emplace_back(1.0f * pow(-1, x));
        result.emplace_back(1.0f * pow(-1, y));
        result.emplace_back(1.0f * pow(-1, z));
      }
    }
  }
  return result;
}


std::vector<GLfloat> genSphere() {
  std::vector<GLfloat> result;
  return result;
}