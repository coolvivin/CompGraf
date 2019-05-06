#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 11.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
float range = 0.0f;
float spotAngle = 0.0f;
float rot = 0.0f;
int i, j;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,  0.0f, -0.0f),
	glm::vec3(0.0f,  0.0f, -0.0f),
	glm::vec3(0.0f,  0.0f, -0.0f),
	glm::vec3(0.0f,  0.0f, -0.0f)
};

glm::vec3 LightP1;
bool circuito = false;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "EXTERIOR", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// OpenGL options
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	Model Mymodel_1((char*)"Models/Proyecto/Tree_Conifer_1.obj");
	Model nano((char*)"Models/Proyecto/nanosuit/nanosuit.obj");
	Model Mymodel_2((char*)"Models/Proyecto/window_big.obj");
	

	// Build and compile our shader program


	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] =
	{
		// Positions            // Normals              // Texture Coords //modo de dibujo hacia la derecha
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,  	1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,    1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,

		//triangulo
		-0.5f,  0.0f, 0.0f,    0.0f,  0.0f,  -1.0f,     0.0f,  0.0f,
		0.5f,  0.0f, 0.0f,     0.0f,  0.0f,  -1.0f,     1.0f,  0.0f,
		0.0f,  0.5f,  0.0f,    0.0f, 0.0f,   -1.0f,     1.0f,  1.0f
	};

	GLuint indices[] =
	{
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35,
		36,37,38
	};

	// Positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); // Note that we skip over the other data in our buffer object (we don't need the normals/textures, only positions).
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// Load textures
	GLuint texture1, texture2, texture3, texture4, texture5, texture6, texture7, texture8, texture9, texture10, texture11;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);
	glGenTextures(1, &texture3);
	glGenTextures(1, &texture4);
	glGenTextures(1, &texture5);
	glGenTextures(1, &texture6);
	glGenTextures(1, &texture7);
	glGenTextures(1, &texture8);
	glGenTextures(1, &texture9);
	glGenTextures(1, &texture10);
	glGenTextures(1, &texture11);

	int textureWidth, textureHeight, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *image;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	// TEXTURA 1
	image = stbi_load("images/concreto.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 1" << std::endl;
	}
	stbi_image_free(image);

	// TEXTURA 2
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/pared.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 2" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);


	// TEXTURA 3
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/rocas.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture3);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 3" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// TEXTURA 4
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/pared.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture4);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 4" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// TEXTURA 5
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/hojas.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture5);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 5" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// TEXTURA 6
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/azul.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture6);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 6" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// TEXTURA 7
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/AMARILLO.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture7);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 7" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// TEXTURA 8
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/PUERTA.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture8);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 8" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// TEXTURA 9
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/ventana2.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture9);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 9" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// TEXTURA 10
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/cr.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture10);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 10" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);
	////Textura 11 de la ventana
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	//image = stbi_load("images/ventana2.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	//glBindTexture(GL_TEXTURE_2D, texture11);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//glGenerateMipmap(GL_TEXTURE_2D);
	//if (image)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "No carga la textura 11!!!!!" << std::endl;
	//}
	//stbi_image_free(image);

	//glBindTexture(GL_TEXTURE_2D, 0);


	// Specular map
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	image = stbi_load("images/ADOQUIN2.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop

	float NanoZ = 0.0, NanoX = 0.0;
	float rotationNano = 0.0;
	int  cuartoVuelta = 0, direcionNano = 0;
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		// == ==========================
		// Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		// the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		// by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		// by using 'Uniform buffer objects', but that is something we discuss in the 'Advanced GLSL' tutorial.
		// == ==========================
		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f);

		// Point light 1
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), LightP1.x, LightP1.y, LightP1.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), LightP1.x, LightP1.y, LightP1.z);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 1.0f, 0.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 0.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 1.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));



		//EDIFICIO
		////PISO CONCRETO////

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAO);
		glm::mat4 model(1);
		model = glm::mat4(1);
		//model = glm::translate(model, lightPos);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(80.0f, 0.2f, 80.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//derecha
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 20.5F, 14.5f));
		model = glm::scale(model, glm::vec3(13.0f, 39.0, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//derecha-derecha
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(31.5f, 21.9f, -10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(49.0f, 41.9f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//derecha-izquierda
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(18.5f, 21.9f, -1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 41.9f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//izquierda-derecha
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.5f, 21.9f, -1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 41.9f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//izquierda-IZQUIERDA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-31.5f, 21.9f, -10.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(49.0f, 41.9f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////IZQUIERDA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 20.5F, 14.5f));
		model = glm::scale(model, glm::vec3(13.0f, 39.0, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//IZQUIERDA RAYADO
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 5.1f, 14.8f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//IZQUIERDA RAYADo 2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 15.0f, 14.8f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		 //IZQUIERDARAYADo 3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 25.0f, 14.8f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//IZQUIERDA RAYADo 4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 35.0f, 14.8f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////VENTANAS-IZQUIERDA-H5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 42.0f, 14.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////VENTANAS-IZQUIERDA-H4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 40.0f, 14.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////VENTANAS-IZQUIERDA-H3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 30.0f, 14.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////VENTANAS-IZQUIERDA-H2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 20.0f, 14.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		////VENTANAS-IZQUIERDAH
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 10.0f, 14.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//techo derecha
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		//model = glm::translate(model, lightPos);
		model = glm::translate(model, glm::vec3(25.0f, 42.85f, -10.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(13.0f, 49.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//techo IZQUIERDA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-25.0f, 42.85f, -10.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(13.0f, 49.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//techo enmedio
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.0f, 42.85f, -25.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(37.0f, 18.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//atras
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.0f, 22.0f, -34.5f));
		model = glm::scale(model, glm::vec3(63.0f, 41.7F, 0.0f));
		//model = glm::translate(model, glm::vec3(25.0f, 20.5F, 14.5f));
		//model = glm::scale(model, glm::vec3(13.0f, 39.0, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//enfrente
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.0f, 21.9f, -16.5f));
		model = glm::scale(model, glm::vec3(37.0f, 41.9, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-FRONTALES
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.0f, 22.0f, -16.5f));
		model = glm::scale(model, glm::vec3(2.0f, 42.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(+7.0f, 22.0f, -16.5f));
		model = glm::scale(model, glm::vec3(2.0f, 42.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-FRONTALES-H
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, -16.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(37.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-FRONTALES-H2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 20.0f, -16.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(37.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-FRONTALES-H3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 30.0f, -16.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(37.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-FRONTALES-H4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 40.0f, -16.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(37.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-FRONTALES-H5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 42.0f, -16.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(37.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-IZQUIERDA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.5f, 22.0f, -15.4f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 42.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/*model = glm::translate(model, glm::vec3(-25.0f, 20.5F, 14.5f));
		model = glm::scale(model, glm::vec3(13.0f, 39.0, 0.0f));*/

		//VENTANAS IZQUIERDA ENMEDIO
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.5f, 22.0f, 0.1f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 42.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS IZQUIERDA ENMEDIO H1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.5f, 10.1f, -0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.5f, 2.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS IZQUIERDA ENMEDIO H2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.5f, 20.0f, -0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.5f, 2.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//VENTANAS IZQUIERDA ENMEDIO H3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.5f, 30.0f, -0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.5f, 2.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//VENTANAS IZQUIERDA ENMEDIO H4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.5f, 40.0f, -0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.5f, 2.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS IZQUIERDA ENMEDIO H5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.5f, 42.0f, -0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.5f, 2.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS DERECHA ENMEDIO H5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(+18.5f, 42.0f, -0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.5f, 2.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS DERECHA ENMEDIO H4
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(+18.5f, 40.0f, -0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.5f, 2.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS derecha ENMEDIO H1
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(+18.5f, 10.1f, -0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.5f, 2.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS DERECHA ENMEDIO H2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(+18.5f, 20.0f, -0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.5f, 2.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//VENTANAS DERECHA ENMEDIO H3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(+18.5f, 30.0f, -0.8f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(27.5f, 2.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-18.5f, 22.0f, 14.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f,42.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-31.5f, 22.0f, 14.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 42.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Ventanas Derecha
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(18.5f, 22.0f, -15.4f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 42.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(+18.5f, 22.0f, 0.1f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 42.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(+18.5f, 22.0f, 14.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 42.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(+31.5f, 22.0f, 14.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 42.0, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-DERECHA-H5
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 42.0f, 14.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-DERECHA-H4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 40.0f, 14.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-DERECHA-H3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 30.0f, 14.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-DERECHA-H2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 20.0f, 14.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//VENTANAS-DERECHA-H
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture4);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 10.0f, 14.5f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 2.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//DERECHA RAYADO
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 5.0f, 14.8f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//DERECHA RAYADo 2
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 15.0f, 14.8f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//DERECHA RAYADo 3
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 25.0f, 14.8f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//DERECHA RAYADo 4
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture10);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(25.0f, 35.0f, 14.8f));
		//model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(12.0f, 8.0f, 0.4f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//JARDINERA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.5f, 2.1f, 25.0f));
		model = glm::scale(model, glm::vec3(13.0f, 1.9f, 15.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Hojas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture5);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.3f, 3.1f, 25.0f));
		model = glm::scale(model, glm::vec3(10.0f, 0.f, 11.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Arbol
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-3.5f, 1.5f, 25.0f));
		model = glm::scale(model, glm::vec3(0.07f, 0.07f, 0.07f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel_1.Draw(lightingShader);
		glBindVertexArray(0);

		//VENTANA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture6);
		glBindVertexArray(VAO);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(8.5f, 7.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 3.0f,5.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mymodel_2.Draw(lightingShader);
		glBindVertexArray(0);

		//persona
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-9.0f, 0.5f, 15.5f));
		model = glm::translate(model, glm::vec3(NanoX, 0.5f, NanoZ));
		model = glm::rotate(model, rotationNano, glm::vec3(0, 1, 0));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		nano.Draw(lightingShader);
		if (keys[GLFW_KEY_P])
		{
			if (direcionNano == 0 && cuartoVuelta == 0) {
				NanoZ += 0.01;
				if (direcionNano == 0 && NanoZ > 20.0) {
					direcionNano = 1;
					cuartoVuelta = 1;
					NanoZ = 20.0;
				}
			}
			if (direcionNano == 1 && cuartoVuelta == 1) {
				rotationNano += 0.01;
				if (rotationNano > glm::radians(90.0f)) {
					cuartoVuelta = 2;
					rotationNano = glm::radians(90.0f);
				}
			}
			if (direcionNano == 1 && cuartoVuelta == 2) {
				NanoX += 0.01;
				if (direcionNano == 1 && NanoX > 18.0) {
					direcionNano = 2;
					cuartoVuelta = 2;
					NanoX = 18.0;
				}
			}
			if (direcionNano == 2 && cuartoVuelta == 2) {
				rotationNano += 0.01;
				if (rotationNano > glm::radians(180.0f)) {
					cuartoVuelta = 3;
					rotationNano = glm::radians(180.0f);
				}
			}
			if (direcionNano == 2 && cuartoVuelta == 3) {
				NanoZ -= 0.01;
				if (direcionNano == 2 && NanoZ < 0.0) {
					direcionNano = 3;
					cuartoVuelta = 4;
					NanoZ = 0.0;
				}
			}
			if (direcionNano == 3 && cuartoVuelta == 4) {
				rotationNano += 0.01;
				if (rotationNano > glm::radians(270.0f)) {
					cuartoVuelta = 5;
					rotationNano = glm::radians(270.0f);
				}
			}
			if (direcionNano == 3 && cuartoVuelta == 5) {
				NanoX -= 0.01;
				if (direcionNano == 3 && NanoX < 0.0) {
					direcionNano = 0;
					cuartoVuelta = 6;
					NanoX = 0.0;
				}
			}
			if (direcionNano == 0 && cuartoVuelta == 6) {
				rotationNano += 0.01;
				if (rotationNano > glm::radians(360.0f)) {
					cuartoVuelta = 0;
					rotationNano = glm::radians(0.0f);
				}
			}
		}
		float ventanaX=0,  ventanaY=0;

		//for (i = 0; i < 3; i++) {
		//	//Ventanas (de verdad y no sus mamadas de Irving de arriba) enfrente centro
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture9);
		//	glBindVertexArray(VAO);
		//	model = glm::mat4(1);
		//	model = glm::translate(model, glm::vec3(0.0f, 35.1f, -16.2f));
		//	model = glm::translate(model, glm::vec3(0.0f, ventanaY, 0.0f));
		//	model = glm::scale(model, glm::vec3(11.9f, 8.15f, 0.5f));
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//	ventanaY -= 10.0f;
		//}
		//ventanaY = 0;
		//for (i = 0; i < 4; i++) {
		//	//Ventanas (de verdad y no sus mamadas de Irving de arriba) enfrente Izquierdo
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture11);
		//	glBindVertexArray(VAO);
		//	model = glm::mat4(1);
		//	model = glm::translate(model, glm::vec3(-13.5f, 35.1f, -16.2f));
		//	model = glm::translate(model, glm::vec3(0.0f, ventanaY, 0.0f));
		//	model = glm::scale(model, glm::vec3(11.0f, 8.15f, 0.5f));
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//	ventanaY -= 10.0f;
		//}
		//ventanaY = 0;
		//for (i = 0; i < 4; i++) {
		//	//Ventanas (de verdad y no sus mamadas de Irving de arriba) enfrente Derecho
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture11);
		//	glBindVertexArray(VAO);
		//	model = glm::mat4(1);
		//	model = glm::translate(model, glm::vec3(13.5f, 35.1f, -16.2f));
		//	model = glm::translate(model, glm::vec3(0.0f, ventanaY, 0.0f));
		//	model = glm::scale(model, glm::vec3(11.0f, 8.15f, 0.5f));
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//	ventanaY -= 10.0f;
		//}

		//ventanaY = 0;
		//for (i = 0; i < 4; i++) {
		//	//Ventanas (de verdad y no sus mamadas de Irving de arriba) izquiero esquina
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture11);
		//	glBindVertexArray(VAO);
		//	model = glm::mat4(1);
		//	model = glm::translate(model, glm::vec3(-18.25f, 35.0f, -7.65f));
		//	model = glm::translate(model, glm::vec3(0.0f, ventanaY, 0.0f));
		//	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//	model = glm::scale(model, glm::vec3(13.45f, 7.75f, 0.5f));
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//	ventanaY -= 10.0f;
		//}
		//ventanaY = 0;
		//for (i = 0; i < 4; i++) {
		//	//Ventanas (de verdad y no sus mamadas de Irving de arriba) izquiero orilla
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture11);
		//	glBindVertexArray(VAO);
		//	model = glm::mat4(1);
		//	model = glm::translate(model, glm::vec3(-18.25f, 35.0f, 7.65f));
		//	model = glm::translate(model, glm::vec3(0.0f, ventanaY, 0.0f));
		//	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//	model = glm::scale(model, glm::vec3(11.9f, 7.75f, 0.5f));
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//	ventanaY -= 10.0f;
		//}
		//ventanaY = 0;
		//for (i = 0; i < 4; i++) {
		//	//Ventanas (de verdad y no sus mamadas de Irving de arriba) Derecho esquina
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture11);
		//	glBindVertexArray(VAO);
		//	model = glm::mat4(1);
		//	model = glm::translate(model, glm::vec3(18.25f, 35.0f, -7.65f));
		//	model = glm::translate(model, glm::vec3(0.0f, ventanaY, 0.0f));
		//	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//	model = glm::scale(model, glm::vec3(13.45f, 7.75f, 0.5f));
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//	ventanaY -= 10.0f;
		//}
		//ventanaY = 0;
		//for (i = 0; i < 4; i++) {
		//	//Ventanas (de verdad y no sus mamadas de Irving de arriba) Derecho orilla
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture11);
		//	glBindVertexArray(VAO);
		//	model = glm::mat4(1);
		//	model = glm::translate(model, glm::vec3(18.25f, 35.0f, 7.0f));
		//	model = glm::translate(model, glm::vec3(0.0f, ventanaY, 0.0f));
		//	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//	model = glm::scale(model, glm::vec3(11.9f, 7.75f, 0.5f));
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//	ventanaY -= 10.0f;
		//}


		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);

		//for (GLuint i = 0; i < 4; i++)
		//{
		//	model = glm::mat4(1);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	if (keys[GLFW_KEY_1])
	{
		range += 0.1;
		rot += 1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_2])
	{
		range -= 0.1;
		printf("El rango es %f\n", range);
	}

	if (keys[GLFW_KEY_3])
	{
		range += 0.1;
		printf("El spotangle es %f\n", range);
	}

	if (keys[GLFW_KEY_4])
	{
		range -= 0.1;
		printf("El spotangle es %f\n", range);
	}

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z += 1.0f;
	}
	if (keys[GLFW_KEY_G])
	{
		//pointLightPositions[0].x -= 0.1f;
		//pointLightPositions[0].y -= 0.1f;
		pointLightPositions[0].z -= 1.0f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[1].x += 1.0f;
		pointLightPositions[1].y += 1.0f;
		pointLightPositions[1].z += 1.0f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[1].x -= 1.0f;
		pointLightPositions[1].y -= 1.0f;
		pointLightPositions[1].z -= 1.0f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[2].x += 0.5f;
		pointLightPositions[2].y += 0.5f;
		pointLightPositions[2].z += 0.5f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[2].x -= 1.0f;
		pointLightPositions[2].y -= 1.0f;
		pointLightPositions[2].z -= 1.0f;
	}
	if (keys[GLFW_KEY_I])
	{
		pointLightPositions[3].x += 1.0f;
		pointLightPositions[3].y += 1.0f;
		pointLightPositions[3].z += 1.0f;
	}

	if (keys[GLFW_KEY_O])
	{
		circuito = false;
	}
	if (keys[GLFW_KEY_K])
	{
		pointLightPositions[3].x -= 1.0f;
		pointLightPositions[3].y -= 1.0f;
		pointLightPositions[3].z -= 1.0f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
			LightP1 = glm::vec3(1.0f, 0.0f, 0.0f);
		else
			LightP1 = glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{

	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

