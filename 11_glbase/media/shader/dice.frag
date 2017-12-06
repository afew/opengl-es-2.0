precision mediump float;

varying   vec2      vr_tex;
varying   vec3      vr_nor;
varying   vec3      vr_tzn;
varying   vec3      vr_eye;

uniform   vec3      lgt_dir;
uniform   vec4      lgt_dif;

uniform   sampler2D us_dif;
uniform   sampler2D us_nor;

void main() {
	vec4 ct_dif  = texture2D(us_dif, vr_tex);

	vec3 ct_nor  = normalize(texture2D(us_nor, vr_tex).xyz * 2.0 - 1.0);
	vec3 nor     = normalize(vr_nor);

	vec3 N = nor;
	vec3 T = normalize(vr_tzn);
	vec3 B = normalize(cross(T, N));
	     T = normalize(cross(B, N));

	mat3 tm = mat3(T, B, N);

	nor = tm * ct_nor;


	vec3 eye     = normalize(vr_eye);
	vec3 ref     = reflect(lgt_dir, nor); //lgt_dir - 2.0 * dot(lgt_dir, nor) * nor;

	float	spc  = dot(ref, eye);//(dot(ref, eye) + 1.0) *0.5;
			spc  = clamp(spc, 0.0, 1.0);
			spc  = pow(spc, 15.0)* 1.1;

	float	lgt  = (dot(-lgt_dir, nor) +1.0) * 0.48;
			lgt += spc;

	float   gray = (ct_dif.r + ct_dif.g + ct_dif.b) * 0.334;
			gray = pow(gray, 2.0)*1.2;


	gl_FragColor.rgb = gray * lgt_dif.rgb * lgt;
	gl_FragColor.a   = ct_dif.a   * lgt_dif.a;
}