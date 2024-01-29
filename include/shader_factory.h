#ifndef SHADER_FACTORY_H
#define SHADER_FACTORY_H

char* read_shader(char *shader_path);
GLuint compile_shader(GLenum type, const char* source_code);
GLuint create_shader_program(char* vertex_path, char* fragment_path);

#endif
