package org.omegagraphics.OmegaWTKAndroid;


import android.app.NativeActivity;
import android.content.Context;

public class AndroidAppJava {
    Context context;

    public Context getContext() {
        return context;
    }

    public AndroidAppJava(NativeActivity activity){
        context = (Context) activity;
    }
}
