#version 450 compatibility

out vec3 color;
in vec3 fragmentColor;

uniform vec2 resolution;
uniform float time;
uniform float zoom;
uniform vec2 centerPoint;
const float ASPECT_RATIO = resolution.x / resolution.y;

float mandel(vec2 point, int iterations) {
  int i = 0;
  vec2 a = vec2(0, 0);

  while(dot(a, a) < 4.0 && i < iterations) {
    float tmp = a.x * a.x - a.y * a.y + point.x;
    a.y = 2.0 * a.x * a.y + point.y;
    a.x = tmp;
    i++;
    if(i == iterations) {
      return 0.0;
    }
  }

  return float(i) / float(iterations);
}

float map(float x, float s1, float e1, float s2, float e2) {
  return (x - s1) / (e1 - s1) * (e2 - s2) + s2;
}

void main() {

  vec2 rightPoint;
  rightPoint.x = map(gl_FragCoord.x, 0, resolution.x, -zoom, zoom);
  rightPoint.y = map(gl_FragCoord.y, 0, resolution.y, -zoom / ASPECT_RATIO, zoom / ASPECT_RATIO);
  rightPoint += centerPoint;

  int iters = int(1 / sqrt(zoom)) + 64;
  float m = mandel(rightPoint, iters);
  color = vec3(m, m, m);
  /*
  if(true) {
    if(abs(rightPoint.x) < 0.001/zoom) {
      color = vec3(0.0, 1.0, 0.0);
    }
    if(abs(rightPoint.y) < 0.001/zoom) {
      color = vec3(1.0, 0.0, 0.0);
    }
  }
  */
}