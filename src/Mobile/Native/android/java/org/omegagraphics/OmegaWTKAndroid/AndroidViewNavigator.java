package org.omegagraphics.OmegaWTKAndroid;

import androidx.annotation.NonNull;

import java.util.ArrayList;

public class AndroidViewNavigator {
    private final ArrayList<AndroidItemJava> items;

    private AndroidItemJava currentView;

    public AndroidViewNavigator(){
        items = new ArrayList<AndroidItemJava>();
        currentView = null;
    }
    /** Adds view to Navigator
     * */
    public void addView(@NonNull AndroidItemJava itemJava){
        items.add(itemJava);
        if(currentView == null){
            currentView = itemJava;
        }
    }
    
    public void removeView(@NonNull  AndroidItemJava itemJava){
        items.remove(itemJava);
        if(items.isEmpty()){
            currentView = null;
        }
    }


}
