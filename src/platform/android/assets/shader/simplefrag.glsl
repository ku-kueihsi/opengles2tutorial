#ifdef GLES2

precision mediump float;
#extension GL_EXT_separate_shader_objects 

#else //PC GL 330+

#version 330

#endif

uniform sampler2D u_TextureUnit;
varying vec2 v_TextureCoordinates;
varying vec3 v_Normal;
varying vec4 debugColor;

void main()
{
    //u_TextureUnit = 0;
    float DiffuseFactor = dot(normalize(v_Normal), normalize(vec3(1.0f, -1.0f, 1.0f)));
    vec4 DiffuseColor;                                                              
                                                                                    
    if (DiffuseFactor > 0) {                                                        
        DiffuseColor = vec4(1.0f, 1.0f, 1.0f, 1.0f) * DiffuseFactor;                                               
    }                                                                               
    else {                                                                          
        DiffuseColor = vec4(0, 0, 0, 0);                                            
    } 
    gl_FragColor = texture2D(u_TextureUnit, v_TextureCoordinates) * (DiffuseColor + vec4(0.1f, 0.1f, 0.1f, 0.0f));
//    gl_FragColor = debugColor;
//    gl_FragColor = vec4(1,1,1,1);

    //gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
}

//varying vec4 v_color;
//void main() {
//	gl_FragColor = v_color;
//	//gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);  
//}
