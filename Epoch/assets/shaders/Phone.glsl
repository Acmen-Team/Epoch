#type VERTEX

#version 430 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec2 v_TexCoord;
out vec3 v_WorldPos;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Pos, 1.0);
	v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	v_TexCoord = a_TexCoord;
	v_WorldPos = vec3(u_Transform * vec4(a_Pos, 1.0));
}

#type FRAGMENT

#version 430 core

in vec3 v_Normal;
in vec2 v_TexCoord;
in vec3 v_WorldPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 LightPosition;
uniform vec3 ObjectColor;
uniform vec3 ViewPosition;

uniform sampler2D u_Texture1;

uniform Material material;
uniform Light light;

out vec4 FragColor;

void main()
{
	// 
	vec3 ambient = light.ambient * material.ambient;

	//
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(light.position - v_WorldPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//
	float specularStrength = 0.5;
	vec3 viewDir = normalize(ViewPosition - v_WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = (ambient + diffuse + specular) * ObjectColor * texture(u_Texture1, v_TexCoord).rgb;

	FragColor = vec4(result, 1.0);
}