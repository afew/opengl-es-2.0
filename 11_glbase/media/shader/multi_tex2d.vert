precision mediump float;

attribute vec4 at_pos;
attribute vec2 at_tx0;

varying   vec2 vr_tx0;

void main() {

  vr_tx0      = at_tx0;
  gl_Position = at_pos;
}
