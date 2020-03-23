#version 460 core
out vec4 Fragcolor;
in float C;
in vec2 Tex;
uniform sampler2D texture1;
void main()
{
//	Fragcolor = vec4(1.0f*C,0.0f,0.0f, 1.0f);
	Fragcolor = texture(texture1,Tex);
}