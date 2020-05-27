#version 440 core

//layout(location = 0) uniform mat4 MVP;
//
//layout(location = 0) in vec3 vPosition;

//layout(location = 1) in vec3 vColors;
//layout(location = 2) in vec2 vTextureCoords;
//
//layout(location = 2) in vec3 vPosition;			//Local vertex Colors
//layout(location = 3) in vec3 vNormal;			//Vertex Normals
//
//layout(location = 0) out vec3 color;
//layout(location = 1) out vec2 textureCoord;
//
//layout(location = 2) out vec3 vPositionEyeSpace;
//layout(location = 3) out vec3 vNormalEyeSpace;
//layout(location = 4) out vec3 textureVector;

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

	// Vertex final Position (in Clip Coordenates)
	gl_Position = Projection * ModelView * vec4(vPosition, 1.0f);

}