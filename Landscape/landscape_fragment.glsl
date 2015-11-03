//precision lowp float;
uniform float gTime;
varying float oHeight;
uniform float bTerrainMode;

void main()
{
	if (bTerrainMode == 0.0f)
	{
		gl_FragColor = vec4(1.0,0.0,0.0,1.0);
		return;
	}
	if (oHeight < 0.35) 
	{
		gl_FragColor  = vec4(0.0,0.0,1.0,1.0);
	} else
	{
		gl_FragColor = vec4(mix(vec3(0.0,1.0,0.0),vec3(1.0),oHeight),1.0);
	}
}