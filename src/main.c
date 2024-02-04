#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "shader_factory.h"
#include "vector.h"

GLFWwindow* create_window() {

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	GLFWwindow *window = glfwCreateWindow(1920, 1080, "OpenGL Window", monitor, NULL);
	if (!window) {
		glfwTerminate();
		return NULL;
	}

	return window;
}

void setup_glfw(GLFWwindow *window) {

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void setup_glew() {

	if(glewInit() != GLEW_OK) {
		perror("Error initializing GLEW.\n");
		glfwTerminate();
		return;
	}

	if (!GLEW_VERSION_3_3) {
		perror("OpenGL 3.3 is not supported!\n");
		glfwTerminate();
		return;
	}

}

void bind_vertex_buffer(GLuint *VAO) {

	float vertices[] = {
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f,  -1.0f, 0.0f,

		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f
	};

	GLuint VBO;
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(*(unsigned int*)VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void handle_input(GLFWwindow *window, double* camera_position, double* zoom) {

	if(glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	double camera_speed = 0.02;
	double zoom_speed = 1.01;
	double move_vector[] = {0.0, 0.0};

	// Camera movement
	if(glfwGetKey(window, GLFW_KEY_W)) {
		move_vector[1] += 1.0;
	}
	if(glfwGetKey(window, GLFW_KEY_A)) {
		move_vector[0] -= 1.0;
	}
	if(glfwGetKey(window, GLFW_KEY_S)) {
		move_vector[1] -= 1.0;
	}
	if(glfwGetKey(window, GLFW_KEY_D)) {
		move_vector[0] += 1.0;
	}

	// Zoom
	if(glfwGetKey(window, GLFW_KEY_UP)) {
		*zoom /= zoom_speed;
	}
	if(glfwGetKey(window, GLFW_KEY_DOWN)) {
		*zoom *= zoom_speed;
	}


	// Normalize move vector
	double move_magnitude = sqrt(move_vector[0]*move_vector[0] + move_vector[1]*move_vector[1]);
	if(move_magnitude > 0) {
		move_vector[0] *= *zoom * camera_speed / move_magnitude;
		move_vector[1] *= *zoom * camera_speed / move_magnitude;
	}

	// Update values
	camera_position[0] += move_vector[0];
	camera_position[1] += move_vector[1];
}

int main(int argc, char **argv) {

	if(!glfwInit()) {
		return -1;
	}

	GLFWwindow *window = create_window();
	setup_glfw(window);
	setup_glew();
	GLuint shader_program = create_shader_program("../src/vertex_shader.glsl", "../src/fragment_shader.glsl");

	GLuint VAO;
	bind_vertex_buffer(&VAO);

	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glUseProgram(shader_program);
	GLint resolution_location = glGetUniformLocation(shader_program, "resolution");
	glUniform2f(resolution_location, 1920.0f, 1080.0f);


	double camera_position[] = {0.0, 0.0};
	double zoom = 1.0;
	double time = 0.0;


	while (!glfwWindowShouldClose(window)) {

		handle_input(window, camera_position, &zoom);

		GLint time_location = glGetUniformLocation(shader_program, "time");
		glUniform1f(time_location, time);
		time += 0.01;

		GLint camera_position_location = glGetUniformLocation(shader_program, "camera_position");
		glUniform2f(camera_position_location, camera_position[0], camera_position[1]);

		GLint zoom_location = glGetUniformLocation(shader_program, "zoom");
		glUniform1f(zoom_location, zoom);

		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		glfwSwapBuffers(window);

		glfwPollEvents();
		
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

}
