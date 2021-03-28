#type VERTEX

#version 330 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec4 a_Color;

out vec2 v_TexCoord;
out vec4 v_Color;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
}

#type FRAGMENT

#version 330 core

uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;

in  vec2 v_TexCoord;
in  vec4 v_Color;
out vec4 FragColor;

void main() 
{
	FragColor = mix(texture(u_Texture1, v_TexCoord), texture(u_Texture2, v_TexCoord), 0.2);
}