package com.makinarium.makinariumanimatronickeysystem.com.makinarium.presetthings;

import android.icu.text.RelativeDateTimeFormatter;
import android.util.Log;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.makinarium.makinariumanimatronickeysystem.FaceSector;
import com.makinarium.makinariumanimatronickeysystem.MessageTypes;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities.AnalogDirection;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities.Constants;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;


//handler class for for single animations, not presets
//holds a performance, so list of messages to send

public class ButtonPerformance extends AbstractPerformance{

    private List<PerformancePiece<byte[]>> performance;
    private HashSet<Integer> channels;
    public ButtonPerformance(int id, Button button, FaceSector faceSector, ProgressBar progressBar, TextView textView, int activeColor, int performToRecColor)
    {
        super(id, button,faceSector,progressBar, textView, activeColor, performToRecColor);
        performance = new ArrayList<>();
    }

    //utils functions
    public void deletePerformance()
    {
        super.deletePerformance();
        performance = new ArrayList<>();
        channels = new HashSet<>();
    }


    public void addPerformancePiece(byte[] action, int time)
    {
        if(performance.size() == 0)
            time = 0;
        performance.add(new PerformancePiece<>(action,time));
        setPerform(true);
        setDuration(getDuration() + time);
    }

    public void addPerformancePiece(byte[] action, int time, String text)
    {
        if(performance.size() == 0)
            time = 0;
        performance.add(new PerformancePiece<>(action,time, text));
        setPerform(true);
        setDuration(getDuration() + time);
    }

    public List<PerformancePiece<byte[]>> getPerformance()
    {
        return new ArrayList<>(performance);
    }

    public HashSet<Integer> getChannels(){ return channels;}

    public void compressMessage()
    {
        Log.i("PFGZ", "-- START COMPRESSING");
        Log.i("PFGZ","performance size : " + String.valueOf(performance.size()));
        Log.i("PFGZ" , "performance duration : " +String.valueOf(super.getDuration()));
        int pIndex = 0;

        //keep track of channel alst packet info:
        //direction: 0, not init 1 increasing -1 decreasing
        //time: epoch of last packet
        //value: value of lack packet
        HashMap<Integer,Integer> channelDirectionDict = new HashMap<>();
        HashMap<Integer,Long> channelTimeDict = new HashMap<>();
        HashMap<Integer,Integer> channelValueDict = new HashMap<>();

        //memorize channels affected by this performance, useful for precise filtering
        for(PerformancePiece<byte[]> p : performance) {
            channels.add(p.getChannelPin());
        }
        Log.i("PFGZ" , "channels recorded: " + channels.toString());
        long curentMessageTime = 0;

        for(PerformancePiece<byte[]> p : performance) {
            int pChannel = p.getChannelPin();
            long pTime = p.getMillisToAction();
            int pValue = p.getAnalogValue();

            Log.i("PFGZ_piece", "PIECE: " + p.toString()+ "- of length: "+String.valueOf(pTime));
            if (channelValueDict.containsKey(pChannel)){

                int prevDirection = channelDirectionDict.get(pChannel);
                long prevTime = channelTimeDict.get(pChannel);
                int prevValue = channelValueDict.get(pChannel);
                int currentDir = 0;
                if (pValue != prevValue) {
                    currentDir = (pValue - prevValue) / Math.abs(pValue - prevValue);
                }
                boolean dirChange = prevDirection != currentDir;
                if(!dirChange){
                    if(curentMessageTime + pTime - prevTime < Constants.DELAYTOERASEFORBTE){
                        p.eraseThis();
                    }
                }
                channelValueDict.put(pChannel,pValue);
                channelDirectionDict.put(pChannel,currentDir);
                channelTimeDict.put(pChannel,curentMessageTime);
            }else{
                channelValueDict.put(pChannel,pValue);
                channelDirectionDict.put(pChannel,0);
                channelTimeDict.put(pChannel,curentMessageTime);
            }
            Log.i("PFGZ_dicts", String.valueOf(channelValueDict));
            Log.i("PFGZ_dicts", String.valueOf(channelDirectionDict));
            Log.i("PFGZ_dicts", String.valueOf(channelTimeDict));
            curentMessageTime += pTime;
        }


        int timeToAddToOthers = 0;

        for(PerformancePiece<byte[]> p : performance) {
            if(p.isToErase()) {
                timeToAddToOthers += p.getMillisToAction();
            }else{
                p.addMillis(timeToAddToOthers);
                timeToAddToOthers = 0;
            }
        }



        Log.i("PFGZ_clear", "size before:" +  String.valueOf(performance.size()));
        Log.i("PFGZ_clear" , "duration before:" +   String.valueOf(super.getDuration()));
        performance.removeIf(p -> p.isToErase());


        int duration = 0;
        for(PerformancePiece<byte[]> p : performance) {
            duration += p.getMillisToAction();
        }
        super.setDuration(duration);

        Log.i("PFGZ_clear", "size after:" +  String.valueOf(performance.size()));
        Log.i("PFGZ_clear" , "duration after:" +   String.valueOf(super.getDuration()));
    }


}
