#pragma once
#include "Geometry.h"

namespace Model_Viewer {

	class Model {

		public:
			GLfloat *vertices;
			GLfloat *colors;

			Model(GLfloat &v, GLfloat &vc) {

				*vertices = v;
				*colors = vc;

			}

			Model() {

			}
	};

}