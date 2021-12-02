package org.omegagraphics.OmegaWTKAndroid;


import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import androidx.annotation.NonNull;

public class AndroidNotificationCenterJava {
    private final Notification.Builder builder;
    private final NotificationManager manager;
    private int currentId = 0;
    public AndroidNotificationCenterJava(@NonNull Context context){
        String id = "MyID";

        NotificationChannel channel = new NotificationChannel(id,"",NotificationManager.IMPORTANCE_DEFAULT);
        this.manager = context.getSystemService(NotificationManager.class);
        manager.createNotificationChannel(channel);

        this.builder = new Notification.Builder(context,id);
    }
    public void sendNotification(String title,String desc){
        builder.setContentTitle(title).setContentText(desc);
        manager.notify(currentId,builder.build());
        currentId += 1;
    }
}
