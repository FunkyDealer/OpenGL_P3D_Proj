#version 440 core

layout(location = 0) in vec3 color; //Vertice Color
layout (location = 1) in vec2 textureCoord;	 //Texture Coordinate

uniform sampler2D TexSampler;

layout (location = 0) out vec4 fColor; // Final Color of Fragments

void main()
{
	fColor = texture(TexSampler, textureCoord);
}



