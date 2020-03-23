#version 460 core
out vec4 Fragcolor;
in float C;
void main()
{
	Fragcolor = vec4(1.0f*C,0.0f,0.0f, 1.0f);
}