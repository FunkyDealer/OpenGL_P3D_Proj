#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm\glm.hpp>
#define GLFW_USE_DWM_SWAP_INTERVAL       
#include <GLFW\glfw3.h>
#include <glm/gtc/quaternion.hpp>
using namespace glm;

namespace Model_Viewer {

	class Camera {
	private:
		float sensivity = 0.1f;
		quat rotationQuaternion;
		

	public:
		GLfloat zoom = 10.0f;
		GLfloat targetHeight = 0.0f;
		GLfloat camHeight = 0.0f;

		vec3 position;
		vec3 target;
		vec3 direction;
		vec3 up;
		vec3 forward;


		float rotX = 0, rotY = 0;
		vec2 mousePos, lastMousePos;
		bool canRotate = false;

		mat3 YPR = mat3(1);

		mat4 viewMatrix;
		

		Camera() {

			zoom = 10.0f;
			position = vec3(0.0f, camHeight, zoom);
			target = vec3(0.0f, 0.0f, 0.0f);
			direction = normalize(target - position);
			up = vec3(0.0f, 1.0f, 0.0f);
			forward = direction;

			viewMatrix = lookAt(
				position,	// Camera Position in the World
				direction,	// Direction at which the camera Is Pointing
				up		// Camera Up Vector
			);


			cout << position.x << " " << position.y << " " << position.z << endl;
		}

		Camera(vec3 position, vec3 target, vec3 up) {
		this->position = position;
		this->target = target;
		this->direction = normalize(target - position);
		this->up = up;
		this->forward = direction;

		viewMatrix = lookAt(
			position,	// Camera Position in the World
			direction,	// Direction at which the camera Is Pointing
			up		// Camera Up Vector
		);
		}

		Camera(vec3 position, vec3 target, vec3 up, vec3 forward) {

			this->position = position;
			this->target = target;
			this->direction = forward;
			this->up = up;
			this->forward = forward;

			viewMatrix = lookAt(
				position,	// Camera Position in the World
				direction,	// Direction at which the camera Is Pointing
				up		// Camera Up Vector
			);

		}


		void update() {

			//Update Camera  Data
			if (canRotate)
			{
				float xoffset = mousePos.x - lastMousePos.x;
				float yoffset = lastMousePos.y - mousePos.y;

				lastMousePos = mousePos;

				xoffset *= sensivity;
				yoffset *= sensivity;
/*
				rotX += xoffset;
				rotY += yoffset;

				if (rotY > 89.0f)
					rotY = 89.0f;
				else if (rotY < -89.0f)
					rotY = -89.0f;	*/					

				//vec3 dir;
				//
				//dir.x = cos(radians(rotX)) * cos(radians(rotY));
				//dir.z = sin(radians(rotX));
				//dir.y = sin(radians(rotY)) * cos(radians(rotY));

			//	mat3 Rotz = mat3(1);
			//	mat3 Roty = { {cos(radians(rotX)),0,sin(radians(rotX))},{0,1,0},{-sin(radians(rotX)),0,cos(radians(rotX))} };
			//	mat3 Rotx = { {1,0,0},{0,cos(radians(rotY)),-sin(radians(rotY))},{0,sin(radians(rotY)),cos(radians(rotY))} };				

				//YPR = Rotz * Roty * Rotx;
				//YPR = Rotx;
				//YPR = Roty;
				vec3 verticalAxis = cross(forward, up);
				vec3 horizontalAxis = up;
				

				position = position * AngleAxis(yoffset, verticalAxis);
				position = position * AngleAxis(xoffset, horizontalAxis);

				//position = position * YPR;
			}

			direction = normalize(target - position);
			forward = direction;

			//cout << position.x << " " << position.y << " " << position.z << endl;

			viewMatrix = lookAt(
				position,	// Camera Position in the World
				direction,	// Direction at which the camera Is Pointing
				up		// Camera Up Vector
			);
		}

		void KeyboardControl(int key, int mods) {
			switch (key) {
			case GLFW_KEY_UP:
				if (mods == GLFW_MOD_CONTROL) position += up;			//Changes Camera Position(Height)
				else { target += vec3(0.0, 1.0, 0.0); }								//Changes Camera's Target Position(Height)

				break;
			case GLFW_KEY_DOWN:
				if (mods == GLFW_MOD_CONTROL) position -= up;			//Changes Camera Position(Height)
				else { target -= vec3(0.0, 1.0, 0.0); }				//Changes Camera's Target Position(Height)

				break;
			}
		}

		void MouseControl(bool mouse1Pressed, double xpos, double ypos) {
			if (mouse1Pressed) {
				


			}
			mousePos = vec2(xpos, ypos);
			
		}

		void ScrollControl(double xoffset, double yoffset) {

			if (yoffset == 1) {
				float distance = length(target - position);
					if (distance > 1) {
						position += this->forward;
					}

			//	cout << position.x << " " << position.y << " " << position.z << endl;
			}
			else if (yoffset == -1) {
				position -= this->forward;
			//	cout << position.x << " " << position.y << " " << position.z << endl;
			}
		}

		

	private:

		quat AngleAxis(float angle, vec3& axis) {
			vec3 vn = normalize(axis); //Normalized Axis
			angle = radians(angle); //angle to radians
			angle *= 0.5f;
			float sinAngle = sin(angle);

			return quat(cos(angle), vn.x * sinAngle, vn.y * sinAngle, vn.z * sinAngle);
		}
		

	};

}