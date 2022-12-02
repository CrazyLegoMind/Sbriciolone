package com.makinarium.makinariumanimatronickeysystem;

import android.content.Context;
import android.content.Intent;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v4.content.res.ResourcesCompat;
import android.util.Log;
import android.widget.TextView;

import com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities.Constants;

public class CheckConnectionsThread extends Thread {

    private static final long timeToDeclareDeath = 5000;

    private Context mContext;

    private long lastTimeMouthAlive;
    private long lastTimeEyesAlive;
    private long lastTimeHeadAlive;
    private long lastTimeTailAlive;
    private boolean mouthStatus = true;
    private boolean eyesStatus = true;
    private boolean headStatus = true;
    private boolean tailStatus = true;

    private boolean threadAlive = true;

    private TextView headStatusView;
    private TextView eyesStatusView;

    private int aliveColor = 0;
    private int deadColor = 0;
    
    public CheckConnectionsThread(Context mContext,TextView eyesView,TextView headView, int aliveColor,int deadColor) {
        this.mContext = mContext;
        this.eyesStatusView = eyesView;
        this.headStatusView = headView;
        this.aliveColor = aliveColor;
        this.deadColor = deadColor;
    }

    public void run(){

        while(threadAlive)
        {
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }

            long currentTime = System.currentTimeMillis();
            
            checkAndSend(currentTime,lastTimeEyesAlive,eyesStatusView);
            checkAndSend(currentTime,lastTimeHeadAlive,headStatusView);
        }

    }

    private void checkAndSend(long currentTime, long lastTime, TextView statusView)
    {
        if(currentTime - lastTime < timeToDeclareDeath) {

            statusView.setText(Constants.connectionOK);
            statusView.setTextColor(aliveColor);
            return;
        }
        statusView.setText(Constants.connectionNO);
        statusView.setTextColor(deadColor);

    }
    
    public void setLastTimeEyesAlive(long lastTimeEyesAlive) {
        this.lastTimeEyesAlive = lastTimeEyesAlive;
    }

    public void setLastTimeHeadAlive(long lastTimeHeadAlive) {
        this.lastTimeHeadAlive = lastTimeHeadAlive;
    }
    public void setLastTimeTailAlive(long lastTimeTailAlive) {
        this.lastTimeTailAlive = lastTimeTailAlive;
    }

    public void stopChecking()
    {
        threadAlive = false;
    }

}
