#include "game.h"
//#include "glwrapper.h"
#ifdef __cplusplus
extern "C" {
#endif
#include <GLES2/gl2.h>  /* use OpenGL ES 2.x */
#ifdef __cplusplus
}
#endif
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string.h>
#include <cstdlib>
#include <cstdio>

using namespace std;

template<class T>
void SimpleCross(T v1x, T v1y, T v1z, T v2x, T v2y, T v2z, T &v3x, T &v3y,
		T &v3z) {
	v3x = v1y * v2z - v1z * v2y;
	v3y = v1z * v2x - v1x * v2z;
	v3z = v1x * v2y - v1y * v2x;
	return;
}

template<class T>
T SimpleDot(T v1x, T v1y, T v1z, T v2x, T v2y, T v2z) {
	return v1x * v2x + v1y * v2y + v1z * v2z;
}

template<class T>
T SimpleLength(T x, T y, T z) {
	return sqrt(SimpleDot(x, y, z, x, y, z));
}

template<class T>
void SimpleNormalize(T x, T y, T z, T &nx, T &ny, T &nz) {
	T normal = SimpleLength(x, y, z);
	if (normal > 0) {
		nx = x / normal;
		ny = y / normal;
		nz = z / normal;
	} else {
		nx = 0.0f;
		ny = 0.0f;
		nz = 0.0f;
	}
	return;
}

