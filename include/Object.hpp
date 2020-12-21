#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>
#include <iostream>

class Object {
public:
  virtual void load() = 0;
  virtual void draw(glm::mat4 projectionView, GLuint shaderID) = 0;

  Object() = default;

  ~Object() = default;

protected:
  GLuint vertexBuffer;
  GLuint colorBuffer;
  GLuint indicesBuffer;

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> colors;
  std::vector<std::uint32_t> indices;
};

