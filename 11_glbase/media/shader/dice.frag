precision mediump float;

varying   vec2      vr_tex;
varying   vec3      vr_nor;
varying   vec3      vr_eye;

uniform   vec3      lgt_dir;
uniform   vec4      lgt_dif;

uniform   sampler2D us_dif;
uniform   sampler2D us_nor;

void main() {
	vec3 nor     = normalize(vr_nor);
	vec3 eye     = normalize(vr_eye);


	float	spc    = (dot(reflect(lgt_dir, nor), eye) + 1.0) *0.5;
			spc    = clamp(spc, 0.0, 1.0);
			spc    = pow(spc, 2.0)* 0.9;

	float lam    = (dot(-lgt_dir, nor) + 1.0) * 0.4;
	float lgt    = lam + spc;


	vec4 ct_dif  = texture2D(us_dif, vr_tex);
	vec3 ct_nor  = normalize(texture2D(us_nor, vr_tex).xyz * 2.0 - 1.0);


	vec3 B = vec3(1.0, 0.0, 0.0);
	vec3 T = vec3(0.0, 1.0, 0.0);
	vec3 N = normalize(vr_nor);
	T = normalize(cross(N, B));
	B = normalize(cross(T, N));

	mat3 tm = mat3(B, T, N);

	vec3 C = ct_nor.xyz;

	ct_nor = tm *C;



	float bump   = (dot(lgt_dir, ct_nor) + 1.0) * 0.5;
			bump   = pow(bump, 5.0) * 5.0;

	//lgt *= bump;

	gl_FragColor.rgb = ct_nor;//vec3(lgt, lgt, lgt);	//gl_FragColor.rgb = ct_dif.rgb * lgt_dif.rgb * lgt;
	gl_FragColor.a   = ct_dif.a * lgt_dif.a;
}