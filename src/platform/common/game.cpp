#include "game.h"
//#include "glwrapper.h"
#ifdef __cplusplus
extern "C" {
#endif
#include <GLES2/gl2.h>  /* use OpenGL ES 2.x */
#include <android/log.h>
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

size_t filelen(FILE * pfile) {
	if (!pfile){
		return 0;
	}
	fseek(pfile, NULL, SEEK_END);
	size_t filesize = ftell(pfile);
	rewind (pfile);
	return filesize;
}

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
static FILE * vertshader_file = NULL, * fragshader_file = NULL;

static GLint u_matrix = -1;
static GLint attr_pos = 0, attr_color = 1;

void set_vertshader_file(FILE * pfile){
	vertshader_file = pfile;
//	__android_log_print(ANDROID_LOG_VERBOSE, "test", "%d", filelen(vertshader_file));
//	__android_log_print(ANDROID_LOG_VERBOSE, "test", "%d\n", vertshader_file);
}

void set_fragshader_file(FILE * pfile){
	fragshader_file = pfile;
//	__android_log_print(ANDROID_LOG_VERBOSE, "test", "%d", filelen(fragshader_file));
//	__android_log_print(ANDROID_LOG_VERBOSE, "test", "%d\n", fragshader_file);
}

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

GLuint LoadShader(FILE * vfile, FILE * ffile){
//	__android_log_print(ANDROID_LOG_VERBOSE, "test", "loading shaders");
	if (!vfile || !ffile){
		exit(1);
	}
	size_t len;
	len = filelen(vfile);
	char * vertShaderText = new char[len + 1];
	fread(vertShaderText, len, sizeof(char), vfile);
	vertShaderText[len] = '\0';

	len = filelen(ffile);
	char * fragShaderText = new char[len + 1];
	fread(fragShaderText, len, sizeof(char), ffile);
	fragShaderText[len] = '\0';

//	__android_log_print(ANDROID_LOG_VERBOSE, "test", "%s\n", vertShaderText);
//	__android_log_print(ANDROID_LOG_VERBOSE, "test", "%s\n", fragShaderText);

	GLuint fragShader, vertShader, program;
	GLint stat;

	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, (const char **) &fragShaderText, NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &stat);
	if (!stat) {
		//      printf("Error: fragment shader did not compile!\n");
		exit(1);
	}

	vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, (const char **) &vertShaderText, NULL);
	glCompileShader(vertShader);
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &stat);
	if (!stat) {
		//      printf("Error: vertex shader did not compile!\n");
		exit(1);
	}

	program = glCreateProgram();
	glAttachShader(program, fragShader);
	glAttachShader(program, vertShader);

	delete vertShaderText;
	delete fragShaderText;
	return program;
}

static void create_shaders(void) {
//	static const char *fragShaderText = "precision mediump float;\n"
//			"varying vec4 v_color;\n"
//			"void main() {\n"
//			"   gl_FragColor = v_color;\n"
//			"}\n";
//	static const char *vertShaderText = "precision mediump float;\n"
//			"uniform mat4 modelviewProjection;\n"
//			"attribute vec4 pos;\n"
//			"attribute vec4 color;\n"
//			"varying vec4 v_color;\n"
//			"void main() {\n"
//			"   gl_Position = modelviewProjection * pos;\n"
//			"   v_color = color;\n"
//			"}\n";
////	__android_log_print(ANDROID_LOG_VERBOSE, "test", "%s", vertShaderText);
////	__android_log_print(ANDROID_LOG_VERBOSE, "test", "%s", fragShaderText);
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
//
//	program = glCreateProgram();
//	glAttachShader(program, fragShader);
//	glAttachShader(program, vertShader);

	program = LoadShader(vertshader_file, fragshader_file);
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
