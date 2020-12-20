#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdio>

GLuint LoadShaders(
	const char * vertex_file_path,
	const char * fragment_file_path
);