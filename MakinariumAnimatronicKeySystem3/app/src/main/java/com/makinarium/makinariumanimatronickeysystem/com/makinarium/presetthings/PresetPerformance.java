package com.makinarium.makinariumanimatronickeysystem.com.makinarium.presetthings;

import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.makinarium.makinariumanimatronickeysystem.FaceSector;

import java.util.ArrayList;
import java.util.List;

//handler class for animation presets, holding all linked
//button performances to start when starting the animation

public class PresetPerformance extends AbstractPerformance{
    private ButtonPerformance row1Btn;
    private ButtonPerformance row2Btn;
    private ButtonPerformance row3Btn;
    private ButtonPerformance row4Btn;
    private ButtonPerformance row5Btn;

    private int runningBtnCounter = 0;

    public PresetPerformance(int id, Button button, FaceSector faceSector, ProgressBar progressBar,
                             TextView textView, int activeColor, int performToRecColor)
    {
        super(id, button,faceSector,progressBar, textView, activeColor, performToRecColor);
    }


    public void setButtonPerformance(ButtonPerformance bp)
    {
        switch (bp.getFaceSector())
        {
            case EAR:
                row1Btn = bp;
                break;
            case EYEBROWS:
                row2Btn = bp;
                break;
            case EYELIDS:
                row3Btn = bp;
                break;
            case SNOUT:
                row4Btn = bp;
                break;
            case MOUTH:
                row5Btn = bp;
                break;
                default:
                    return;
        }

        setPerform(true);

        if(bp.getDuration() > this.getDuration())
            setDuration(bp.getDuration());
    }


    public void deletePerformance()
    {
        super.deletePerformance();
        row1Btn = null;
        row2Btn = null;
        row3Btn = null;
        row4Btn = null;
        row5Btn = null;
    }


    public List<Integer> getButtonsToPress()
    {
        List result = new ArrayList<>();

        if(row1Btn != null)
            result.add(row1Btn.getId());
        if(row2Btn != null)
            result.add(row2Btn.getId());
        if(row3Btn != null)
            result.add(row3Btn.getId());
        if(row4Btn != null)
            result.add(row4Btn.getId());
        if(row5Btn != null)
            result.add(row5Btn.getId());

        return result;
    }
    public void tickRunningBtnCounter(){
        if (runningBtnCounter <= 1){
            runningBtnCounter = 0;
            stopThread();
        }else{
            runningBtnCounter = runningBtnCounter -1;
        }
    }

    public void initRunning(){
        runningBtnCounter = 0;
        if(row1Btn != null)
            runningBtnCounter = runningBtnCounter +1;
        if(row2Btn != null)
            runningBtnCounter = runningBtnCounter +1;
        if(row3Btn != null)
            runningBtnCounter = runningBtnCounter +1;
        if(row4Btn != null)
            runningBtnCounter = runningBtnCounter +1;
        if(row5Btn != null)
            runningBtnCounter = runningBtnCounter +1;
        threadRunning = true;
    }
}
