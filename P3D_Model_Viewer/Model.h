#pragma once
#include <GL\glew.h>
#include <GL\gl.h>
#include <string>
using namespace std;

namespace Model_Viewer {

	typedef struct {
		string name; //material name
		GLfloat *ka;
		GLfloat *kd;
		GLfloat *ks;
		GLfloat *Ns;
		GLfloat *Ni;
		string map; //Texture name
	} Material;

	class Model {

	private:
		string textureFile; //Texture file(in case of no material)
		string materialFile; //material File
		string file; //Model File/Name

	public:
		GLfloat *vertices; //Vertice Coordinates information
		GLfloat *colors; //Vertice Colors information
		GLfloat *textures; //vertice Texture Coordinates information
		GLfloat *normals; //vertice Normal coordinates information
		float modelTotalVertices; //Total of vertices
		float modelTotalColors; //total of color vertices
		float modelTotalTextures; //Total of texture vertice 
		float modelTotalNormals; //Total of normals
		Material material; //Model Material

		unsigned char *imageData; //Texture ImageData

		Model(GLfloat &v, GLfloat &vc) { //Create a model from just vertices and colors
			*vertices = v;
			*colors = vc;
		}

		Model(string file) {
			this->file = file;
		}

		Model() {
		}

		~Model() {
			//cout << "model has been destroyed" << endl;
		}

		void setTextureFile(string file) {			
			textureFile = file;
		}

		void setMaterialFile(string file) {
			materialFile = file;
		}

		string getTextureFile() {
			cout << "texture File returned: " << textureFile << endl;
			return textureFile;
		}

		string getMaterialFile() {
			return this->materialFile;
		}

		void setName(string name) {
			this->file = name;
		}

		string getName() {
			return file;
		}

	};


}