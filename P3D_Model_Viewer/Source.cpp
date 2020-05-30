#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include "stb_image.h"
#include <glm\gtc\matrix_inverse.hpp> // glm::inverseTranspose()

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
using namespace std;
#include <fstream>

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

#define GLEW_STATIC
#include <GL\glew.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>

#define GLFW_USE_DWM_SWAP_INTERVAL       
#include <GLFW\glfw3.h>

#include "LoadShaders.h"
#include "Camera.h"
#include "Model.h"


using namespace glm;
using namespace Model_Viewer;

const vec3 DEFAULT_DIR = vec3(1.0f, 1.0f, 0.0f);
const vec3 UP = vec3(0.0f, 1.0f, 0.0f);
const vec3 DOWN = vec3(0.0f, -1.0f, 0.0f);
const vec3 RIGHT = vec3(1.0f, 0.0f, 0.0f);
const vec3 LEFT = vec3(-1.0f, 1.0f, 0.0f);
const vec3 BACKWARD = vec3(0.0f, 0.0f, -1.0f);
const vec3 FORWARD = vec3(0.0f, 0.0f, 1.0f);

const mat4 IDENTITY = mat4(1.0f); //Indentity Matrix

//Source.cpp Functions
void init();
void display(GLFWwindow *window);
void InputManager(GLFWwindow* window, int key, int scancode, int action, int mods);
void modsInput(GLFWwindow* window, unsigned int codepoint, int mods);
void ScrollCallback(GLFWwindow * window, double xoffset, double yoffset);
void MouseClickCallBack(GLFWwindow* window, int button, int action, int mods);
void MousePosCallBack(GLFWwindow* window, double xpos, double ypos);
void InputManager(GLFWwindow* window, int key, int scancode, int action, int mods);
void setFullScreen(GLFWwindow* window);
void setWindowedScreen(GLFWwindow* window);
void switchLight(string light);

int screenWidth = 800;
int screenHeight = 600;
char title[20] = "Model Viewer";

//VAOS && VBOs
#define NumBuffers 4 // Vertices, Cores, Textures
int SceneTotalVAOs;
//GLuint VAOs[NumVAOs];
GLuint *VAOs;
GLuint Buffers[NumBuffers];

GLuint ShaderProgram;

mat4 LocalWorld, View, Projection;
mat3 NormalMatrix;  //Normal Matrix For Light

GLfloat ANGLE = 0.0f; //ANGLE OF MODEL
float rotateSpeed = 0.01f; //Speed at which it rotates

float nearPlane = 0.1f;
float farPlane = 100.f;

float aspectRatio = float(screenWidth) / float(screenHeight);

int geometry = 3;

GLuint SceneTotalVertices = 0;

bool mouse1Pressed = false;
bool ctrlPressed = false;
vec2 mouse1PressLocation = vec2(0, 0);


float deltaTime = 0;
float elapsedTime = 0;
float lastFrameTime = 0; //Time since last Frame

bool isRotating = false;

Camera camera;



int main() {


	GLFWwindow *window;

	if (!glfwInit()) return -1;

	GLFWmonitor* monitor = glfwGetPrimaryMonitor(); //gets monitor
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor); //Gets Video Settings

	cout << "w: " << videoMode->width << "h: " << videoMode->height << "@ " << videoMode->refreshRate << "fps" << endl;

	window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr); //Creates window with glfw


	if (!window) { //If window is null
		glfwTerminate(); //Terminate glfw //~bug preventing measure
		return -1;
	}

	glfwMakeContextCurrent(window); //make the context the window

	glewInit();
	glfwSwapInterval(1); //sets v-Sync

	camera = Camera();

	init();

	//Callback Functions
	glfwSetKeyCallback(window, InputManager); //Input with keys
	glfwSetCharModsCallback(window, modsInput); //Input with Mods
	glfwSetScrollCallback(window, ScrollCallback); //Input with mousewheel
	glfwSetMouseButtonCallback(window, MouseClickCallBack); //Input with Mouse
	glfwSetCursorPosCallback(window, MousePosCallBack); //Input with Mouse Position;

	mat4 projection = perspective(radians(45.0f), aspectRatio, nearPlane, farPlane);
	mat4 LocalWorld = mat4(1.0f); //Model World //identity matrix


	while (!glfwWindowShouldClose(window)) { //Indica pedido de fecho glfwSetWindowShouldClose(window, 1); //Pede para fechar
		

		display(window);


	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}



