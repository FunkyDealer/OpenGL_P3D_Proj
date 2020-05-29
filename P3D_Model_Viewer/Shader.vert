#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;		// View * Model
uniform mat4 Projection;
uniform mat3 NormalMatrix;

 in vec3 vColors;
 in vec2 vTextureCoords;

 in vec3 vPosition;			//Local vertex Colors
 in vec3 vNormal;			//Vertex Normals

 out vec3 color;
 out vec2 textureCoord;

 out vec3 vPositionEyeSpace;
 out vec3 vNormalEyeSpace;
 out vec3 textureVector;

 uniform float time;

const float amplitude = 0.125;
const float frequency = 2;
const float PI = 3.14159;

uniform int deforming;

void switchDeform(mat4 MVP);
void deformModel(mat4 MVP);
void ExpandModel(mat4 MVP);

void main()
{ 
	color = vColors;

	textureCoord = vTextureCoords;

	// Vertex position in Eye Coordinates
	vPositionEyeSpace = (ModelView * vec4(vPosition, 1.0)).xyz;

	// transform the vertex normal
	vNormalEyeSpace = normalize(NormalMatrix * vNormal);

	// texture Coordinate for the Cubermap
	textureVector = vPosition;	

	mat4 MVP = Projection * ModelView;

	switchDeform(MVP);

}

void switchDeform(mat4 MVP) {

	switch (deforming) {
		case 1:
		deformModel(MVP);
		break;
		case 2:
		ExpandModel(MVP);
		break;
		case 0:
		gl_Position =  MVP * vec4(vPosition, 1.0f);
		break;
	}	

}


void deformModel(mat4 MVP) {	

	float distance = length(vPosition);
	float z = amplitude*sin(-PI*distance*frequency+time);

	gl_Position = MVP * vec4(vPosition.x, vPosition.y, z,1);

}

void ExpandModel(mat4 MVP)
{
	vec3 vPos=vPosition*0.004*time;
    gl_Position = MVP*vec4(vPos,1f);

}