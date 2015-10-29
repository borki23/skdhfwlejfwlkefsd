int snowHeight;
precision lowp float;
uniform float gl_Time;
void main()
{
   gl_FragColor = vec4( 1.0, abs(sin(mod(gl_Time/1000.0f,500.0))), 0.0, 1.0 );
}