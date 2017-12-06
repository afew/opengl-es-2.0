
attribute vec4      at_pos;
attribute vec2      at_tex;

varying   vec2      vr_tex;

//varying   vec3      vr_nor;

uniform   mat4      um_Wld;
uniform   mat4      um_Viw;
uniform   mat4      um_Prj;


void main() {
	vr_tex      = at_tex;

	vec4 pos    = um_Wld * at_pos;
	pos         = um_Viw * pos;
	pos         = um_Prj * pos;
	gl_Position = pos;
}