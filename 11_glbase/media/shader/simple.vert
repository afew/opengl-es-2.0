
attribute highp vec4 at_pos;
uniform mediump mat4 transformationMatrix;
void main()
{
	gl_Position = at_pos * transformationMatrix;
}

