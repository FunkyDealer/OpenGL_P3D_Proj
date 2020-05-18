#include "Geometry.h"
#include "LoadShaders.h"

#include "stb_image.h"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


int screenWidth = 800;
int screenHeight = 600;
char title[20] = "Model Viewer";

//VAOS && VBOs
#define NumVAOs 1
#define NumBuffers 3 // Vertices, Cores, Textures
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

GLuint ShaderProgram;

mat4 LocalWorld, View, Projection;

GLfloat zoom = 10.0f;
GLfloat targetHeight = 0.0f;
GLfloat camHeight = 0.0f;
GLfloat ANGLE = 0.0f; //ANGLE OF MODEL
float rotateSpeed = 0.01f; //Speed at which it rotates

float nearPlane = 0.1f;
float farPlane = 100.f;

float aspectRatio = float(screenWidth) / float(screenHeight);

int geometry = 3;

GLuint numVertices = 0;

bool mouse1Pressed = false;
bool ctrlPressed = false;
vec2 mouse1PressLocation = vec2(0, 0);

vec3 camLocation = vec3(0.0f, camHeight, zoom);
vec3 camTarget = vec3(0.0f, targetHeight, 0.0f);
vec3 camDirection = camTarget - camLocation;
bool isRotating = true;

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

	init();

	//Callback Functions
	glfwSetKeyCallback(window, InputManager); //Input with keys
	glfwSetScrollCallback(window, scrollCallback); //Input with mousewheel
	glfwSetCharModsCallback(window, modsInput); //Input with Mods
	glfwSetMouseButtonCallback(window, mouseClickCallBack); //Input with Mouse
	glfwSetCursorPosCallback(window, mousePosCallBack); //Input with Mouse Position;

	mat4 projection = perspective(radians(45.0f), aspectRatio, nearPlane, farPlane);
	mat4 LocalWorld = mat4(1.0f); //Model World //identity matrix

	while (!glfwWindowShouldClose(window)) { //Indica pedido de fecho glfwSetWindowShouldClose(window, 1); //Pede para fechar


		display();

		glfwSwapBuffers(window); //Buffers
		glfwPollEvents(); //Events
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}



void init() {
	//--------------------------- Create arrays in RAM ---------------------------
	Model model;

	switch (geometry) {
	case 1: //Triangle
		model = LoadTriangle();
		numVertices += model.modelTotalVertices;
		break;
	case 2: //Cube
		model = LoadCube(); //Load Cube Vertices
		numVertices += model.modelTotalVertices;
		break;
	case 3: //Model
		model = LoadXYZModel("Model/Iron_Man.xyz");
		numVertices += model.modelTotalVertices;
		load_Model_texture(model);
		break;
	default:
		throw "Invalid geometry selected";
		break;
	}
	

	cout << "Ended Model Creation" << endl;

	Projection = perspective(radians(45.0f), aspectRatio, nearPlane, farPlane);

	View = glm::lookAt(
		vec3(0.0f, 0.0f, zoom),	// eye (posicao da camara). Cmaara na posicao (x=0, y=0, z=5). Nota que movo o mundo em sentido contrario.
		BACKWARD,	// center (para onde esta a "olhar")
		UP		// up
	);

	LocalWorld = IDENTITY;
	mat4 ModelViewProjection = Projection * View * LocalWorld;

	// --------------------------- VAOs - Vertex Array Objects ---------------------------
	glGenVertexArrays(NumVAOs, VAOs); //Generate VAO
	glBindVertexArray(VAOs[0]); //Bind VAO "0"

	// --------------------------- VBOs - Vertex Buffer Objects ---------------------------
	glGenBuffers(NumBuffers, Buffers); //Generate NumBufffer names for VBOs

	for (int i = 0; i < NumBuffers; i++) { //For each Name of VBO
		
		glBindBuffer(GL_ARRAY_BUFFER, Buffers[i]); //Bind VBO to buffer GL_ARRAY_BUFFER
		switch (i) {
		case 0:
			glBufferStorage(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), model.vertices, 0); //Initialize the VBO that's active
			break;
		case 1:	
			glBufferStorage(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(float), model.colors, 0); //Initialize the VBO that's active 	
			break;
		case 2:
			glBufferStorage(GL_ARRAY_BUFFER, numVertices * 2 * sizeof(float), model.textures, 0); //Initialize the VBO that's active 
			break;
		}
	}

	//---------------------- Shaders ---------------------------
	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "triangles.vert" },
		{ GL_FRAGMENT_SHADER, "triangles.frag" },
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

	// put a valor in uniform MVP
	GLint mvp_ID = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "MVP");
	glProgramUniformMatrix4fv(ShaderProgram, mvp_ID, 1, GL_FALSE, value_ptr(ModelViewProjection));

	//glBindVertexArray(VAOs[0]); // it's not necessary to bind the VAO, as it is already active in opengl context.

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


	glEnableVertexAttribArray(Coords_ID); //Activate the Coordenate Attribute for the active VAO

	glEnableVertexAttribArray(Colors_ID); //Activate the Color Attribute for the Active VAO

	glEnableVertexAttribArray(textureID); //Activate the Texture Attribute for the Active VAO
	
	//points the unit of texture to connect to the sampler TexSampler
	//glUniform1i(glGetUniformLocation(programa, "TexSampler"), 0 /* Texture Unit #0 */);
	GLint locationTexSampler = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "TexSampler");
	glProgramUniform1i(ShaderProgram, locationTexSampler, 0 /* Texture Unit #0 */);

	glViewport(0, 0, screenWidth, screenHeight); //Define viewport Window

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

	unsigned char *imageData = stbi_load( cstr, &width, &height, &nChannels, 0);
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

