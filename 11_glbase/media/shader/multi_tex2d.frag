precision mediump float;

varying   vec2 vr_tx0;

uniform   sampler2D us_tx0;
uniform   sampler2D us_tx1;

void main() {

  vec4 color0  = texture2D(us_tx0, vr_tx0);
  vec4 color1  = texture2D(us_tx1, vr_tx0);

  gl_FragColor = vec4(color0.rgb, color1.r);
}
