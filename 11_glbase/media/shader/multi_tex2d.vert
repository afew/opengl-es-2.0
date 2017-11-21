precision mediump float;

attribute vec4 att_pos;
attribute vec2 att_tx0;

varying   vec2 var_tx0;

void main() {

  var_tx0     = att_tx0;
  gl_Position = att_pos;
}
