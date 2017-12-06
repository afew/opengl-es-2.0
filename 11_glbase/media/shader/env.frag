precision mediump float;

varying   vec2      vr_tex;
uniform   sampler2D us_dif;

//varying   vec3        vr_nor;
//uniform   samplerCube us_cube;



void main() {

	vec4 ct_dif  = texture2D(us_dif, vr_tex);
	//vec4 ct_dif  = textureCube(us_cube, vr_nor);
	gl_FragColor = ct_dif;

}