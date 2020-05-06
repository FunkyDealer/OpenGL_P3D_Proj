#pragma once
#include "Geometry.h"

namespace Model_Viewer {

	class Model {

		public:
			GLfloat *vertices;
			GLfloat *colors;
			GLfloat *textures;
			GLfloat *normals;
			float modelTotalVertices;
			float modelTotalColors;
			float modelTotalTextures;
			float modelTotalNormals;

			Model(GLfloat &v, GLfloat &vc) {

				*vertices = v;
				*colors = vc;

			}

			Model() {

			}
	};

}