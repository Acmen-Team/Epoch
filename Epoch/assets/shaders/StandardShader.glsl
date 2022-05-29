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
	vec4 color;
	float intensity;
	float range;
	float spotAngle;

	// type of light...
	// ... Different light types correspond to different light shards.
	int type;

	// no shader..
	vec3 diffuse;
	vec3 specular;
	vec3 ambient;


	float constant;
    float linear;
    float quadratic;
};

uniform vec3 ObjectColor;
uniform vec3 ViewPosition;
uniform sampler2D u_Texture1;

uniform Vector_Material v_material;
uniform Sampler_Material s_material;

uniform Light lights[6];
uniform int LightNums;
out vec4 FragColor;


vec3 DirectionalLightCalculate(Light light)
{
	vec3 result;

	vec3 lightDir = normalize(vec3(-light.direction.z, -light.direction.y, light.direction.x) + vec3(0.0, 1.0, 0.0));
	vec3 norm = normalize(v_Normal);

	result = max(dot(norm, lightDir), 0.0) * (light.color).rgb;

	return result * light.intensity;
}



vec3 PointLightCalculate(Light light)
{
	vec3 res;

	float range = light.range;
	light.constant = 1.0;
	
	if(range <= 0) {
		return vec3(0.0, 0.0, 0.0);
	}
	else if(range <= 7) {
		light.linear = 0.7;
		light.quadratic = 1.8;
	}
	else if(range <= 13) {
		light.linear = 0.35;
		light.quadratic = 0.44;
	}
	else if(range <= 20) {
		light.linear = 0.22;
		light.quadratic = 0.20;
	}
	else if(range <= 32) {
		light.linear = 0.14;
		light.quadratic = 0.07;
	}
	else if(range <= 50) {
		light.linear = 0.09;
		light.quadratic = 0.032;
	}
	else if(range <= 65) {
		light.linear = 0.07;
		light.quadratic = 0.017;
	}
	else if(range <= 100) {
		light.linear = 0.045;
		light.quadratic = 0.0075;
	}
	else if(range <= 160) {
		light.linear = 0.027;
		light.quadratic = 0.0028;
	}
	else {
		light.linear = 0.022;
		light.quadratic = 0.0019;
	}

	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(light.position - v_WorldPos);

	float ambientStrength = 1.0;
	light.ambient = (light.color).xyz * ambientStrength;

	float diff = max(dot(norm, lightDir), 0.0);
	light.diffuse = diff * (light.color).rgb;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(ViewPosition - v_WorldPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), v_material.shininess);
	light.specular = specularStrength * spec * (light.color).rgb;

	vec3 ambient = light.ambient * v_material.ambient;
	vec3 diffuse = light.diffuse * diff * v_material.diffuse;
	vec3 specular = light.specular * spec * v_material.specular;
		
	float distance = length(light.position - v_WorldPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	
	res = ambient + diffuse + specular;
	
	return res * light.intensity;
}


vec3 SpotLightCalculate(Light light)
{
	vec3 res;	

	vec3 norm = normalize(v_Normal);

	vec3 lightDir = normalize(light.position - v_WorldPos);
	vec3 spotDir = normalize(vec3(-light.direction.z, -light.direction.y, light.direction.x) + vec3(0.0, 1.0, 0.0));
	
	float theta = dot(lightDir, spotDir);

	float proLightDir = length(light.position - v_WorldPos) * theta;

	if(theta > light.spotAngle && light.range > proLightDir) {
		res = max(dot(norm, lightDir), 0.0) * (light.color).rgb;
	}
	else {
		res = vec3(0.0, 0.0, 0.0);
	}

	return res * light.intensity;
}

void main()
{
	vec3 result;
	for(int i = 0; i < LightNums; ++i) {
		vec3 LightColor;
		
		if(lights[i].type == 0) {
			LightColor = DirectionalLightCalculate(lights[i]);
		}
		else if(lights[i].type == 1) {
			LightColor = PointLightCalculate(lights[i]);
		}
		else if(lights[i].type == 2) {
			LightColor = SpotLightCalculate(lights[i]);
		}

		result += LightColor * ObjectColor * texture(u_Texture1, v_TexCoord).rgb;
	}

	FragColor = vec4(result, 1.0);
}