void init() {
	//--------------------------- Create arrays in RAM ---------------------------

	//Model model = Model(1);
	//Model model = Model(0);
	Model model = Model("Model/Iron_Man.xyz", &SceneTotalVertices, &SceneTotalVAOs);

	VAOs = (GLuint*)malloc(SceneTotalVAOs * sizeof(GLuint)); //VAOs Array

	cout << "Ended Model Creation" << endl;

	Projection = perspective(radians(45.0f), aspectRatio, nearPlane, farPlane);

	View = camera.viewMatrix;

	LocalWorld = IDENTITY;
	mat4 ModelView = View * LocalWorld;
	NormalMatrix = inverseTranspose(mat3(ModelView));

	mat4 ModelViewProjection = Projection * View * LocalWorld;

	// --------------------------- VAOs - Vertex Array Objects ---------------------------
	glGenVertexArrays(SceneTotalVAOs, VAOs); //Generate VAO

	for (int i = 0; i < SceneTotalVAOs; i++)
	{
		glBindVertexArray(VAOs[i]); //Bind VAO
	}	

	// --------------------------- VBOs - Vertex Buffer Objects ---------------------------
	glGenBuffers(NumBuffers, Buffers); //Generate NumBufffer names for VBOs

	for (int i = 0; i < NumBuffers; i++) { //For each Name of VBO
		
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]); //Bind VBO to buffer GL_ARRAY_BUFFER
		switch (i) {
		case 0:
			glBufferStorage(GL_ARRAY_BUFFER, SceneTotalVertices * 3 * sizeof(float), model.vertices, 0); //Initialize the VBO that's active
			break;
		case 1:	
			glBufferStorage(GL_ARRAY_BUFFER, SceneTotalVertices * 3 * sizeof(float), model.colors, 0); //Initialize the VBO that's active 	
			break;
		case 2:
			glBufferStorage(GL_ARRAY_BUFFER, SceneTotalVertices * 2 * sizeof(float), model.textures, 0); //Initialize the VBO that's active 
			break;
		case 3:
			glBufferStorage(GL_ARRAY_BUFFER, SceneTotalVertices * 3 * sizeof(float), model.normals, 0); //Initialize the VBO that's active 
		}
	}	

	//---------------------- Shaders ---------------------------
	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "Shader.vert" },
		{ GL_FRAGMENT_SHADER, "Shader.frag" },
		{ GL_NONE, NULL }
	};		

	ShaderProgram = LoadShaders(shaders);
	if (!ShaderProgram) exit(EXIT_FAILURE);
	glUseProgram(ShaderProgram);	

	// --------------------------- Connect Atributtes to Shaders ---------------------------
	// Get the Location of Attribute vPosition in Shader Program
	//GLint Coors_ID = glGetAttribLocation(shaderProgram, "vPosition"); // for versions older than 4.3
	GLint Coords_ID = glGetProgramResourceLocation(ShaderProgram, GL_PROGRAM_INPUT, "vPosition"); // for versions new or equal to 4.3

	// Get the Location of Attribute vColors in Shader Program
	//GLint Colors_ID = glGetAttribLocation(ShaderProgram, "vColors"); // for versions older than 4.3
	 GLint Colors_ID = glGetProgramResourceLocation(ShaderProgram, GL_PROGRAM_INPUT, "vColors"); // for versions new or equal to 4.3

	// Get the Location of Attribute vTextureCoors in Shader Program
	//GLint Colors_ID = glGetAttribLocation(ShaderProgram, "vTextureCoords"); // for versions older than 4.3
	GLint textureID = glGetProgramResourceLocation(ShaderProgram, GL_PROGRAM_INPUT, "vTextureCoords");

	//Get the Location of the vNormal in the Shader Program
	GLint normalId = glGetProgramResourceLocation(ShaderProgram, GL_PROGRAM_INPUT, "vNormal");	

	// Activate the VBO buffer[0]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);

	// connects the attribute 'vPosition' from shaders to the active VBO and VAO
	glVertexAttribPointer(Coords_ID, 3 /*3 elements per vertice*/, GL_FLOAT/*float type*/, GL_FALSE, 0, nullptr);	

	// Activate the VBO buffer[1]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[1]);
	//  connects the attribute 'vColors' from shaders to the active VBO and VAO
	glVertexAttribPointer(Colors_ID, 3 /*3 elements per vertice*/, GL_FLOAT/*float type*/, GL_FALSE, 0, nullptr);

	//Activate the VBO buffer[2]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[2]);
	//  connects the attribute 'vTextureCoors' from shaders to the active VBO and VAO
	glVertexAttribPointer(textureID, 2 /*2 elements per vertice*/, GL_FLOAT/*float type*/, GL_FALSE, 0, nullptr);

	//Activate the VBO buffer[2]
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[3]);
	//  connects the attribute 'vNormal' from shaders to the active VBO and VAO
	glVertexAttribPointer(normalId, 3 /*3 elements per vertice*/, GL_FLOAT/*float type*/, GL_FALSE, 0, nullptr);

	glEnableVertexAttribArray(Coords_ID); //Activate the Coordenate Attribute for the active VAO
	glEnableVertexAttribArray(Colors_ID); //Activate the Color Attribute for the Active VAO
	glEnableVertexAttribArray(textureID); //Activate the Texture Attribute for the Active VAO
	glEnableVertexAttribArray(normalId); //Activate the Normal Attribute for the Active VAO
	
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "time"), glfwGetTime());
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "deforming"), 0);

	//points the unit of texture to connect to the sampler TexSampler
	GLint locationTexSampler = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "TexSampler");
	glProgramUniform1i(ShaderProgram, locationTexSampler, 0 /* Texture Unit #0 */);

	// ****************************************************
	// Uniforms
	// ****************************************************	
	
	// Atribui valor ao uniform Model
	GLint modelId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "Model");
	glProgramUniformMatrix4fv(ShaderProgram, modelId, 1, GL_FALSE, glm::value_ptr(LocalWorld));
	// Atribui valor ao uniform View
	GLint viewId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "View");
	glProgramUniformMatrix4fv(ShaderProgram, viewId, 1, GL_FALSE, glm::value_ptr(View));
	// Atribui valor ao uniform ModelView
	GLint modelViewId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "ModelView");
	glProgramUniformMatrix4fv(ShaderProgram, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
	// Atribui valor ao uniform Projection
	GLint projectionId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "Projection");
	glProgramUniformMatrix4fv(ShaderProgram, projectionId, 1, GL_FALSE, glm::value_ptr(Projection));
	// Atribui valor ao uniform NormalMatrix
	GLint normalViewId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "NormalMatrix");
	glProgramUniformMatrix3fv(ShaderProgram, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

	// Fonte de luz ambiente global
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "ambientLight.ambient"), 1, value_ptr(vec3(0.4, 0.4, 0.4)));

	// Fonte de luz direcional
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "directionalLight.direction"), 1, value_ptr(vec3(1.0, 0.0, 0.0)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "directionalLight.ambient"), 1, value_ptr(vec3(0.2, 0.2, 0.2)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "directionalLight.diffuse"), 1, value_ptr(vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "directionalLight.specular"), 1, value_ptr(vec3(1.0, 1.0, 1.0)));

	//// Fonte de luz pontual #1
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[0].position"), 1, value_ptr(vec3(0.0, 0.0, 5.0)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[0].ambient"), 1, value_ptr(vec3(0.1, 0.1, 0.1)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[0].diffuse"), 1, value_ptr(vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[0].specular"), 1, value_ptr(vec3(1.0, 1.0, 1.0)));
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[0].constant"), 1.0f);
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[0].linear"), 0.06f);
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[0].quadratic"), 0.02f);

	//// Fonte de luz pontual #2
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[1].position"), 1, value_ptr(vec3(-2.0, 2.0, 5.0)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[1].ambient"), 1, value_ptr(vec3(0.1, 0.1, 0.1)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[1].diffuse"), 1, value_ptr(vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[1].specular"), 1, value_ptr(vec3(1.0, 1.0, 1.0)));
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[1].constant"), 1.0f);
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[1].linear"), 0.06f);
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointLight[1].quadratic"), 0.02f);

	//// Fonte de luz Conica
	//glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLightt.position"), 1, value_ptr(vec3(-2.0, 2.0, 5.0)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.position"), 1, value_ptr( camera.position ));
	//glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLightt.direction"), 1, value_ptr(vec3(0.2, 0.2, 0.2)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.ambient"), 1, value_ptr(vec3(0.1, 0.1, 0.1)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.diffuse"), 1, value_ptr(vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.specular"), 1, value_ptr(vec3(1.0, 1.0, 1.0)));
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.constant"), 1.0f);
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.linear"), 0.06f);
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.quadratic"), 0.02f);
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.direction"), 1, value_ptr( camera.direction ));
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.spotCutoffAngle"), 5.0f);
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.spotExponent"), 2.0f);

	//Material
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "material.emissive"), 1, value_ptr(vec3(0.0, 0.0, 0.0)));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "material.ambient"), 1, value_ptr(model.material.ambient));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "material.diffuse"), 1, value_ptr(model.material.difuse));
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "material.specular"), 1, value_ptr(model.material.specular));
	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "material.shininess"), model.material.specular_Exponent);

	glProgramUniform1i(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "ambientSwitch"), 1);
	glProgramUniform1i(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "directionalSwitch"), 1);
	glProgramUniform1i(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "pointSwitch"), 1);
	glProgramUniform1i(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "coneSwitch"), 1);		

	glViewport(0, 0, screenWidth, screenHeight); //Define viewport Window
	
}



