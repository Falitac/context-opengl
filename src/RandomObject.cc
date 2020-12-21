#include "../include/RandomObject.hpp"

void RandomObject::load() {
  std::size_t vertexCount = 3 * 40;

  for(std::size_t i = 0; i < vertexCount; i++) {
    GLfloat x, y, z;
    x = Random::get(-6.0f, 6.0f);
    y = Random::get(-6.0f, 6.0f);
    z = Random::get(-6.0f, 6.0f);
    vertices.emplace_back(x, y, z);
  }

  for(std::size_t i = 0; i < vertices.size(); i++) {
    GLfloat r, g, b;
    r = Random::get(0.0f, 1.0f);
    g = Random::get(0.0f, 1.0f);
    b = Random::get(0.0f, 1.0f);
    colors.emplace_back(r, g, b);
  }

  for(std::size_t i = 0; i < vertices.size(); i++) {
    GLuint v1, v2, v3;
    v1 = Random::get<GLuint>(0, vertexCount - 1);
    v1 = Random::get<GLuint>(0, vertexCount - 1);
    v1 = Random::get<GLuint>(0, vertexCount - 1);
    indices.emplace_back(v1);
    indices.emplace_back(v2);
    indices.emplace_back(v3);
  }
  applyData();
}
