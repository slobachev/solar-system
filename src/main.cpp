#include <string>
#include <algorithm>
#include <vector>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL/SOIL.h>

// Properties
GLuint screenWidth = 1200, screenHeight = 800;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint loadCubemap(vector<const GLchar*> faces);

// Camer
Camera camera(glm::vec3(0.0f, 1.0f, 23.0f));
bool keys[1024];
GLfloat lastX = 600, lastY = 400;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Models", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Options
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    // Setup and compile our shaders
    Shader shader("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/shaders/vertex.vs", "C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/shaders/fragment.fs");
	Shader skyboxShader("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/shaders/cubemap.vs", "C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/shaders/cubemap.fs");
    // Load models
	/*Model sun_model("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/objects/Sun/sun2.obj");
	Model mercury_model("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/objects/mercury/mercury.obj");
	Model mars_model("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/objects/mars/Mars_normal-strong.obj");*/
	Model earth_model("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/objects/earth/earth.obj");
	/*Model venus_model("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/objects/Venus/venus.obj");
	Model saturn_model("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/objects/Saturn/saturn3.obj");
	Model neptun_model("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/objects/Neptune/neptune.obj");
	Model uranus_model("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/objects/Uranus/uranus.obj");
	Model jupiter_model("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/objects/jupiter/jupiter1.obj");*/
	
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	// Setup skybox VAO
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);

	// Cubemap
	/*vector<const GLchar*> faces;
	faces.push_back("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/textures/skybox/right.jpg");
	faces.push_back("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/textures/skybox/left.jpg");
	faces.push_back("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/textures/skybox/top.jpg");
	faces.push_back("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/textures/skybox/bottom.jpg");
	faces.push_back("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/textures/skybox/back.jpg");
	faces.push_back("C:/Users/user/Documents/Visual Studio 2015/Projects/SecondSemester/Cubemap/resources/textures/skybox/front.jpg");
	GLuint cubemapTexture = loadCubemap(faces);*/

    // Game loop
    while(!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = 2*(currentFrame - lastFrame);
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        Do_Movement();

        // Clear the colorbuffer
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();   // <-- Don't forget this one!
        // Transformation matrices
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth/(float)screenHeight, 0.1f, 45.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model1, model2, model3, model4, model5, model6, model7, model8, model9;
  //      model1 = glm::translate(model1, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
  //      model1 = glm::scale(model1, glm::vec3(0.1f, 0.1f, 0.1f));	// It's a bit too big for our scene, so scale it down
  //      glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model1));
  //      sun_model.Draw(shader);

		//model2 = glm::translate(model2, glm::vec3(11.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model2 = glm::scale(model2, glm::vec3(0.01f, 0.01f, 0.01f));	// It's a bit too big for our scene, so scale it down
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model2));
		//mercury_model.Draw(shader);

		//model3 = glm::translate(model3, glm::vec3(-11.5f, -1.75f, -6.0f)); // Translate it down a bit so it's at the center of the scene
		//model3 = glm::scale(model3, glm::vec3(0.01f, 0.01f, 0.01f));	// It's a bit too big for our scene, so scale it down
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model3));
		//venus_model.Draw(shader);

		model4 = glm::translate(model4, glm::vec3(-12.0f, -1.75f, 10.0f)); // Translate it down a bit so it's at the center of the scene
		model4 = glm::scale(model4, glm::vec3(0.003f, 0.003f, 0.003f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model4));
		earth_model.Draw(shader);

		//model5 = glm::translate(model5, glm::vec3(12.0f, -1.75f, -9.0f)); // Translate it down a bit so it's at the center of the scene
		//model5 = glm::scale(model5, glm::vec3(0.1f, 0.1f, 0.1f));	// It's a bit too big for our scene, so scale it down
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model5));
		//mars_model.Draw(shader);

		//model6 = glm::translate(model6, glm::vec3(-17.0f, -1.75f, 14.0f)); // Translate it down a bit so it's at the center of the scene
		//model6 = glm::scale(model6, glm::vec3(0.03f, 0.03f, 0.03f));	// It's a bit too big for our scene, so scale it down
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model6));
		//saturn_model.Draw(shader);

		//model7 = glm::translate(model7, glm::vec3(-13.0f, -1.75f, -17.0f)); // Translate it down a bit so it's at the center of the scene
		//model7 = glm::scale(model7, glm::vec3(0.03f, 0.03f, 0.03f));	// It's a bit too big for our scene, so scale it down
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model7));
		//neptun_model.Draw(shader);

		//model8 = glm::translate(model8, glm::vec3(15.0f, -1.75f, -22.0f)); // Translate it down a bit so it's at the center of the scene
		//model8 = glm::scale(model8, glm::vec3(0.03f, 0.03f, 0.03f));	// It's a bit too big for our scene, so scale it down
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model8));
		//uranus_model.Draw(shader);

		//model9 = glm::translate(model9, glm::vec3(19.0f, -1.75f, 16.0f)); // Translate it down a bit so it's at the center of the scene
		//model9 = glm::scale(model9, glm::vec3(0.05f, 0.05f, 0.05f));	// It's a bit too big for our scene, so scale it down
		//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model9));
		//jupiter_model.Draw(shader);

		// Draw skybox as lasl
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		// skybox cube
		/*glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);*/
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front)
// -Z (back)
GLuint loadCubemap(vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	return textureID;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}