void display(GLFWwindow *window) {
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f }; //Black Color
	static const float white[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //White Color
	static const float grey[] = { 0.4f, 0.4f, 0.4f, 1.0f }; //Grey Color

	elapsedTime = glfwGetTime();
	float currentFrameTime = elapsedTime;
	deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	//cout << "deltatime " << deltaTime << endl;

	//glPolygonMode(GL_FRONT_AND_BACK, /*GL_FILL*/  GL_LINE /*GL_POINT*/);
	//glEnable(GL_LINE_SMOOTH); //activates antialiasing
	//glLineWidth(2.0f); //defines the line width
	//glPointSize(5.0f);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND); // Para que o GL_LINE_SMOOTH tenha efeito
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	float color[4] = { 0,0,0,0 };

	glClearBufferfv(GL_COLOR, 0, color);
	glClear(GL_DEPTH_BUFFER_BIT);

	glClearBufferfv(GL_COLOR, 0, grey); //Clears screen all black

	// Update Uniform data

	camera.update();

	View = camera.viewMatrix; //View Matrix - Camera

	Projection = perspective(radians(45.0f), aspectRatio, nearPlane, farPlane); //Projection Matrix

	if (isRotating) LocalWorld = rotate(IDENTITY, ANGLE += rotateSpeed, normalize(UP)); //Rotate Model Automatically

	mat4 ModelView = View * LocalWorld;

	mat4 ModelViewProjection = Projection * View * LocalWorld;

	NormalMatrix = inverseTranspose(mat3(ModelView));
	
	// Attributes value to the uniform model
	GLint modelId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "Model");
	glProgramUniformMatrix4fv(ShaderProgram, modelId, 1, GL_FALSE, value_ptr(LocalWorld));

	GLint viewId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "View");
	glProgramUniformMatrix4fv(ShaderProgram, viewId, 1, GL_FALSE, value_ptr(View));

	GLint modelViewId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "ModelView");
	glProgramUniformMatrix4fv(ShaderProgram, modelViewId, 1, GL_FALSE, value_ptr(ModelView));
	
	GLint normalViewId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "NormalMatrix");
	glProgramUniformMatrix3fv(ShaderProgram, normalViewId, 1, GL_FALSE, value_ptr(NormalMatrix));	

	GLint projectionId = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "Projection");
	glProgramUniformMatrix4fv(ShaderProgram, projectionId, 1, GL_FALSE, glm::value_ptr(Projection));

	glProgramUniform1f(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "time"), elapsedTime);
	glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.position"), 1, value_ptr(camera.position));
	//glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.direction"), 1, value_ptr(normalize(camera.direction)));
	//glProgramUniform3fv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "spotLight.direction"), 1, value_ptr(camera.direction));

	//cout << "Camera X: " << camera.camLocation.x << "Camera Y: " << camera.camLocation.y << "Camera Z: " << camera.camLocation.z << endl;

	// Activates the VAOs
	for (int i = 0; i < SceneTotalVAOs; i++)
	{
		glBindVertexArray(VAOs[i]); //Bind VAO
	}


	//Draws Primitives GL_TRIANGLES using active VAOs
	glDrawArrays(GL_TRIANGLES, 0, SceneTotalVertices);	

	glfwSwapBuffers(window); //Buffers
	glfwPollEvents(); //Events
}

