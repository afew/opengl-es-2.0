
// vertex shader.
attribute mediump vec4   at_pos;
attribute mediump vec2   at_tx0;

varying   mediump vec2   vr_tx0;

void main() {
	gl_Position = at_pos;
	vr_tx0 = at_tx0;
}
