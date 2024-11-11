#version 400
#define MAX_NUM_LIGHTS 4
#define MAX_SEMPLES 20

in vec3 normal;
in vec3 f_pos;
in vec3 color;
in vec2 tex_coord;
in vec4 f_pos_light_space;
out vec4 frag_color;



struct DirLight
{
 vec3 direction;
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};



struct PointLight
{
 vec3 position;
 float constant;
 float linear;
 float quadratic;
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;

 mat4 projection;
 float farPlane;
 int idShadowCubeMap;
};



struct SpotLight
{
 vec3 position;
 vec3 direction;
 float cutOff;
 float outerCutOff;
 float constant;
 float linear;
 float quadratic;
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;

 mat4 projection;
 float farPlane;
 int idShadowCubeMap;
};



struct LightsInfo
{
 DirLight dirLight;
 PointLight pointLights[MAX_NUM_LIGHTS];
 SpotLight spotLights[MAX_NUM_LIGHTS];
 int numPLights;
 int numSLights;
};


struct Material
{
 sampler2D texture;
 sampler2D specularmap;
 vec3 specular;
 float shininess;
};


uniform LightsInfo lights;
uniform Material material;
uniform vec3 campos;
uniform sampler2D shadowmap;
uniform samplerCube[MAX_NUM_LIGHTS*2] shadowCubeMaps;


vec3 sampleOffsetDirections[MAX_SEMPLES] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   


vec3 CalcDirLight (DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight (SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
	vec3 norm = normalize (normal);
	vec3 viewDir = normalize (campos - f_pos);
	vec3 result = CalcDirLight (lights.dirLight, norm, viewDir);
	for (int i = 0; i < lights.numPLights; i++){
		result += CalcPointLight(lights.pointLights[i], norm, f_pos, viewDir);
	}
	for (int j = 0; j < lights.numSLights; j++){
		result += CalcSpotLight(lights.spotLights[j], norm, f_pos, viewDir);
	}

	// Итоговый цвет фрагмента
	frag_color = vec4 (result, 1.0);
}



vec3 CalcDirLight (DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize (-light.direction);
	float diff = max (dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect (-lightDir, normal);
	float spec = pow (max (dot (viewDir, reflectDir), 0.0), material.shininess);
	vec3 ambient = light.ambient * vec3 (texture (material.texture, tex_coord));
	vec3 diffuse = light.diffuse * diff * vec3 (texture (material.texture, tex_coord));
	vec3 specular = light.specular * spec * vec3 (texture (material.specularmap, tex_coord));

	//PCF + shadow
	float shadow = 0.0f;
	vec3 lightCoord = f_pos_light_space.xyz / f_pos_light_space.w;
	if(lightCoord.z <= 1.0f){
		lightCoord = (lightCoord + 1.0f) / 2.0f;

		float closestDepth = texture(shadowmap,lightCoord.xy).r;
		float currentDepth = lightCoord.z;

		float bias = max(0.001f * (1.0f - diff), 0.0005f );

		vec2 texelSize = 1.0 / textureSize(shadowmap, 0);

		for(int x = -1; x <= 1; ++x)
		{
			for(int y = -1; y <= 1; ++y)
			{
				float pcfDepth = texture(shadowmap, lightCoord.xy + vec2(x, y) * texelSize).r;
				shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
			}
		}
		shadow /= 9.0;

	}

	return (ambient* (1.5f - shadow) + diffuse* (1.0f - shadow) + specular* (1.0f - shadow));
}



vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	vec3 ambient = light.ambient * vec3 (texture (material.texture, tex_coord));
	vec3 diffuse = light.diffuse * diff * vec3 (texture (material.texture, tex_coord));
	vec3 specular = light.specular * spec * vec3 (texture (material.specularmap, tex_coord));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	
	vec3 fragToLight = fragPos - light.position;
	float currentDepth = length(fragToLight);

	float shadow = 0.0;
	float bias   = max(0.001f * (1.0f - diff), 0.0005f );
	float viewDistance = length(light.position - fragPos);
	float diskRadius = 0.05;
	for(int i = 0; i < MAX_SEMPLES; ++i)
	{
		float closestDepth = texture(shadowCubeMaps[light.idShadowCubeMap], fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		closestDepth *= light.farPlane;
		if(currentDepth - bias > closestDepth)
			shadow += 1.0;
	}
	shadow /= float(MAX_SEMPLES); 

	return (ambient * (1.5f - shadow) + diffuse* (1.0f - shadow) + specular* (1.0f - shadow));
}



vec3 CalcSpotLight (SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize (light.position - fragPos);
	vec3 result;
	float theta = dot (lightDir, normalize(-light.direction));

	if ( theta < light.outerCutOff )
	{
		result = vec3(0.f,0.f,0.f);
	}else
	{		

		vec3 fragToLight = fragPos - light.position;
		float currentDepth = length(fragToLight);

		float shadow = 0.0;
		float bias   = max(0.001f * (1.0f - max (dot(norm, lightDir), 0.0)), 0.0005f );
		float viewDistance = length(light.position - fragPos);
		float diskRadius = 0.05;
		for(int i = 0; i < MAX_SEMPLES; i+=2)
		{
			float closestDepth = texture(shadowCubeMaps[light.idShadowCubeMap], fragToLight + sampleOffsetDirections[i] * diskRadius).r;
			closestDepth *= light.farPlane;
			if(currentDepth - bias > closestDepth)
				shadow += 1.0;
		}
		shadow /= float(MAX_SEMPLES);
		
		if(theta > light.cutOff){
			result = (light.ambient * (1.0f - shadow)) * vec3 (texture (material.texture, tex_coord));
		}
		else{
			vec3 ambient = light.ambient * vec3 (texture (material.texture, tex_coord));
			float diff = max (dot(norm, lightDir), 0.0);
			vec3 diffuse = light.diffuse * diff * vec3 (texture (material.texture, tex_coord));
			vec3 viewDir = normalize (campos - fragPos);
			vec3 reflectDir = reflect (-lightDir, norm);
			float spec = pow (max(dot(viewDir, reflectDir), 0.0), material.shininess);
			vec3 specular = light.specular * spec * vec3 (texture (material.specularmap, tex_coord));
			float distance = length (light.position - fragPos);
			float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
			float epsilon   = light.cutOff - light.outerCutOff;
			float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
			ambient *= attenuation * intensity;
			diffuse *= intensity;
			specular *= intensity;
			result = (ambient * (0.5f - shadow) + diffuse* (1.0f - shadow) + specular* (1.0f - shadow));
		}
	}


	return result;
}