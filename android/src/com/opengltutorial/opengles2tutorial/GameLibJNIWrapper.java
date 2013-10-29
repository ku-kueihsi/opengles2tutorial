package com.opengltutorial.opengles2tutorial;

public class GameLibJNIWrapper {
	static {
        System.loadLibrary("game");
    }
 
    public static native void on_surface_created();
 
    public static native void on_surface_changed(int width, int height);
 
    public static native void on_draw_frame();
    
    public static native void on_touch();
}
