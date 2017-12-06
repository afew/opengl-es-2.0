precision mediump float;

attribute highp   vec4 at_pos;
attribute lowp    vec4 at_dif;
attribute mediump vec2 at_tx0;

varying   lowp    vec4 vr_dif;
varying   mediump vec2 vr_tx0;

void main() {

  vr_dif      = at_dif;
  vr_tx0      = at_tx0;
  gl_Position = at_pos;
}