//-----------------------------------Inputs bellow -----------------------------------------
void InputManager(GLFWwindow* window, int key, int scancode, int action, int mods) { //Keys Input manager
	if (action == GLFW_PRESS) {
		camera.KeyboardControl(key, mods); //Camera's keyboard Controls
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			cout << "pressed esc" << endl;
			glfwSetWindowShouldClose(window, 1); //asks to close
			break;
		case GLFW_KEY_1: //Activate/Deactivate Ambient Light
			switchLight("ambientSwitch");	
			break;
		case GLFW_KEY_2: //Activate/Deactivate Directional Light
			switchLight("directionalSwitch");
			break;
		case GLFW_KEY_3: //Activate/Deactivate Point Light
			switchLight("pointSwitch");			
			break;
		case GLFW_KEY_4: //Activate/Deactivate Cone Light
			switchLight("coneSwitch");
			break;
		case GLFW_KEY_5:
			setFullScreen(window);					//Sets FullScreen
			break;
		case GLFW_KEY_6:
			setWindowedScreen(window);				//Sets Windowed Mode
			break;
		case GLFW_KEY_F1: //Disable Deformation
			cout << "Stopped Deformations" << endl;
			glProgramUniform1i(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "deforming"), 0);
			break;
		case GLFW_KEY_F2: //Enable Deformation
			cout << "Model Ripple Deformation" << endl;
			glProgramUniform1i(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "deforming"), 1);
			break;
		case GLFW_KEY_F3: //Enable Deformation
			cout << "Model Expansion Deformation" << endl;
			glProgramUniform1i(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "deforming"), 2);
			break;
		case GLFW_KEY_R:
			if (isRotating) isRotating = false;
			else isRotating = true;
			break;
		}
	}
}

