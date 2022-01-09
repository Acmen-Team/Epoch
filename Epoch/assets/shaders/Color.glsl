#type VERTEX

#version 430 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos + a_Normal * 0.0, 1.0);
}

#type FRAGMENT

#version 430 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.7, 0.7, 0.15, 1.0);
}