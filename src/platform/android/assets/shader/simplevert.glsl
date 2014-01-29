#ifdef GLES2
precision mediump float;
#extension GL_EXT_separate_shader_objects 
layout (location = 0) attribute vec4 a_Position;
layout (location = 1) attribute vec3 a_Normal;
layout (location = 2) attribute vec2 a_TextureCoordinates;
layout (location = 3) attribute vec3 a_boneIds;
layout (location = 4) attribute vec3 a_boneWeights;
#else//PC GL

#version 330
layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TextureCoordinates;
layout (location = 3) in vec3 a_boneIds;
layout (location = 4) in vec3 a_boneWeights;
#endif

const int kMaxBoneWeights = 3;
const int kMaxBones = 100;

uniform mat4 modelviewProjection;
uniform mat4 world_mat;
uniform mat4 gBones[kMaxBones];

varying vec2 v_TextureCoordinates;
varying vec3 v_Normal;
varying vec4 debugColor;

void main()
{
//    mat4 BoneTransform = gBones[a_boneIds[0]] * a_boneWeights[0];
//    BoneTransform     += gBones[a_boneIds[1]] * a_boneWeights[1];
//    BoneTransform     += gBones[a_boneIds[2]] * a_boneWeights[2];

    mat4 BoneTransform = gBones[int(a_boneIds[0])] * a_boneWeights[0];
    BoneTransform     += gBones[int(a_boneIds[1])] * a_boneWeights[1];
    BoneTransform     += gBones[int(a_boneIds[2])] * a_boneWeights[2];

    v_TextureCoordinates = a_TextureCoordinates;

    v_Normal = (world_mat * BoneTransform * vec4(a_Normal, 0.0)).xyz;
    gl_Position = modelviewProjection * BoneTransform * vec4(a_Position, 1.0);

//    debugColor[a_boneWeights[a_boneIds[0]], a_boneWeights[a_boneIds[1]], 1, 1];
    debugColor = vec4(a_boneIds[0], 0, 0, 1);
//    v_Normal = (world_mat * gBones[a_boneIds[0]] * vec4(a_Normal, 0.0)).xyz;
//    gl_Position = modelviewProjection * gBones[a_boneIds[0]] * vec4(a_Position, 1.0);

//    v_Normal = (world_mat * vec4(a_Normal, 0.0)).xyz;
//    gl_Position = modelviewProjection * vec4(a_Position, 1.0);
    //gl_Position = a_Position;
}


//uniform mat4 modelviewProjection;
//attribute vec4 pos;
//attribute vec4 color;
//varying vec4 v_color;
//void main() {
//   gl_Position = modelviewProjection * pos;
//   //gl_Position = ftransform();
//   v_color = color;
//}