void switchLight(string light) {
	int value;

	const char *lightC = light.c_str();

	glGetUniformiv(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, lightC), &value);
	if (value == 1) {
		value = 0;
		cout << lightC <<" is now off" << endl;
	}
	else {
		value = 1;
		cout << lightC << " is now on" << endl;
	}
	glProgramUniform1i(ShaderProgram, glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, lightC), value);
}

void modsInput(GLFWwindow* window, unsigned int codepoint, int mods) { //Mods Inputmanager
	if (codepoint == 'a' && mods == GLFW_MOD_SHIFT) {
		cout << "shift + a" << endl;
	}	
}


void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset) { //Scrolling CallBack
	camera.ScrollControl(xoffset, yoffset);
}

void MouseClickCallBack(GLFWwindow* window, int button, int action, int mods) { //On mouse Click Call Back
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		//cout << "mouse 1 pressed" << endl;
		mouse1Pressed = true;

		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);

		mouse1PressLocation = vec2(x, y);
		camera.lastMousePos = camera.mousePos;
		camera.canRotate = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
		//cout << "mouse 1 released" << endl;
		mouse1Pressed = false;
		mouse1PressLocation = vec2(0, 0);
		camera.canRotate = false;
	}
}

void MousePosCallBack(GLFWwindow* window, double xpos, double ypos) { //Mouse Position CallBack - Gets Mouse Position every frame
	camera.MouseControl(mouse1Pressed, xpos, ypos);
}


void setFullScreen(GLFWwindow* window) { //Set Full screen
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

	int fullWidth = videoMode->width;
	int fullHeight = videoMode->height;

	cout << "width: " << fullWidth << " heigth " << fullHeight << " " << videoMode->refreshRate << endl;

	aspectRatio = float(fullWidth) / float(fullHeight);

	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
	glfwSetWindowMonitor(window, monitor, 0, 0, fullWidth, fullHeight, videoMode->refreshRate);
	glfwGetFramebufferSize(window, &fullWidth, &fullHeight);
	glViewport(0, 0, fullWidth, fullHeight);
}

void setWindowedScreen(GLFWwindow* window) { //Set Windowed
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

	aspectRatio = float(screenWidth) / float(screenHeight);

	glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
	glfwSetWindowMonitor(window, NULL, videoMode->width / 3, videoMode->height / 3, screenWidth, screenHeight, videoMode->refreshRate);
	glfwGetFramebufferSize(window, &screenHeight, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);
}






