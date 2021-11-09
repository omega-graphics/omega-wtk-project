package org.omegagraphics.OmegaWTKAndroid;

import android.app.*;
import android.content.Context;
import android.view.*;
import android.widget.FrameLayout;
import android.widget.RelativeLayout;
import androidx.annotation.NonNull;

public class AndroidItemJava {
    SurfaceView surfaceView;
    SurfaceControl control;
    FrameLayout layout;
    public AndroidItemJava(Context context,int x,int y,int w,int h){
        surfaceView = new SurfaceView(context);
        surfaceView.layout(x,y + h,x + w,y);
        control = surfaceView.getSurfaceControl();
        layout = new FrameLayout(context);
        layout.addView(surfaceView);
    }
    public Surface getSurface(){
        return new Surface(control);
    }

    public void addChildNativeItem(@NonNull AndroidItemJava itemJava){
        layout.addView(itemJava.layout);
    }

    public void removeChildNativeItem(@NonNull AndroidItemJava itemJava){
        layout.removeView(itemJava.layout);
    }

    public void enable(){
        surfaceView.setVisibility(View.VISIBLE);
    }

    public void disable(){
        surfaceView.setVisibility(View.INVISIBLE);
    }

    public void resize(int x,int y,int w,int h){
        surfaceView.requestLayout();
        surfaceView.layout(x,y + h,x + w,y);
    }
    static AndroidItemJava Create(Context context,int x,int y,int w,int h){
      return new AndroidItemJava(context,x,y,w,h);
    }
}