void display() {
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f }; //Black Color

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL  /*GL_LINE*/ /*GL_POINT*/);
	//glEnable(GL_LINE_SMOOTH); //activates antialiasing
	//glLineWidth(0.5f); //defines the line width
	//glPointSize(5.0f);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND); // Para que o GL_LINE_SMOOTH tenha efeito
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	float color[4] = { 0,0,0,0 };

	glClearBufferfv(GL_COLOR, 0, color);
	glClear(GL_DEPTH_BUFFER_BIT);

	glClearBufferfv(GL_COLOR, 0, black); //Clears screen all black

	// Update Uniform data

	//Update Camera  Data
	camLocation = vec3(0.0f, camHeight, zoom);
	camTarget = vec3(0.0f, targetHeight, 0.0f);
	camDirection = camTarget - camLocation;

	//View Matrix
	View = lookAt(
		camLocation,	// Camera Position in the World
		camDirection,	// Direction at which the camera Is Pointing
		UP		// Camera Up Vector
	);

	Projection = perspective(radians(45.0f), aspectRatio, nearPlane, farPlane); //Projection Matrix

	if (isRotating) LocalWorld = rotate(IDENTITY, ANGLE += rotateSpeed, normalize(UP)); //Rotate Model Automatically

	mat4 ModelViewProjection = Projection * View * LocalWorld;

	GLint mvp_ID = glGetProgramResourceLocation(ShaderProgram, GL_UNIFORM, "MVP");
	glProgramUniformMatrix4fv(ShaderProgram, mvp_ID, 1, GL_FALSE, glm::value_ptr(ModelViewProjection));

	// Activates the VAOs
	glBindVertexArray(VAOs[0]);

	//Draws Primitives GL_TRIANGLES using active VAOs
	glDrawArrays(GL_TRIANGLES, 0, numVertices);
}


//-----------------------------------Inputs bellow -----------------------------------------

void InputManager(GLFWwindow* window, int key, int scancode, int action, int mods) { //Keys Input manager
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			cout << "pressed esc" << endl;
			glfwSetWindowShouldClose(window, 1); //asks to close
			break;
		case GLFW_KEY_1:
			setFullScreen(window);					//Sets FullScreen
			break;
		case GLFW_KEY_2:
			setWindowedScreen(window);				//Sets Windowed Mode
			break;
		case GLFW_KEY_LEFT:
			
			break;
		case GLFW_KEY_RIGHT:
			
			break;
		case GLFW_KEY_UP:												
			if (mods == GLFW_MOD_CONTROL) camHeight += 1.0f;			//Changes Camera Position(Height)
			else targetHeight += 1.0;									//Changes Camera's Target Position(Height)
			break;
		case GLFW_KEY_DOWN:
			if (mods == GLFW_MOD_CONTROL) camHeight -= 1.0f;			//Changes Camera Position(Height)
			else targetHeight -= 1.0f;									//Changes Camera's Target Position(Height)
			break;
		case GLFW_KEY_R:
			if (isRotating) isRotating = false;
			else isRotating = true;
		}
	}
}

void modsInput(GLFWwindow* window, unsigned int codepoint, int mods) { //Mods Inputmanager

	if (codepoint == 'a' && mods == GLFW_MOD_SHIFT) {
		cout << "shift + a" << endl;
	}	
}

void setFullScreen(GLFWwindow* window) { //Set Full screen
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

	int fullWidth = videoMode->width;
	int fullHeight = videoMode->height;

	cout << "width: " << fullWidth << " heigth " << fullHeight << endl;

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

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) { //Scrolling CallBack
	if (yoffset == 1) zoom += fabs(zoom) * 0.1f;
	else if (yoffset == -1) zoom -= fabs(zoom) * 0.1f;
}

void mousePosCallBack(GLFWwindow* window, double xpos, double ypos) { //Mouse Position CallBack - Gets Mouse Position every frame
	if (mouse1Pressed) {
		//cout << "X: " << xpos << " Y: " << ypos << endl;
		vec2 currentLoc = vec2(xpos, ypos);
		vec2 dir = currentLoc - mouse1PressLocation;
	}
}

void mouseClickCallBack(GLFWwindow* window, int button, int action, int mods) { //On mouse Click Call Back
	if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
		//cout << "mouse 1 pressed" << endl;
		mouse1Pressed = true;

		double x;
		double y;
		glfwGetCursorPos(window, &x, &y);

		mouse1PressLocation = vec2(x, y);

	}
	else if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
		//cout << "mouse 1 released" << endl;
		mouse1Pressed = false;
		mouse1PressLocation = vec2(0, 0);
	}

}

