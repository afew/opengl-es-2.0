
attribute vec4      at_pos;
attribute vec3      at_nor;
attribute vec3      at_bnr;
attribute vec2      at_tex;

varying   vec2      vr_tex;
varying   vec3      vr_nor;
varying   vec3      vr_bnr;
varying   vec3      vr_eye;

uniform   mat4      um_Wld;
uniform   mat4      um_Viw;
uniform   mat4      um_Prj;
uniform   vec3      uf_cam;

void main() {
	vec4 pos    = um_Wld * at_pos;

	vr_tex      = at_tex;
	vr_nor      = vec3(um_Wld * vec4(at_nor, 0.0));
	vr_bnr      = vec3(um_Wld * vec4(at_bnr, 0.0));
	vr_eye      = uf_cam - vec3(pos);

	pos         = um_Viw * pos;
	pos         = um_Prj * pos;
	gl_Position = pos;
}