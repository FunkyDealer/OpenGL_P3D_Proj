#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm\glm.hpp>
#define GLFW_USE_DWM_SWAP_INTERVAL       
#include <GLFW\glfw3.h>
using namespace glm;

namespace Model_Viewer {

	class Camera {



	public:
		GLfloat zoom = 10.0f;
		GLfloat targetHeight = 0.0f;
		GLfloat camHeight = 0.0f;

		vec3 camLocation = vec3(0.0f, camHeight, zoom);
		vec3 camTarget = vec3(0.0f, targetHeight, 0.0f);
		vec3 camDirection = camTarget - camLocation;


		float rotX = 90, rotY = 45;
		vec2 MousePos, lastMP;
		bool canRotate = false;

		mat3 YPR = mat3(1);

		Camera() {

		}


		void update() {

			//Update Camera  Data
			if (canRotate)
			{
				float xoffset = MousePos.x - lastMP.x;
				float yoffset = lastMP.y - MousePos.y;

				lastMP = MousePos;

				rotX += xoffset * 0.7;
				rotY += yoffset * 0.7;

				if (rotY > 89)
				{
					rotY = 89;
				}
				else if (rotY < -89)
				{
					rotY = -89;
				}

				//vec3 dir;
				//
				//dir.x = cos(radians(rotX)) * cos(radians(rotY));
				//dir.z = sin(radians(rotX));
				//dir.y = sin(radians(rotY)) * cos(radians(rotY));

				mat3 Rotz = { {1,0,0},{0,1,0},{0,0,1} };
				mat3 Roty = { {cos(radians(rotX)),0,sin(radians(rotX))},{0,1,0},{-sin(radians(rotX)),0,cos(radians(rotX))} };
				mat3 Rotx = { {1,0,0},{0,cos(radians(rotY)),-sin(radians(rotY))},{0,sin(radians(rotY)),cos(radians(rotY))} };

				YPR = Rotz * Roty * Rotx;

				camLocation = vec3(0, targetHeight, zoom)*YPR;
			}
			else
			{
				camLocation = vec3(0, targetHeight, zoom) * YPR;
			}

			//cout << canRotate << rotX << rotY << endl;
			camTarget = vec3(0.0f, targetHeight, 0.0f);
			camDirection = camTarget - camLocation;
		}

		void KeyboardControl(int key, int mods) {
			switch (key) {
			case GLFW_KEY_UP:
				if (mods == GLFW_MOD_CONTROL) camHeight += 1.0f;			//Changes Camera Position(Height)
				else targetHeight += 1.0;									//Changes Camera's Target Position(Height)
				break;
			case GLFW_KEY_DOWN:
				if (mods == GLFW_MOD_CONTROL) camHeight -= 1.0f;			//Changes Camera Position(Height)
				else targetHeight -= 1.0f;									//Changes Camera's Target Position(Height)
				break;
			}
		}

		void MouseControl(bool mouse1Pressed, double xpos, double ypos) {
			if (mouse1Pressed) {

			}
			MousePos = vec2(xpos, ypos);
		}

		void ScrollControl(double xoffset, double yoffset) {
			if (yoffset == 1) zoom += fabs(zoom) * 0.1f;
			else if (yoffset == -1) zoom -= fabs(zoom) * 0.1f;
		}

		

	private:

	
		

	};

}