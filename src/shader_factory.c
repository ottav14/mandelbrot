#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "shader_factory.h"

char* read_shader(char *shader_path) {

	FILE *shader = fopen(shader_path, "r");
	if(shader == NULL) {
		perror("Error opening shader.\n");
		return "";
	}

	fseek(shader, 0, SEEK_END);
	long file_size = ftell(shader);
	fseek(shader, 0, SEEK_SET);

	char *buffer = (char*)malloc(file_size);

	fread(buffer, file_size, 1, shader);

	fclose(shader);

	return buffer;
}

GLuint compile_shader(GLenum type, const char* source_code) {

	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &source_code, NULL);
	glCompileShader(shader);


	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		perror("Shader compilation error.\n");
		glDeleteShader(shader);
		return 0;
	}

	return shader;

}

GLuint create_shader_program(char* vertex_path, char* fragment_path) {
	const char* vertex_code = read_shader(vertex_path);
	const char* fragment_code = read_shader(fragment_path);

	GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_code);
	GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_code);

	free((void*)vertex_code);
	free((void*)fragment_code);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	GLint success;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (!success) {
		char info_log[512];
		glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
		fprintf(stderr, "Shader program linking error: %s\n", info_log);
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	return shader_program;
}

