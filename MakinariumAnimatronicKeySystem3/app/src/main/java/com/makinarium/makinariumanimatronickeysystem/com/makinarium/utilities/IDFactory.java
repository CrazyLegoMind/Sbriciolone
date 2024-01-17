package com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities;

import com.makinarium.makinariumanimatronickeysystem.R;

import java.util.HashMap;

public class IDFactory {

    private static HashMap<Integer,Integer> buttonsLogicID;
    private static HashMap<Integer,Integer> textToButtonLogicID;

    public static void initializeButtons()
    {
        CounterClick counter = new CounterClick();


        buttonsLogicID = new HashMap<>();
        buttonsLogicID.put(R.id.preset_01, counter.click());
        buttonsLogicID.put(R.id.preset_02, counter.click());
        buttonsLogicID.put(R.id.preset_03, counter.click());
        buttonsLogicID.put(R.id.preset_04, counter.click());
        buttonsLogicID.put(R.id.preset_05, counter.click());
        buttonsLogicID.put(R.id.preset_06, counter.click());
        buttonsLogicID.put(R.id.preset_07, counter.click());
        buttonsLogicID.put(R.id.preset_08, counter.click());
        buttonsLogicID.put(R.id.preset_09, counter.click());
        buttonsLogicID.put(R.id.preset_10, counter.click());
        buttonsLogicID.put(R.id.preset_11, counter.click());
        buttonsLogicID.put(R.id.preset_12, counter.click());

        buttonsLogicID.put(R.id.row1Btn01, counter.click());
        buttonsLogicID.put(R.id.row1Btn02, counter.click());
        buttonsLogicID.put(R.id.row1Btn03, counter.click());
        buttonsLogicID.put(R.id.row1Btn04, counter.click());
        buttonsLogicID.put(R.id.row1Btn05, counter.click());
        buttonsLogicID.put(R.id.row1Btn06, counter.click());
        buttonsLogicID.put(R.id.row1Btn07, counter.click());
        buttonsLogicID.put(R.id.row1Btn08, counter.click());
        buttonsLogicID.put(R.id.row1Btn09, counter.click());
        buttonsLogicID.put(R.id.row1Btn10, counter.click());
        buttonsLogicID.put(R.id.row1Btn11, counter.click());
        buttonsLogicID.put(R.id.row1Btn12, counter.click());

        buttonsLogicID.put(R.id.row2Btn01, counter.click());
        buttonsLogicID.put(R.id.row2Btn02, counter.click());
        buttonsLogicID.put(R.id.row2Btn03, counter.click());
        buttonsLogicID.put(R.id.row2Btn04, counter.click());
        buttonsLogicID.put(R.id.row2Btn05, counter.click());
        buttonsLogicID.put(R.id.row2Btn06, counter.click());
        buttonsLogicID.put(R.id.row2Btn07, counter.click());
        buttonsLogicID.put(R.id.row2Btn08, counter.click());
        buttonsLogicID.put(R.id.row2Btn09, counter.click());
        buttonsLogicID.put(R.id.row2Btn10, counter.click());
        buttonsLogicID.put(R.id.row2Btn11, counter.click());
        buttonsLogicID.put(R.id.row2Btn12, counter.click());

        buttonsLogicID.put(R.id.row3Btn01, counter.click());
        buttonsLogicID.put(R.id.row3Btn02, counter.click());
        buttonsLogicID.put(R.id.row3Btn03, counter.click());
        buttonsLogicID.put(R.id.row3Btn04, counter.click());
        buttonsLogicID.put(R.id.row3Btn05, counter.click());
        buttonsLogicID.put(R.id.row3Btn06, counter.click());
        buttonsLogicID.put(R.id.row3Btn07, counter.click());
        buttonsLogicID.put(R.id.row3Btn08, counter.click());
        buttonsLogicID.put(R.id.row3Btn09, counter.click());
        buttonsLogicID.put(R.id.row3Btn10, counter.click());
        buttonsLogicID.put(R.id.row3Btn11, counter.click());
        buttonsLogicID.put(R.id.row3Btn12, counter.click());

        buttonsLogicID.put(R.id.row4Btn01, counter.click());
        buttonsLogicID.put(R.id.row4Btn02, counter.click());
        buttonsLogicID.put(R.id.row4Btn03, counter.click());
        buttonsLogicID.put(R.id.row4Btn04, counter.click());
        buttonsLogicID.put(R.id.row4Btn05, counter.click());
        buttonsLogicID.put(R.id.row4Btn06, counter.click());
        buttonsLogicID.put(R.id.row4Btn07, counter.click());
        buttonsLogicID.put(R.id.row4Btn08, counter.click());
        buttonsLogicID.put(R.id.row4Btn09, counter.click());
        buttonsLogicID.put(R.id.row4Btn10, counter.click());
        buttonsLogicID.put(R.id.row4Btn11, counter.click());
        buttonsLogicID.put(R.id.row4Btn12, counter.click());

        buttonsLogicID.put(R.id.row5Btn01, counter.click());
        buttonsLogicID.put(R.id.row5Btn02, counter.click());
        buttonsLogicID.put(R.id.row5Btn03, counter.click());
        buttonsLogicID.put(R.id.row5Btn04, counter.click());
        buttonsLogicID.put(R.id.row5Btn05, counter.click());
        buttonsLogicID.put(R.id.row5Btn06, counter.click());
        buttonsLogicID.put(R.id.row5Btn07, counter.click());
        buttonsLogicID.put(R.id.row5Btn08, counter.click());
        buttonsLogicID.put(R.id.row5Btn09, counter.click());
        buttonsLogicID.put(R.id.row5Btn10, counter.click());
        buttonsLogicID.put(R.id.row5Btn11, counter.click());
        buttonsLogicID.put(R.id.row5Btn12, counter.click());



        textToButtonLogicID = new HashMap<>();

        textToButtonLogicID.put(R.id.presetText01, getLogicID(R.id.preset_01));
        textToButtonLogicID.put(R.id.presetText02, getLogicID(R.id.preset_02));
        textToButtonLogicID.put(R.id.presetText03, getLogicID(R.id.preset_03));
        textToButtonLogicID.put(R.id.presetText04, getLogicID(R.id.preset_04));
        textToButtonLogicID.put(R.id.presetText05, getLogicID(R.id.preset_05));
        textToButtonLogicID.put(R.id.presetText06, getLogicID(R.id.preset_06));
        textToButtonLogicID.put(R.id.presetText07, getLogicID(R.id.preset_07));
        textToButtonLogicID.put(R.id.presetText08, getLogicID(R.id.preset_08));
        textToButtonLogicID.put(R.id.presetText09, getLogicID(R.id.preset_09));
        textToButtonLogicID.put(R.id.presetText10, getLogicID(R.id.preset_10));
        textToButtonLogicID.put(R.id.presetText11, getLogicID(R.id.preset_11));
        textToButtonLogicID.put(R.id.presetText12, getLogicID(R.id.preset_12));

        textToButtonLogicID.put(R.id.row1Text01, getLogicID(R.id.row1Btn01));
        textToButtonLogicID.put(R.id.row1Text02, getLogicID(R.id.row1Btn02));
        textToButtonLogicID.put(R.id.row1Text03, getLogicID(R.id.row1Btn03));
        textToButtonLogicID.put(R.id.row1Text04, getLogicID(R.id.row1Btn04));
        textToButtonLogicID.put(R.id.row1Text05, getLogicID(R.id.row1Btn05));
        textToButtonLogicID.put(R.id.row1Text06, getLogicID(R.id.row1Btn06));
        textToButtonLogicID.put(R.id.row1Text07, getLogicID(R.id.row1Btn07));
        textToButtonLogicID.put(R.id.row1Text08, getLogicID(R.id.row1Btn08));
        textToButtonLogicID.put(R.id.row1Text09, getLogicID(R.id.row1Btn09));
        textToButtonLogicID.put(R.id.row1Text10, getLogicID(R.id.row1Btn10));
        textToButtonLogicID.put(R.id.row1Text11, getLogicID(R.id.row1Btn11));
        textToButtonLogicID.put(R.id.row1Text12, getLogicID(R.id.row1Btn12));

        textToButtonLogicID.put(R.id.row2Text01, getLogicID(R.id.row2Btn01));
        textToButtonLogicID.put(R.id.row2Text02, getLogicID(R.id.row2Btn02));
        textToButtonLogicID.put(R.id.row2Text03, getLogicID(R.id.row2Btn03));
        textToButtonLogicID.put(R.id.row2Text04, getLogicID(R.id.row2Btn04));
        textToButtonLogicID.put(R.id.row2Text05, getLogicID(R.id.row2Btn05));
        textToButtonLogicID.put(R.id.row2Text06, getLogicID(R.id.row2Btn06));
        textToButtonLogicID.put(R.id.row2Text07, getLogicID(R.id.row2Btn07));
        textToButtonLogicID.put(R.id.row2Text08, getLogicID(R.id.row2Btn08));
        textToButtonLogicID.put(R.id.row2Text09, getLogicID(R.id.row2Btn09));
        textToButtonLogicID.put(R.id.row2Text10, getLogicID(R.id.row2Btn10));
        textToButtonLogicID.put(R.id.row2Text11, getLogicID(R.id.row2Btn11));
        textToButtonLogicID.put(R.id.row2Text12, getLogicID(R.id.row2Btn12));

        textToButtonLogicID.put(R.id.row3Text01, getLogicID(R.id.row3Btn01));
        textToButtonLogicID.put(R.id.row3Text02, getLogicID(R.id.row3Btn02));
        textToButtonLogicID.put(R.id.row3Text03, getLogicID(R.id.row3Btn03));
        textToButtonLogicID.put(R.id.row3Text04, getLogicID(R.id.row3Btn04));
        textToButtonLogicID.put(R.id.row3Text05, getLogicID(R.id.row3Btn05));
        textToButtonLogicID.put(R.id.row3Text06, getLogicID(R.id.row3Btn06));
        textToButtonLogicID.put(R.id.row3Text07, getLogicID(R.id.row3Btn07));
        textToButtonLogicID.put(R.id.row3Text08, getLogicID(R.id.row3Btn08));
        textToButtonLogicID.put(R.id.row3Text09, getLogicID(R.id.row3Btn09));
        textToButtonLogicID.put(R.id.row3Text10, getLogicID(R.id.row3Btn10));
        textToButtonLogicID.put(R.id.row3Text11, getLogicID(R.id.row3Btn11));
        textToButtonLogicID.put(R.id.row3Text12, getLogicID(R.id.row3Btn12));

        textToButtonLogicID.put(R.id.row4Text01, getLogicID(R.id.row4Btn01));
        textToButtonLogicID.put(R.id.row4Text02, getLogicID(R.id.row4Btn02));
        textToButtonLogicID.put(R.id.row4Text03, getLogicID(R.id.row4Btn03));
        textToButtonLogicID.put(R.id.row4Text04, getLogicID(R.id.row4Btn04));
        textToButtonLogicID.put(R.id.row4Text05, getLogicID(R.id.row4Btn05));
        textToButtonLogicID.put(R.id.row4Text06, getLogicID(R.id.row4Btn06));
        textToButtonLogicID.put(R.id.row4Text07, getLogicID(R.id.row4Btn07));
        textToButtonLogicID.put(R.id.row4Text08, getLogicID(R.id.row4Btn08));
        textToButtonLogicID.put(R.id.row4Text09, getLogicID(R.id.row4Btn09));
        textToButtonLogicID.put(R.id.row4Text10, getLogicID(R.id.row4Btn10));
        textToButtonLogicID.put(R.id.row4Text11, getLogicID(R.id.row4Btn11));
        textToButtonLogicID.put(R.id.row4Text12, getLogicID(R.id.row4Btn12));

        textToButtonLogicID.put(R.id.row5Text01, getLogicID(R.id.row5Btn01));
        textToButtonLogicID.put(R.id.row5Text02, getLogicID(R.id.row5Btn02));
        textToButtonLogicID.put(R.id.row5Text03, getLogicID(R.id.row5Btn03));
        textToButtonLogicID.put(R.id.row5Text04, getLogicID(R.id.row5Btn04));
        textToButtonLogicID.put(R.id.row5Text05, getLogicID(R.id.row5Btn05));
        textToButtonLogicID.put(R.id.row5Text06, getLogicID(R.id.row5Btn06));
        textToButtonLogicID.put(R.id.row5Text07, getLogicID(R.id.row5Btn07));
        textToButtonLogicID.put(R.id.row5Text08, getLogicID(R.id.row5Btn08));
        textToButtonLogicID.put(R.id.row5Text09, getLogicID(R.id.row5Btn09));
        textToButtonLogicID.put(R.id.row5Text10, getLogicID(R.id.row5Btn10));
        textToButtonLogicID.put(R.id.row5Text11, getLogicID(R.id.row5Btn11));
        textToButtonLogicID.put(R.id.row5Text12, getLogicID(R.id.row5Btn12));
    }


    public static int getLogicID(int rID)
    {
        return buttonsLogicID.get(rID);
    }

    public static int convertTextToButton(int trID)
    {
        return textToButtonLogicID.get(trID);
    }
}
