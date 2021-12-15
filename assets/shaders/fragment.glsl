#version 450 compatibility

out vec3 color;
in vec3 fragmentColor;

uniform vec2 resolution;
uniform float time;
uniform float zoom;
uniform vec2 centerPoint;
const float ASPECT_RATIO = resolution.x / resolution.y;

double mandel(dvec2 point, int iterations) {
  int i = 0;
  dvec2 a = dvec2(0, 0);

  while(dot(a, a) < 4.0 && i < iterations) {
    double tmp = a.x * a.x - a.y * a.y + point.x;
    a.y = 2.0 * a.x * a.y + point.y;
    a.x = tmp;
    i++;
    if(i == iterations) {
      return 0.0;
    }
  }

  return double(i) / double(iterations);
}

float map(float x, float s1, float e1, float s2, float e2) {
  return (x - s1) / (e1 - s1) * (e2 - s2) + s2;
}

vec3 spectral_color(float l)        // RGB <0,1> <- lambda l <0.0, 1.0>
{
  l = map(l, 0.0, 1.0, 400.0, 700.0);
  float t;  vec3 c=vec3(0.0,0.0,0.0);
        if ((l>=400.0)&&(l<410.0)) { t=(l-400.0)/(410.0-400.0); c.r=    +(0.33*t)-(0.20*t*t); }
  else if ((l>=410.0)&&(l<475.0)) { t=(l-410.0)/(475.0-410.0); c.r=0.14         -(0.13*t*t); }
  else if ((l>=545.0)&&(l<595.0)) { t=(l-545.0)/(595.0-545.0); c.r=    +(1.98*t)-(     t*t); }
  else if ((l>=595.0)&&(l<650.0)) { t=(l-595.0)/(650.0-595.0); c.r=0.98+(0.06*t)-(0.40*t*t); }
  else if ((l>=650.0)&&(l<700.0)) { t=(l-650.0)/(700.0-650.0); c.r=0.65-(0.84*t)+(0.20*t*t); }
        if ((l>=415.0)&&(l<475.0)) { t=(l-415.0)/(475.0-415.0); c.g=             +(0.80*t*t); }
  else if ((l>=475.0)&&(l<590.0)) { t=(l-475.0)/(590.0-475.0); c.g=0.8 +(0.76*t)-(0.80*t*t); }
  else if ((l>=585.0)&&(l<639.0)) { t=(l-585.0)/(639.0-585.0); c.g=0.84-(0.84*t)           ; }
        if ((l>=400.0)&&(l<475.0)) { t=(l-400.0)/(475.0-400.0); c.b=    +(2.20*t)-(1.50*t*t); }
  else if ((l>=475.0)&&(l<560.0)) { t=(l-475.0)/(560.0-475.0); c.b=0.7 -(     t)+(0.30*t*t); }
  return c;
}

void main() {

  dvec2 rightPoint;
  rightPoint.x = map(gl_FragCoord.x, 0, resolution.x, -zoom, zoom);
  rightPoint.y = map(gl_FragCoord.y, 0, resolution.y, -zoom / ASPECT_RATIO, zoom / ASPECT_RATIO);
  rightPoint += centerPoint;

  int iters = int(1 / pow(zoom, 0.6)) + 64;
  float m = float(mandel(rightPoint, iters));
  color = spectral_color(m);
}