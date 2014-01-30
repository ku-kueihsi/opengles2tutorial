precision mediump float;
#extension GL_EXT_separate_shader_objects : enable
#ifdef GL_EXT_separate_shader_objects
#define UseLayout(x) layout(location = x)
#else
#define UseLayout(x)
#endif
//layout (location = 0) attribute vec4 pos;
//layout (location = 1) attribute vec4 color;
UseLayout(0) attribute vec4 pos;
UseLayout(1) attribute vec4 color;

uniform mat4 modelviewProjection;
//attribute vec4 pos;
//attribute vec4 color;
varying vec4 v_color;
void main() {
   gl_Position = modelviewProjection * pos;
   //gl_Position = ftransform();
   v_color = color;
}