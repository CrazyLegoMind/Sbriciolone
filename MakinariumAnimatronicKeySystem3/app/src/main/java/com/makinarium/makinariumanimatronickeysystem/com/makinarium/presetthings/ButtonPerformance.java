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
import java.util.List;


//handler class for for single animations, not presets
//holds a performance, so list of messages to send

public class ButtonPerformance extends AbstractPerformance{

    private List<PerformancePiece<byte[]>> performance;

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


    public void compressMessage()
    {
        Log.i("PFGZ", "-- START COMPRESSING");
        Log.i("PFGZ","performance size : " + String.valueOf(performance.size()));
        Log.i("PFGZ" , "performance duration : " +String.valueOf(super.getDuration()));
        int pIndex = 0;
        HashMap<Integer,Boolean> first = new HashMap<>();

        //init an array to ignore the first packet for each channel
        for(PerformancePiece<byte[]> p : performance) {
            first.put(p.getChannelPin(), true);
        }
        Log.i("PFGZ" , "channels recorded: " + first.toString());

        for(PerformancePiece<byte[]> p : performance) {
            int pChannel = p.getChannelPin();

            Log.i("PFGZ_piece", "PIECE: " + p.toString()+ "- of length: "+String.valueOf(p.getMillisToAction()));

            //ignore only if first packet
            if (first.get(pChannel)) {
                first.put(pChannel, false);
                pIndex++;
                Log.i("PFGZ_piece","failed mask");
                continue;
            }

            //if this piece is to erase or not a servo command skip to next piece
            if(p.isToErase() || !(p.getType() == MessageTypes.SERVO)) {
                pIndex++;
                Log.i("PFGZ_piece","erased");
                continue;
            }

            PerformancePiece<byte[]> y = null;
            PerformancePiece<byte[]> z = null;

            int millisFuture = 0;
            AnalogDirection direction = AnalogDirection.UP;
            //compress from the second packet of a channel to the end of animation
            //check only packet after this one
            for(int i = pIndex + 1; i < performance.size(); i++) {
                PerformancePiece piece = performance.get(i);



                //keep track of the timings
                millisFuture += piece.getMillisToAction();

                //if the piece is not from this own channel
                //is to be clear or is not a servo command
                //skip it so analyze next piece
                if(!(piece.getType() == MessageTypes.SERVO) ||
                        piece.getChannelPin() != pChannel ||
                        piece.isToErase())
                    continue;
                //now the piece is good
                Log.i("PFGZ_piece",  "- loop with: " + piece.toString());
                //when enough time is passed from the last good piece
                //exit the for and modify the performance

                if(millisFuture > Constants.DELAYTOERASEFORBTE)
                    if(piece.getMillisToAction() > Constants.LITTLEDELAY)
                        break;

                //if not enough time is passed i keep track of direction changes
                if(y == null) {
                    //if no packet is set i memorize it and init the direction
                    y = piece;
                    y.eraseThis();
                    int diff = p.getAnalogValue() - piece.getAnalogValue();
                    if(diff < 0)
                        direction = AnalogDirection.UP; // diff is negative
                    else
                        direction = AnalogDirection.DOWN; //diff is positive

                } else {
                    //if I already have a packet check for direction changes
                    //if direction changes exit the loop
                    int diff = y.getAnalogValue() - piece.getAnalogValue();
                    if(direction == AnalogDirection.UP && diff > 0)
                        break;
                    if(direction == AnalogDirection.DOWN && diff < 0)
                        break;
                    //erase only if there was no direction change
                    z = piece;
                    z.eraseThis();
                }
            }
            //after the for loop

            Log.i("PFGZ_piece",  "PIECE: " + p.toString()+ "for loop passed");
            if(z!= null) {
                Log.i("PFGZ_piece",  "PIECE: " + p.toString()+ "overwriting with z");
                p.setAnalogStringAndChecksum(z.getAnalogValue(), z.getStringVersion(), z.getCheckSum());
                p.setAction(z.getBytes());
            } else if(y != null) {
                Log.i("PFGZ_piece",  "PIECE: " + p.toString()+ "overwriting with y");
                p.setAnalogStringAndChecksum(y.getAnalogValue(), y.getStringVersion(), y.getCheckSum());
                p.setAction(y.getBytes());
            }

            pIndex++;
        }




        int timeToAddToOthers = 0;

        //Log.i("MILLIS", "AHUAHSUGFHKDKGFHSGSF \n\n");

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
