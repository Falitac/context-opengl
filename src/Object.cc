#include "../include/Object.hpp"

void Object::applyData() {
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

void Object::draw(GLuint shaderID) {
  glUseProgram(shaderID);
  GLint modelID = glGetUniformLocation(shaderID, "model");
  glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}