#pragma once

#include "Object.hpp"
#include "Random.hpp"

class Cube : public Object {
public:
  virtual void draw(glm::mat4 projectionView, GLuint shaderID);
  virtual void load();

  Cube() {
  }
  ~Cube() {
  }

  

};