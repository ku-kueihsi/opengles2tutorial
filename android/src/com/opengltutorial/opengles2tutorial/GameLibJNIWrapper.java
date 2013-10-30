package com.opengltutorial.opengles2tutorial;

import java.io.IOException;
import java.io.InputStream;

import android.content.res.AssetManager;

public class GameLibJNIWrapper {
	static {
        System.loadLibrary("game");
    }
	
//	public static AssetManager assMgr; // asset manager, pass asset info to ndk
//    
//    public static byte[] getAssetContents(String path) throws IOException
//    {
//	    InputStream is = assMgr.open(path, AssetManager.ACCESS_BUFFER);
//	    byte buffer[] = new byte[is.available()];
//	    is.read(buffer);
//	    return buffer;
//    }
    
    public static native void on_surface_created();
 
    public static native void on_surface_changed(int width, int height);
 
    public static native void on_draw_frame();
    
    public static native void on_touch();
}