template<class T>
void SimpleNormalize(T &x, T &y, T &z) {

	T normal = SimpleLength(x, y, z);
	if (normal > 0) {
		x = x / normal;
		y = y / normal;
		z = z / normal;
	} else {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	return;
}

static GLfloat view_rotx = 0.0;// view_roty = 0.0;

static GLint u_matrix = -1;
static GLint attr_pos = 0, attr_color = 1;

static void make_z_rot_matrix(GLfloat angle, GLfloat *m) {
	float c = cos(angle * M_PI / 180.0);
	float s = sin(angle * M_PI / 180.0);
//   int i;
//   for (i = 0; i < 16; i++)
//      m[i] = 0.0;
	memset(m, 0, sizeof(m) * 16);
	m[0] = m[5] = m[10] = m[15] = 1.0;

	m[0] = c;
	m[1] = s;
	m[4] = -s;
	m[5] = c;
}

static void make_scale_matrix(GLfloat xs, GLfloat ys, GLfloat zs, GLfloat *m) {
//   int i;
//   for (i = 0; i < 16; i++)
//      m[i] = 0.0;
	memset(m, 0, sizeof(m) * 16);
	m[0] = xs;
	m[5] = ys;
	m[10] = zs;
	m[15] = 1.0;
}

static void mul_matrix(GLfloat *prod, const GLfloat *a, const GLfloat *b) {
#define A(row,col)  a[(col<<2)+row]
#define B(row,col)  b[(col<<2)+row]
#define P(row,col)  p[(col<<2)+row]
	GLfloat p[16];
	GLint i;
	for (i = 0; i < 4; i++) {
		const GLfloat ai0 = A(i, 0), ai1 = A(i, 1), ai2 = A(i, 2), ai3 = A(i,
				3);
		P(i,0)= ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
		P(i,1)= ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
		P(i,2)= ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
		P(i,3)= ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
	}
	memcpy(prod, p, sizeof(p));
#undef A
#undef B
#undef P
}

static void draw(void) {
	static const GLfloat verts[3][2] = { { -1, -1 }, { 1, -1 }, { 0, 1 } };
	static const GLfloat colors[3][3] =
			{ { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
	GLfloat mat[16], rot[16], scale[16];

	/* Set modelview/projection matrix */
	make_z_rot_matrix(view_rotx, rot);
	make_scale_matrix(0.5, 0.5, 0.5, scale);
	mul_matrix(mat, rot, scale);
	glUniformMatrix4fv(u_matrix, 1, GL_FALSE, mat);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		glVertexAttribPointer(attr_pos, 2, GL_FLOAT, GL_FALSE, 0, verts);
		glVertexAttribPointer(attr_color, 3, GL_FLOAT, GL_FALSE, 0, colors);
		glEnableVertexAttribArray(attr_pos);
		glEnableVertexAttribArray(attr_color);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(attr_pos);
		glDisableVertexAttribArray(attr_color);
	}
}

std::string readFile(const char *filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

GLuint LoadShader(const char *vertex_path, const char *fragment_path) {
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shaders
    std::string vertShaderStr = readFile(vertex_path);
    std::string fragShaderStr = readFile(fragment_path);
    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    GLint result = GL_FALSE;
    int logLength;

    // Compile vertex shader
    std::cout << "Compiling vertex shader." << std::endl;
    glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
    glCompileShader(vertShader);

    // Check vertex shader
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> vertShaderError(logLength);
    glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
    std::cout << &vertShaderError[0] << std::endl;

    // Compile fragment shader
    std::cout << "Compiling fragment shader." << std::endl;
    glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
    glCompileShader(fragShader);

    // Check fragment shader
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> fragShaderError(logLength);
    glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
    std::cout << &fragShaderError[0] << std::endl;

    std::cout << "Linking program" << std::endl;
    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &result);
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::vector<char> programError( (logLength > 1) ? logLength : 1 );
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}

//unsigned long getFileLength(ifstream& file) {
//	if (!file.good())
//		return 0;
//
//	unsigned long pos = file.tellg();
//	file.seekg(0, ios::end);
//	unsigned long len = file.tellg();
//	file.seekg(ios::beg);
//
//	return len;
//}
//
//int loadshader(char* filename, GLchar** ShaderSource, unsigned long &len) {
//	ifstream file;
//	file.open(filename, ios::in); // opens as ASCII!
//	if (!file)
//		return -1;
//
//	len = getFileLength(file);
//
//	if (len == 0)
//		return -2;   // Error: Empty File
//
//	*ShaderSource = (GLchar *) new char[len + 1];
//	if (*ShaderSource == 0)
//		return -3;   // can't reserve memory
//
//	// len isn't always strlen cause some characters are stripped in ascii read...
//	// it is important to 0-terminate the real length later, len is just max possible value...
//	*ShaderSource[len] = 0;
//
//	unsigned int i = 0;
//	while (file.good()) {
//		*ShaderSource[i] = file.get();       // get character from file.
//		if (!file.eof())
//			i++;
//	}
//
//	*ShaderSource[i] = 0;  // 0-terminate it at the correct position
//
//	file.close();
//
//	return 0; // No Error
//}
//
//int unloadshader(GLubyte** ShaderSource) {
//	if (*ShaderSource != 0)
//		delete[] *ShaderSource;
//	*ShaderSource = 0;
//}

static void create_shaders(void) {
	static const char *fragShaderText = "precision mediump float;\n"
			"varying vec4 v_color;\n"
			"void main() {\n"
			"   gl_FragColor = v_color;\n"
			"}\n";
	static const char *vertShaderText = "precision mediump float;\n"
			"uniform mat4 modelviewProjection;\n"
			"attribute vec4 pos;\n"
			"attribute vec4 color;\n"
			"varying vec4 v_color;\n"
			"void main() {\n"
			"   gl_Position = modelviewProjection * pos;\n"
			"   v_color = color;\n"
			"}\n";

	GLuint fragShader, vertShader, program;
	GLint stat;

//	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragShader, 1, (const char **) &fragShaderText, NULL);
//	glCompileShader(fragShader);
//	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &stat);
//	if (!stat) {
////      printf("Error: fragment shader did not compile!\n");
//		exit(1);
//	}
//
//	vertShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertShader, 1, (const char **) &vertShaderText, NULL);
//	glCompileShader(vertShader);
//	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &stat);
//	if (!stat) {
////      printf("Error: vertex shader did not compile!\n");
//		exit(1);
//	}
//
////	unsigned long file_len;
////	GLchar ** shadersource;
////
////
////	const char fragname[] = "simplefrag.frag";
////	file_len = getFileLength(fragname);
////	loadshader(fragname, shadersource, file_len);
////
////	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
////	glShaderSource(fragShader, 1, shadersource, NULL);
////	glCompileShader(fragShader);
////	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &stat);
////	if (!stat) {
////		//      printf("Error: fragment shader did not compile!\n");
////		exit(1);
////	}
////
////	unloadshader((GLubyte**) shadersource);
////
////	const char fragname[] = "simplevert.vert";
////	file_len = getFileLength(fragname);
////	loadshader(fragname, shadersource, file_len);
////
////	vertShader = glCreateShader(GL_VERTEX_SHADER);
////	glShaderSource(vertShader, 1, shadersource, NULL);
////	glCompileShader(vertShader);
////	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &stat);
////	if (!stat) {
////		//      printf("Error: vertex shader did not compile!\n");
////		exit(1);
////	}
////
////	unloadshader((GLubyte**) shadersource);
//
//	program = glCreateProgram();
//	glAttachShader(program, fragShader);
//	glAttachShader(program, vertShader);

	program = LoadShader("assets/simplevert.vert", "assets/simplefrag.frag");
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &stat);
	if (!stat) {
		char log[1000];
		GLsizei len;
		glGetProgramInfoLog(program, 1000, &len, log);
		printf("Error: linking:\n%s\n", log);
		exit(1);
	}

	glUseProgram(program);

	if (1) {
		/* test setting attrib locations */
		glBindAttribLocation(program, attr_pos, "pos");
		glBindAttribLocation(program, attr_color, "color");
		glLinkProgram(program); /* needed to put attribs into effect */
	} else {
		/* test automatic attrib locations */
		attr_pos = glGetAttribLocation(program, "pos");
		attr_color = glGetAttribLocation(program, "color");
	}

	u_matrix = glGetUniformLocation(program, "modelviewProjection");
	printf("Uniform modelviewProjection at %d\n", u_matrix);
	printf("Attrib pos at %d\n", attr_pos);
	printf("Attrib color at %d\n", attr_color);
}

void on_surface_created() {
	glClearColor(0.4, 0.4, 0.4, 0.0);
	// OpenGL init
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_CULL_FACE);
	//glEnable(GL_LIGHTING);
	create_shaders();
}

void on_surface_changed(int width, int height) {
	// No-op
	GLint w = width;
	GLint h = height;
	if (h == 0)
		h = 1;
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

}

void on_touch() {
	view_rotx += 5.0;
	printf("touched\n");
	//draw();
}

void on_draw_frame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
//    view_rotx += 5.0;
	// Reset transformations

}
