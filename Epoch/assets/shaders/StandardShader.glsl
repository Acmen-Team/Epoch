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

struct Sampler_Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Vector_Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;

	glm::vec4 Color;
	float Intensity;
	float Range;
	float SpotAngle;

	// type of light...
	// ... Different light types correspond to different light shards.
	int type;
};

uniform vec3 ObjectColor;
uniform vec3 ViewPosition;
uniform sampler2D u_Texture1;

uniform Vector_Material v_material;
uniform Sampler_Material s_material;

uniform Light lights[6];

out vec4 FragColor;

vec3 LightCalculate(Light light)
{
	//
	vec3 ambient = light.ambient * vec3(texture(s_material.diffuse, v_TexCoord));

	//
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(light.position - v_WorldPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(s_material.diffuse, v_TexCoord));

	//
	float specularStrength = 0.5;
	vec3 viewDir = normalize(ViewPosition - v_WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), s_material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(s_material.specular, v_TexCoord));

	return ambient + diffuse + specular;
}

vec3 DirectionalLightCalculate(Light light)
{
	//
	vec3 ambient = light.ambient * vec3(texture(s_material.diffuse, v_TexCoord));

	//
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(s_material.diffuse, v_TexCoord));

	//
	float specularStrength = 0.5;
	vec3 viewDir = normalize(ViewPosition - v_WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), s_material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(s_material.specular, v_TexCoord));

	return ambient + diffuse + specular;
}

vec3 PointLightCalculate(Light light)
{
	//
	vec3 ambient = light.ambient * vec3(texture(s_material.diffuse, v_TexCoord));

	//
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(light.position - v_WorldPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(s_material.diffuse, v_TexCoord));

	//
	float specularStrength = 0.5;
	vec3 viewDir = normalize(ViewPosition - v_WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), s_material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(s_material.specular, v_TexCoord));
	
	//
	float distance = length(light.position - v_WorldPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 SpotLightCalculate(Light light)
{
	vec3 lightDir = normalize(light.position - v_WorldPos);
	float theta = dot(lightDir, normalize(light.position));

	if(theta > light.cutOff) 
	{
		//
		vec3 ambient = light.ambient * vec3(texture(s_material.diffuse, v_TexCoord));

		//
		vec3 norm = normalize(v_Normal);
		vec3 lightDir = normalize(light.position - v_WorldPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(s_material.diffuse, v_TexCoord));

		//
		float specularStrength = 0.5;
		vec3 viewDir = normalize(ViewPosition - v_WorldPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), s_material.shininess);
		vec3 specular = light.specular * spec * vec3(texture(s_material.specular, v_TexCoord));

		return ambient + diffuse + specular;
	}
	else
		return light.ambient * vec3(texture(s_material.diffuse, v_TexCoord));

}

void main()
{
	vec3 LightColor;
	vec3 result;
	for(int i = 0; i < 6; ++i) {

		vec3 ambient = lights[i].ambient * v_material.ambient;
		
		//
		vec3 norm = normalize(v_Normal);
		vec3 lightDir = normalize(lights[i].position - v_WorldPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = lights[i].diffuse * (diff * v_material.diffuse);
		
		//
		float specularStrength = 0.5;
		vec3 viewDir = normalize(ViewPosition - v_WorldPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), v_material.shininess);
		vec3 specular = lights[i].specular * (spec * v_material.specular);
		
		result += (ambient + diffuse + specular) * ObjectColor * texture(u_Texture1, v_TexCoord).rgb;

		//if(lights[i].type == 0)
		//{
		//	LightColor = LightCalculate(lights[i]);
		//}
		//else if(lights[i].type == 1)
		//{
		//	LightColor = DirectionalLightCalculate(lights[i]);
		//}
		//else if(lights[i].type == 2)
		//{
		//	LightColor = PointLightCalculate(lights[i]);
		//}
		//else if(lights[i].type == 3)
		//{
		//	LightColor = SpotLightCalculate(lights[i]);
		//}
		//LightColor = DirectionalLightCalculate(lights[i]);

		result += LightColor * ObjectColor * texture(u_Texture1, v_TexCoord).rgb;
	}

	FragColor = vec4(result, 1.0);
}