#include "pch.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

int main()
{
	if (!glfwInit()) return -1;

	// request OpenGL 3.3 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	return 0;
}