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
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
};

uniform int LightModel;

uniform vec3 LightPosition;
uniform vec3 ObjectColor;
uniform vec3 ViewPosition;

uniform sampler2D u_Texture1;

uniform Material material;
uniform Light light;

out vec4 FragColor;

vec3 LightCalculate()
{
	//
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));

	//
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(light.position - v_WorldPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));

	//
	float specularStrength = 0.5;
	vec3 viewDir = normalize(ViewPosition - v_WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));

	return ambient + diffuse + specular;
}

vec3 DirectionalLightCalculate()
{
	//
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));

	//
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));

	//
	float specularStrength = 0.5;
	vec3 viewDir = normalize(ViewPosition - v_WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));

	return ambient + diffuse + specular;
}

vec3 PointLightCalculate()
{
	//
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));

	//
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(light.position - v_WorldPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));

	//
	float specularStrength = 0.5;
	vec3 viewDir = normalize(ViewPosition - v_WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));
	
	//
	float distance = length(light.position - v_WorldPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 SpotLightCalculate()
{
	vec3 lightDir = normalize(light.position - v_WorldPos);
	float theta = dot(lightDir, normalize(light.position));

	if(theta > light.cutOff) 
	{
		//
		vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TexCoord));

		//
		vec3 norm = normalize(v_Normal);
		vec3 lightDir = normalize(light.position - v_WorldPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TexCoord));

		//
		float specularStrength = 0.5;
		vec3 viewDir = normalize(ViewPosition - v_WorldPos);
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TexCoord));

		return ambient + diffuse + specular;
	}
	else
		return light.ambient * vec3(texture(material.diffuse, v_TexCoord));

}

void main()
{
	vec3 LightColor;

	if(LightModel == 0)
	{
		LightColor = LightCalculate();
	}
	else if(LightModel == 1)
	{
		LightColor = DirectionalLightCalculate();
	}
	else if(LightModel == 2)
	{
		LightColor = PointLightCalculate();
	}
	else if(LightModel == 3)
	{
		LightColor = SpotLightCalculate();
	}
	//LightColor = PointLightCalculate();

	vec3 result = LightColor * ObjectColor * texture(u_Texture1, v_TexCoord).rgb;

	FragColor = vec4(result, 1.0);
}