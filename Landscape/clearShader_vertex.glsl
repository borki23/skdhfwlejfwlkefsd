uniform mat4 gProjection;
uniform mat4 gModelView;
uniform float gTime;
attribute vec3 gNormal;

void main()
{
	vec4 pos = gProjection*gModelView*gl_Vertex;
	gl_Position = pos;
}

/*
	if (pos.y > 0.5f)
	{
		gl_Position = vec4(pos.x + 0.2*(pos.y/2.0)*sin(mod(gl_Time/1000.0f, 5.0f)), pos.y, pos.z ,pos.w);
	} else
	{
		gl_Position = pos;
	}
*/