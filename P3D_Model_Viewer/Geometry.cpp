#include "Geometry.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

Model LoadCube() { //Generates  Cube
	Model model;
	model.modelTotalVertices = 6 * 6;
	

	GLfloat g_vertex_buffer_data[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,	//Start
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,	//End
	1.0f, 1.0f,-1.0f,	//Start
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,	//End
	-1.0f,-1.0f,-1.0f,	//Start
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,	//End
	1.0f,-1.0f, 1.0f,	//Start
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,	//End
	-1.0f, 1.0f, 1.0f,	//Start
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,	//End
	1.0f, 1.0f, 1.0f,	//Start
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,	//End
	1.0f,-1.0f,-1.0f,	//Start
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,	//End
	1.0f, 1.0f, 1.0f,	//Start
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,	//End
	1.0f, 1.0f, 1.0f,	//Start
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,	//End
	1.0f, 1.0f, 1.0f,	//Start
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f	//End
	};

	GLfloat *vertices = (GLfloat *)malloc(model.modelTotalVertices * 3 * sizeof(GLfloat));

	for (int i = 0; i < model.modelTotalVertices * 3; i++)
	{
		vertices[i] = g_vertex_buffer_data[i];
	}

	model.vertices = vertices;

	GLfloat *colors = (GLfloat *)malloc(model.modelTotalVertices * 3 * sizeof(GLfloat));

	for (int i = 0; i < model.modelTotalVertices; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//vertices[i][j] = 1.0f, 0.0f, 0.0f
			switch (j) {
			case 0:
				colors[i * 3 + j] = 1.0f;
				break;
			case 1:
				colors[i * 3 + j] = 0.0f;
				break;
			case 2:
				colors[i * 3 + j] = 0.0f;
				break;
			}
		};
	}

	model.colors = colors;
	model.setName("cube");
	
	return model;
}


Model LoadTriangle() {
	Model model;
	model.modelTotalVertices = 3;

	GLfloat g_vertex_buffer_data[] = {
	 -1.0f, -1.0f, 0.0f ,
	 1.0f, -1.0f, 0.0f ,
	 0.0f,  1.0f, 0.0f }; // Triangulo coordenadas vertices

	GLfloat *vertices = (GLfloat *)malloc(model.modelTotalVertices * 3 * sizeof(GLfloat));

	for (int i = 0; i < model.modelTotalVertices * 3; i++)
	{
		vertices[i] = g_vertex_buffer_data[i];
	}

	model.vertices = vertices;

	GLfloat g_vertex_data_data[] = {
	1.0f, 0.0f, 0.0f ,
	0.0f, 1.0f, 0.0f ,
	0.0f, 0.0f, 1.0f  // Triangulo Cores Vertices
	};

	GLfloat *colors = (GLfloat *)malloc(model.modelTotalVertices * 3 * sizeof(GLfloat));

	for (int i = 0; i < model.modelTotalVertices * 3; i++)
	{
		colors[i] = g_vertex_data_data[i];

	}

	model.colors = colors;
	model.setName("Triangle");

	return model;
}

Model LoadXYZModel(string fileName)
{
	Model model;
	model.setName(fileName);
	//vector<vec3> VerticesVec;
	vector<GLfloat> Vertices[3], Text_Coords[2], Vertice_Normals[3];
	GLfloat X = 0, Y = 0, Z = 0, XT = 0, YT = 0, XN = 0, YN = 0, ZN = 0;
	string material = "test";
	ifstream file(fileName);
	int nv = 0, nvt = 0, nvn = 0;


	if (file.is_open())
	{
		//GLint size;
		string line;

		file >> line;
		if (line.compare("mtllib") == 0)
		{
			file >> material;
			cout << "fount material " << material << endl;
		}

		while (getline(file, line))
		{
			file >> line;

			if (line.compare("mtllib") == 0)
			{
				file >> material;
				cout << "fount material " << material << endl;
			}
			else if (line == "v")
			{
				file >> X >> Y >> Z;
				
				Vertices[0].push_back(X);
				Vertices[1].push_back(Y);
				Vertices[2].push_back(Z);	
				//VerticesVec.push_back(vec3(X, Y, Z));
				nv++;				
			}
			else if (line == "vt")
			{
				file >> XT >> YT;
				Text_Coords[0].push_back(XT);
				Text_Coords[1].push_back(YT);
				nvt++;
			}
			else if (line == "vn")
			{
				file >> XN >> YN >> ZN;
				Vertice_Normals[0].push_back(XN);
				Vertice_Normals[1].push_back(YN);
				Vertice_Normals[2].push_back(ZN);

				nvn++;
			}
		}
		int n = 0;

		file.close();

		model.modelTotalVertices = nv;
		model.modelTotalNormals = nvn;
		model.modelTotalTextures = nvt;
		model.modelTotalColors = nv;
		model.setMaterialFile(material);
		cout << "ENDED ALL POINTS" << endl;
		cout << "TOTAL VERTICES FOUND: " << model.modelTotalVertices << endl;
		cout << "TOTAL TEXTURE COORDENATES FOUND: " << model.modelTotalNormals << endl;
		cout << "TOTAL NORMALS FOUND: " << model.modelTotalNormals << endl;
		cout << "MATERIAL: " << material << endl;

	}
	else
	{
		cout << "ERROR, FILE NOT FOUND" << endl;
		throw;
	}

	//PLACE THE INFORMATION FROM THE LISTS INTO THE ARRAYS
	GLfloat *vertices = (GLfloat *)malloc(model.modelTotalVertices * 3 * sizeof(GLfloat)); //Vertices

	int currentV = 0;
	for (int i = 0; i < model.modelTotalVertices; i++)
	{
		for (int n = 0; n < 3; n++)
		{
			vertices[currentV] = Vertices[n].at(i);
			currentV++;
		}
	}

	model.vertices = vertices;

	GLfloat *verticesTexture = (GLfloat *)malloc(model.modelTotalTextures * 2 * sizeof(GLfloat)); //Texture

	currentV = 0;
	for (int i = 0; i < model.modelTotalTextures; i++)
	{
		for (int n = 0; n < 2; n++)
		{
			verticesTexture[currentV] = Text_Coords[n].at(i);
			currentV++;
		}
	}

	model.textures = verticesTexture;

	GLfloat *verticesNormal = (GLfloat *)malloc(model.modelTotalNormals * 3 * sizeof(GLfloat)); //Normals

	currentV = 0;
	for (int i = 0; i < model.modelTotalNormals; i++)
	{
		for (int n = 0; n < 3; n++)
		{
			verticesNormal[currentV] = Vertice_Normals[n].at(i);
			currentV++;
		}
	}

	model.normals = verticesNormal;

	GLfloat *verticesColors = (GLfloat *)malloc(model.modelTotalColors * 3 * sizeof(GLfloat)); //Colors

	for (int i = 0; i < model.modelTotalColors; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			//vertices[i][j] = 1.0f, 0.0f, 0.0f
			switch (j) {
			case 0:
				verticesColors[i * 3 + j] = 1.0f;
				break;
			case 1:
				verticesColors[i * 3 + j] = 0.0f;
				break;
			case 2:
				verticesColors[i * 3 + j] = 0.0f;
				break;
			}
		};
	}

	model.colors = verticesColors;

	model.material = getMaterial(model.getMaterialFile());
	
	model.setName("Model");

	return model;
}

