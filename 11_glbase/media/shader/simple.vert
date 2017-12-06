
attribute mediump vec4 at_pos;
uniform   mediump mat4 um_tmWorld;
void main()
{
	gl_Position = um_tmWorld * at_pos;
}

