
// fragment shader.
varying   mediump vec2    vr_tx0;
uniform   lowp sampler2D  us_tx0;

void main() {
	lowp vec4	ret = texture2D(us_tx0, vr_tx0);
	gl_FragColor = ret;
}

