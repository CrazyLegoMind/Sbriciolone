package com.makinarium.makinariumanimatronickeysystem.com.makinarium.presetthings;

import android.widget.Button;
import android.widget.ProgressBar;

import com.makinarium.makinariumanimatronickeysystem.FaceSector;

import java.util.HashMap;

public class ButtonsContainer<T> {

    private HashMap<Integer,ButtonPerformance<T>> mapButtons;

    public ButtonsContainer()
    {
        mapButtons = new HashMap<>();
    }


    public void addButton(int id, Button button, FaceSector sector, ProgressBar progressBar)
    {
        mapButtons.put(id, new ButtonPerformance(button,sector, progressBar));
    }

    public void deactivatesButtonSectorButton(FaceSector sector)
    {
        disActButtons(sector,false);
    }

    public void activatesButtonSectorButton(FaceSector sector)
    {
        disActButtons(sector,true);
    }

    private void disActButtons(FaceSector sector, boolean whatToDo)
    {
        for (Integer id: mapButtons.keySet()) {
            if(mapButtons.get(id).getFaceSector() != sector)
                continue;
            if(whatToDo)
                mapButtons.get(id).activatesButton();
            else
                mapButtons.get(id).deactivatesButton();

        }
    }


    public ButtonPerformance<T> getButtonPerform(int id)
    {
        return mapButtons.get(id);
    }

    public void deactivatesAllButtons()
    {
        for(FaceSector sector : FaceSector.values())
            disActButtons(sector,false);
    }

    public void activatesAllButtons()
    {
        for(FaceSector sector : FaceSector.values())
            disActButtons(sector,true);
    }

}