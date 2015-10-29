uniform mat4 gl_ModelView;
uniform mat4 gl_Projection;

void main()
{
    gl_Position =  gl_Projection * gl_ModelView * gl_Vertex;
}