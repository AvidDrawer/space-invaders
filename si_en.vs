#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in float col;
out float C;
void main()
{
	gl_Position = vec4(aPos.x,aPos.y,0.0f, 1.0f);
	C=col;
}