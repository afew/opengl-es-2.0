precision mediump float;

varying   vec2      vr_tex;
varying   vec3      vr_nor;
varying   vec3      vr_eye;

uniform   samplerCube us_cube;

void main() {
	vec3 N = normalize(vr_nor);
	vec3 E = normalize(vr_eye);

	vec3 r_cube = reflect(E, N);

	vec4 ct_dif  = textureCube(us_cube, r_cube);
	gl_FragColor = ct_dif;

}