#include "../include/Cube.hpp"

#include <glm/gtc/matrix_transform.hpp>


void Cube::load() {

  vertices = {
    { 1.0f,  1.0f,  1.0f},
    {-1.0f,  1.0f,  1.0f},
    { 1.0f, -1.0f,  1.0f},
    {-1.0f, -1.0f,  1.0f},
    { 1.0f,  1.0f, -1.0f},
    {-1.0f,  1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
  };

  for(std::size_t i = 0; i < vertices.size(); i++) {
    GLfloat r, g, b;
    r = Random::get(0.0f, 1.0f);
    g = Random::get(0.0f, 1.0f);
    b = Random::get(0.0f, 1.0f);
    colors.emplace_back(r, g, b);
  }

  indices = {
    7, 3, 1,
    7, 1, 5,

    3, 2, 0,
    3, 0, 1,

    2, 6, 4,
    2, 4, 0,

    6, 7, 5,
    6, 5, 4, 

    7, 6, 2,
    0, 5, 1,

    3, 7, 2,
    0, 4, 5,
  };

  float radius = 5.f;
  float posX = Random::get(-radius, radius);

  float rangeY = glm::sqrt(radius*radius - posX*posX);
  float posY = Random::get(-rangeY, rangeY);

  float rangeZ = glm::sqrt(radius*radius - posY*posY);
  float posZ = Random::get(-rangeZ, rangeZ);
  model = glm::translate(model, {posX, posY, posZ});
  applyData();
}
