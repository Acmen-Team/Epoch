#type VERTEX

#version 430 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

out vec2 v_TexCoord;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	v_TexCoord = a_TexCoord;
}

#type FRAGMENT

#version 430 core

uniform sampler2D u_Texture1;

in  vec2 v_TexCoord;

out vec4 FragColor;

void main()
{
    vec4 result = vec4(texture(u_Texture1, v_TexCoord).xyz, 1.0);
	FragColor = result;
}