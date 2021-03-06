const float HEIGHTBARRIER = 150.0; 
const float WATERBARRIER = 25.0; 
uniform mat4 gProjection;
uniform mat4 gModelView;
uniform float gTime;
varying float oHeight;
attribute vec3 gNormal;

uniform float bTerrainMode;

void main(){
	vec4 pos = gProjection*gModelView*gl_Vertex;
	if (bTerrainMode == 1.0f){
		
		oHeight = gl_Vertex.y/HEIGHTBARRIER;
		if (gl_Vertex.y < WATERBARRIER){
			pos = vec4(pos.x, WATERBARRIER, pos.z, 1.0);
		}
	}	
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