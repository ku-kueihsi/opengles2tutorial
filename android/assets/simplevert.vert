precision mediump float;
uniform mat4 modelviewProjection;
attribute vec4 pos;
attribute vec4 color;
varying vec4 v_color;
void main() {
   gl_Position = modelviewProjection * pos;
   v_color = color;\n"
}