package com.teampsc.cubeat;

import android.app.NativeActivity;

public class PreloadLibraries extends android.app.NativeActivity {
    static {
        System.loadLibrary("luajit");
        System.loadLibrary("CuBeat");
    }
}
 