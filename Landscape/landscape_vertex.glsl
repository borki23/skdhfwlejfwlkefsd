uniform mat4 gl_ModelView;
uniform mat4 gl_Projection;
precision lowp float;
uniform float gl_Time;

void main()
{
	vec4 pos = gl_Projection*gl_ModelView*gl_Vertex;
	if (pos.y > 0.5f)
	{
		gl_Position = vec4(pos.x + 0.2*(pos.y/2.0)*sin(mod(gl_Time/1000.0f, 5.0f)), pos.y, pos.z ,pos.w);
	} else
	{
		gl_Position = pos;
	}
}