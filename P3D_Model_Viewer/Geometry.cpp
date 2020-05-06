#include "Geometry.h"


Model LoadCube() {
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

	return model;
}



Model Load3dModel(string fileName)
{
	Model model;
	vector<float> Vertices[3], Text_Coords[2], Vertice_Normals[3];
	float X = 0, Y = 0, Z = 0, XT = 0, YT = 0, XN = 0, YN = 0, ZN = 0;
	string material = "";
	ifstream file(fileName);
	int nv = 0, nvt = 0, nvn = 0;
	if (file.is_open())
	{
		//GLint size;
		string line;

		//getline(file, material);

		while (getline(file, line))
		{
			file >> line;

			if (line == "v")
			{
				file >> X;
				file >> Y;
				file >> Z;
				Vertices[0].push_back(X);
				Vertices[1].push_back(Y);
				Vertices[2].push_back(Z);
				nv++;

				
			}
			else if (line == "vt")
			{
				file >> XT;
				file >> YT;
				Text_Coords[0].push_back(XT);
				Text_Coords[1].push_back(YT);
				nvt++;
			}
			else if (line == "vn")
			{
				file >> XN;
				file >> YN;
				file >> ZN;
				Vertice_Normals[0].push_back(XN);
				Vertice_Normals[1].push_back(YN);
				Vertice_Normals[2].push_back(ZN);

				nvn++;
			}

		}
		int n = 0;

		/*while (n < Vertices[0].size())
		{
			cout << "Coordenada " << n << " :" << endl;
			cout << "X:" << Vertices[0].at(n) << " Y:" << Vertices[1].at(n) << " Z:" << Vertices[2].at(n) << endl;
			cout << "Xt:" << Text_Coords[0].at(n) << " Yt:" << Text_Coords[1].at(n) << endl;
			cout << "Xn:" << Vertice_Normals[0].at(n) << " Yn:" << Vertice_Normals[1].at(n) << " Zn:" << Vertice_Normals[2].at(n) << endl;
			n++;
		}*/
		file.close();

		model.modelTotalVertices = nv;
		model.modelTotalNormals = nvn;
		model.modelTotalTextures = nvt;
		model.modelTotalColors = nv;
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

	return model;
}
