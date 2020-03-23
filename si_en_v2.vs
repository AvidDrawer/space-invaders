#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 tex;
out float C;
out vec2 Tex;
uniform mat4 model;
uniform float col;
void main()
{
	gl_Position = model*vec4(aPos.x,aPos.y,0.0f, 1.0f);
	C=col;
	Tex=tex;
}