Material getMaterial(string file) {
	Material material;

	material.name = file;
	string mat = "test";
	string MAP = "";
	float matKA[3], matKS[3], matKD[3];
	GLfloat X = 0, Y = 0, Z = 0, // ka
		XI = 0, YI = 0, ZI = 0, // kd
		XS = 0, YS = 0, ZS = 0, // ks
		NS = 0, NI = 0;

	ifstream fileMat("Model/" + file);

	if (fileMat.is_open())
	{
		string line;

		fileMat >> line;
		if (line.compare("newmtl") == 0)
		{
			fileMat >> mat;
			cout << "mat " << mat << endl;
		}

		while (getline(fileMat, line))
		{
			fileMat >> line;

			if (line.compare("newmtl") == 0)
			{
				fileMat >> mat;
				cout << "found mat " << mat << endl;
			}
			else if (line == "Ka")
			{
				fileMat >> X >> Y >> Z;

				matKA[0] = X;
				matKA[1] = Y;
				matKA[2] = Z;
			}
			else if (line == "Kd")
			{
				fileMat >> XI >> YI >> ZI;

				matKD[0] = XI;
				matKD[1] = YI;
				matKD[2] = ZI;
			}
			else if (line == "Ks")
			{
				fileMat >> XS >> YS >> ZS;

				matKS[0] = XS;
				matKS[1] = YS;
				matKS[2] = ZS;
			}
			else if (line == "Ns")
			{
				fileMat >> NS;
				
			}
			else if (line == "Ni")
			{
				fileMat >> NI;				
			}
			else if (line.compare("map") == 0)
			{
				fileMat >> MAP;

				cout << "found map: " << MAP << endl;
			}
		}
		fileMat.close();
	}
	else
	{
		cout << "ERROR, FILE NOT FOUND" << endl;
		throw;
	}
	material.specular_Exponent = NS;

	material.Refraction = NI;

	material.map = "Model/";
	material.map.append(MAP);


	vec3 kaCoords = vec3(matKA[0], matKA[1], matKA[2]);

	material.ambient = kaCoords;

	vec3 kdCoords = vec3(matKD[0], matKD[1], matKD[2]);

	material.difuse = kdCoords;

	vec3 ksCoords = vec3(matKS[0], matKS[1], matKS[2]);

	material.specular = ksCoords;

	return material;
}

void load_Model_texture(Model model) {
	GLuint textureName = 0;

	//Generates a name for the Texture
	glGenTextures(1, &textureName);

	//Activate Texture Unit #0
	glActiveTexture(GL_TEXTURE0);

	//Assigns that texture name to the target GL_TEXTURE_2D from the active Texture Unit
	glBindTexture(GL_TEXTURE_2D, textureName);

	//Defines filtering parameters (Wrapping and size adjusting)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Reading and unconpressing of the file with the texture Image
	int width, height, nChannels;
	//Activates Image vertical flipping
	stbi_set_flip_vertically_on_load(true);
	//Loading the Image to the CPU

	string textureFile = model.material.map; //Gets the texture file from the model's material
	const char *cstr = textureFile.c_str(); //Converts the string to a constant char

	unsigned char *imageData = stbi_load(cstr, &width, &height, &nChannels, 0);
	if (imageData) {
		//Loads the image data to the assigned texture object of the GL_TEXTURE_2d
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);

		//Generate a mipMap for that texture
		glGenerateMipmap(GL_TEXTURE_2D);

		//Unloads the image from CPU Memory
		stbi_image_free(imageData);
	}
	else {
		cout << "Error loading texture!" << endl;
	}
}




