#version 450 compatibility

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;

void main() {
  mat4 MVP = projection * view * model;
  float radius = 1;
  gl_Position = vec4(position, 1.0);
  gl_Position.x = gl_Position.x * radius * sin(time);
  gl_Position.z = gl_Position.z * radius * cos(time);
  fragmentColor = vec3(0.0, 0.5, 0.5);
}