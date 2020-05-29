#version 440 core

//global Ambient Light Source Struct
struct AmbientLight {
	vec3 ambient; //Global ambient light component
};

uniform AmbientLight ambientLight; // Fonte de luz ambiente global

//Directional Light Struct
struct DirectionalLight	{
	vec3 direction;		//Light Direction, world space
	
	vec3 ambient;		// ambient Light
	vec3 diffuse;		// Diffuse Light
	vec3 specular;		// Specular Light 
};

uniform DirectionalLight directionalLight; //Direction light Source

//Point Light Struct
struct PointLight	{
	vec3 position;		// Posicao do ponto de luz, espaco do mundo
	
	vec3 ambient;		// ambient Light
	vec3 diffuse;		// Diffuse Light
	vec3 specular;		// Specular Light 
	
	float constant;		// constant Atenuation coeficient
	float linear;		// linear atenuation coeficient
	float quadratic;	// quadratic atenuation coeficient
};

uniform PointLight pointLight[2]; //2 point light sources

//conic light source struct
struct SpotLight {
	vec3 position;		// Position of the light source in world space
	
	vec3 ambient;		// ambient Light
	vec3 diffuse;		// Diffuse Light
	vec3 specular;		// Specular Light 
	
	float constant;		// constant Atenuation coeficient
	float linear;		// linear atenuation coeficient
	float quadratic;	// quadratic atenuation coeficient

	float spotCutoffAngle, spotExponent;
	vec3 direction;
};

uniform SpotLight spotLight; // Conic Light Source

struct Material{
	vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

uniform Material material;

in vec3 vPositionEyeSpace; //Vertice Positon
in vec3 vNormalEyeSpace; //vertice Normal
in vec3 textureVector;

in vec3 color; //Vertice Color
in vec2 textureCoord;	 //Texture Coordinate

 uniform sampler2D TexSampler;

 layout (location = 0) out vec4 fColor; // Final Color of Fragments

 uniform samplerCube cubeMap;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;		// View * Model

vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light);
vec4 calcPointLight(PointLight light);
vec4 calcSpotLight(SpotLight light);
vec4 computeDiffuse(vec4 lightPosition, vec4 lightDiffuse);

uniform int ambientSwitch;
uniform int directionalSwitch;
uniform int pointSwitch;
uniform int coneSwitch;


void main()
{
	// Calculates the emissive material of the material
	vec4 emissive = vec4(material.emissive, 1.0);

	// calculates the effect of illumination on the fragment
	vec4 light[5];

	 for(int i = 0; i < 5; i++) light[i] = vec4(0,0,0,1); //Initiate Lights at 0
	
	// Ambient Light source
	if (ambientSwitch == 1) light[0] = calcAmbientLight(ambientLight);
	
	// Directional Light source
	if (directionalSwitch == 1) light[1] = calcDirectionalLight(directionalLight);
	
	//  point Light Light source
	if (pointSwitch == 1) for(int i=0; i<2; i++) light[i+2] = calcPointLight(pointLight[i]);		
	
	// cone Light source
	 if (coneSwitch == 1) light[4] = calcSpotLight(spotLight);

	//  Computes fragment final Color
	// With CubeMap
	//fColor = (emissive + light[0] + light[1] + light[2] + light[3] + light[4]) * texture(cubeMap, textureVector);
	//  With fragment Color
	fColor = (emissive + light[0] + light[1] + light[2] + light[3] + light[4]) * texture(TexSampler, textureCoord); //  vec4(1.0, 0.5, 0.5, 1.0);
}

vec4 calcAmbientLight(AmbientLight light) {
	// computes the global ambient light contribution for the object Color
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light) {
	// Computes the reflection of the ambient light Component
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	// Computes the reflection of the diffuse light 
	vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace); // inverse direction of direct Light
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
	
	// Computes the reflection of the specular light component	
	vec3 V = normalize(-vPositionEyeSpace);
	//vec4 H = normalize(L + V);	//  Blinn-Phong Model
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	// Blinn-Phong model
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	//Computes the direction light contribution for the final fragment Color
	return (ambient + diffuse + specular);
}

vec4 calcPointLight(PointLight light) {
	//Computes the reflection of the ambient light  component
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

		//Computes the reflection of the difuse light component
	//vec3 lightPositionEyeSpace = mat3(View) * light.position;
	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	//computes the reflexion of the specular light component
	vec3 V = normalize(-vPositionEyeSpace);
	//vec4 H = normalize(L + V);	//Blinn-Phong Model
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	//Blinn-Phong Model
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
	
	// attenuation
	float dist = length(mat3(View) * light.position - vPositionEyeSpace); //Computes the distance between a light point and a vertex
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	//Computes the point light contribution for the final color of the fragment
	return (attenuation * (ambient + diffuse + specular));
}

vec4 calcSpotLight(SpotLight light) {
	//Computes the reflection of the ambient light  component
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);

	//Computes the reflection of the difuse light component
	//vec3 lightPositionEyeSpace = mat3(View) * light.position;
	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 lightDirectionEyeSpace = normalize(vec4(light.direction,1.0) * View).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;

	//computes the reflexion of the specular light component
	vec3 V = normalize(-vPositionEyeSpace);
	//vec4 H = normalize(L + V);	//Blinn-Phong Model
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	//float NdotH = max(dot(N, H), 0.0);	//Blinn-Phong Model
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);	

	// attenuation
	float dist = length(mat3(View) * light.position - vPositionEyeSpace); //Computes the distance between a light point and a vertex
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	   
	float spotDot = dot(normalize(light.direction), -L);
	spotDot = max(spotDot,0);
	//float spotDot = dot(-V,normalize(light.direction));	
	float spotAttenuation;  // spotlight attenuation factor	

	if (acos(spotDot) < radians(light.spotCutoffAngle)) { //Cut off Angle	   
	spotAttenuation = pow(spotDot, light.spotExponent);
	//spotAttenuation = max(dot(N, -V), 0.0);	

	}
	else {
	spotAttenuation = 0.0; //Light adds no contribution
	}

	attenuation = spotAttenuation;

	//Computes the point light contribution for the final color of the fragment
	return (attenuation * (ambient + diffuse + specular));	

//return vec4(0.0);
}

