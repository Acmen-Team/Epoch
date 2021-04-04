#type VERTEX

#version 330 core
layout(location = 0) in vec3 a_Pos;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
}

#type FRAGMENT

#version 330 core

uniform vec4 u_Color;
out vec4 FragColor;

void main() 
{
	FragColor = u_Color;
}