#include "../include/Cube.hpp"


void Cube::draw(glm::mat4 projectionView, GLuint shaderID) {
  glUseProgram(shaderID);
  GLuint mvpID = glGetUniformLocation(shaderID, "MVP");
  glm::mat4 model = glm::mat4(1.0f);
  model = projectionView * model;

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

  glUniformMatrix4fv(mvpID, 1, GL_FALSE, &model[0][0]);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}

void Cube::load() {
  glGenBuffers(1, &vertexBuffer);
  glGenBuffers(1, &colorBuffer);
  glGenBuffers(1, &indicesBuffer);

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

  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(
    GL_ARRAY_BUFFER,
    vertices.size()*sizeof(glm::vec3),
    &vertices[0],
    GL_STATIC_DRAW
  );
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(
    GL_ARRAY_BUFFER,
    colors.size()*sizeof(glm::vec3),
    &colors[0],
    GL_STATIC_DRAW
  );
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    indices.size()*sizeof(GLuint),
    indices.data(),
    GL_STATIC_DRAW
  );
}