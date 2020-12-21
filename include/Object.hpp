#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <cstdint>
#include <iostream>

class Object {
public:
  virtual void load() = 0;
  virtual void draw(GLuint shaderID);

  Object():
  model(1.f) {
    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &colorBuffer);
    glGenBuffers(1, &indicesBuffer);
  }

  ~Object() {
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &colorBuffer);
    glDeleteBuffers(1, &indicesBuffer);
  }

protected:
  GLuint vertexBuffer;
  GLuint colorBuffer;
  GLuint indicesBuffer;

  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> colors;
  std::vector<std::uint32_t> indices;

  glm::mat4 model;

  void applyData();
};

