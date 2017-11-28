
attribute highp   vec4 at_pos;

varying   lowp    vec4 vr_dif;

uniform   mediump mat4 um_Wld;
uniform   mediump mat4 um_Viw;
uniform   mediump mat4 um_Prj;
uniform   lowp    vec4 uc_dif;

void main() {
	vec4 pos= um_Wld * at_pos;
	pos = um_Viw * pos;
	pos = um_Prj * pos;
	gl_Position = pos;

	vr_dif = uc_dif;
}