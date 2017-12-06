precision mediump float;

varying   lowp    vec4 vr_dif;
varying   mediump vec2 vr_tx0;

uniform   lowp    sampler2D us_tx0;
uniform   lowp    sampler2D us_tx1;

void main() {
  vec4 color0  = texture2D(us_tx0, vr_tx0);
  vec4 color1  = texture2D(us_tx1, vr_tx0);

  gl_FragColor = (vr_dif * color0) * color1;
}
