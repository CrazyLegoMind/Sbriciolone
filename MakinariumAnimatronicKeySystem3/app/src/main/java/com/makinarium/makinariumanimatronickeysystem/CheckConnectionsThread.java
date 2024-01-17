package com.makinarium.makinariumanimatronickeysystem;

import android.content.Context;
import android.content.Intent;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v4.content.res.ResourcesCompat;
import android.util.Log;
import android.widget.TextView;

import com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities.Constants;

public class CheckConnectionsThread extends Thread {

    private static final long timeToDeclareDeath = 2000;


    private long lastTimeMouthAlive;
    private long lastTimeEyesAlive;
    private long lastTimeHeadAlive;

    private boolean receiverconnected = false;

    private boolean threadAlive = true;

    private TextView headStatusView;
    private TextView eyesStatusView;

    private TextView mouthStatusView;

    private int aliveColor = 0;
    private int deadColor = 0;
    
    public CheckConnectionsThread(Context mContext,TextView eyesView,TextView headView, TextView mouthView, int aliveColor,int deadColor) {
        this.eyesStatusView = eyesView;
        this.mouthStatusView = mouthView;
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
            
            updateUI(currentTime,lastTimeEyesAlive,eyesStatusView,false);
            updateUI(currentTime,lastTimeMouthAlive,mouthStatusView,false);
            updateUI(currentTime,lastTimeHeadAlive,headStatusView,true);
        }

    }

    private void updateUI(long currentTime, long lastTime, TextView statusView,boolean receiver)
    {
        if (receiver){
            receiverconnected = currentTime - lastTime < timeToDeclareDeath;
        }
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
    public void setLastTimeMouthAlive(long lastTimeTailAlive) {
        this.lastTimeMouthAlive = lastTimeTailAlive;
    }

    public void stopChecking()
    {
        threadAlive = false;
    }

    public boolean getReceiverStatus(){
        return receiverconnected;
    }

}
