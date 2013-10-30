package com.opengltutorial.opengles2tutorial;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

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
    
    @Override
    public boolean onTouchEvent(MotionEvent event)
    {
        if (event != null)
        {
//            if (event.getAction() == MotionEvent.ACTION_DOWN)
//            {
//                if (mRenderer != null)
//                {
//                    // Ensure we call switchMode() on the OpenGL thread.
//                    // queueEvent() is a method of GLSurfaceView that will do this for us.
//                    queueEvent(new Runnable()
//                    {
//                        @Override
//                        public void run()
//                        {
//                            mRenderer.switchMode();
//                        }
//                    });
// 
//                    return true;
//                }
//            }
                mRenderer.onTouch();
        }

        return super.onTouchEvent(event);
    }

}
