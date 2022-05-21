#vertex
#version 450 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
void main()
{
	gl_Position = proj*view*model*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
#fragment
#version 450 core
out vec4 FragColor;
void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}