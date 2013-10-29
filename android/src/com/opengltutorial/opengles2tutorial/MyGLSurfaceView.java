package com.opengltutorial.opengles2tutorial;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class MyGLSurfaceView extends GLSurfaceView{
	MyRenderer mRenderer;
	public MyGLSurfaceView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}

	// Hides superclass method.
    public void setRenderer(MyRenderer renderer)
    {
        mRenderer = renderer;
        super.setRenderer(renderer);
    }

}
