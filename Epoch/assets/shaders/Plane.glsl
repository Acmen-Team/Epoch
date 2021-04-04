#type VERTEX

#version 330 core
layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 3) in vec3 a_Normal;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() 
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	v_FragPos = vec3(u_Transform * vec4(a_Pos, 1.0));
	v_TexCoord = a_TexCoord;
	v_Normal = mat3(transpose(inverse(u_Transform))) * v_Normal;
}

#type FRAGMENT

#version 330 core

uniform sampler2D u_Texture1;
uniform vec3 u_viewPos;
uniform float u_AmbientStrength;
uniform float u_SpecularStrength;
uniform vec4 u_LightColor;
uniform vec3 u_lightPos;

in  vec2 v_TexCoord;
in  vec3 v_Normal;
in  vec3 v_FragPos;
out vec4 FragColor;

void main()
{
	vec4 ambient = u_AmbientStrength * u_LightColor;

	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_lightPos - v_FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColor.xyz;

	vec3 viewDir = normalize(u_viewPos - v_FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = u_SpecularStrength * spec * u_LightColor.xyz;

    vec4 result = vec4((ambient.xyz + diffuse + specular) * texture(u_Texture1, v_TexCoord).xyz, 1.0);
	FragColor = result;
}