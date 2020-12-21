#pragma once

#include "Object.hpp"
#include "Random.hpp"

class RandomObject: public Object {
public:
  virtual void load();

  RandomObject() = default;

};