package com.makinarium.makinariumanimatronickeysystem.com.makinarium.presetthings;

import com.makinarium.makinariumanimatronickeysystem.MessageTypes;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities.Constants;

//packet handling class, stores a single action tied to
//a servo and a value

public class PerformancePiece <T>{

    private T action;
    private transient String stringVersion;
    private transient MessageTypes type;
    private transient int channelPin;
    private transient int analogValue;
    private transient int checkSum;
    private transient boolean toErase = false;
    private int millisToAction = -1;

    public PerformancePiece(T action, int millisToAction)
    {
        this.action = action;
        this.millisToAction = millisToAction;
    }

    public PerformancePiece(T action, int millisToAction, String message)
    {
        this.action = action;
        this.millisToAction = millisToAction;
        this.stringVersion = message;
        parseMessage();
    }

    //utils functions
    private void parseMessage()
    {
        type = MessageTypes.fromChar(stringVersion.charAt(1));
        switch(type)
        {
            case SERVO:
                String[] array = stringVersion.split(""+Constants.SEPARATOR);
                try{
                    channelPin = Integer.valueOf(array[1]);
                    analogValue = Integer.valueOf(array[2]);
                    checkSum = Integer.valueOf(array[3]);
                }
                catch(Exception e)
                {

                }
                break;
            case STATUSCHANGE:
                break;
            case EVENT:
                break;
            case ERROR:
                break;
            default:
                break;
        }
    }

    public PerformancePiece(PerformancePiece<T> toCopy)
    {
        this.action = toCopy.getAction();
        this.millisToAction = toCopy.getMillisToAction();
    }

    public void addMillis(int millis)
    {
        millisToAction += millis;
    }

    public void eraseThis() {
        toErase = true;
    }

    protected byte[] getBytes() {
        return stringVersion.getBytes();
    }

    private void updateStringVersion() {
        this.stringVersion = "" + stringVersion.charAt(0) + stringVersion.charAt(1) + Constants.SEPARATOR +
                channelPin + Constants.SEPARATOR + analogValue + Constants.SEPARATOR;

        char[] beforeCheckSum = stringVersion.toCharArray();
        int sum = 0;
        for (char c : beforeCheckSum)
            sum += c;
        int checksum = sum % 100;
        this.stringVersion += checksum;
    }

    protected void setAnalogStringAndChecksum(int analogValue, String stringVersion, int checkSum) {
        this.analogValue = analogValue;
        this.stringVersion = stringVersion;
        this.checkSum = checkSum;



    }



    //setters and getters

    public void setAction(T action) {
        this.action = action;
    }

    public T getAction() {
        return action;
    }

    public int getMillisToAction() {
        return millisToAction;
    }

    public int getChannelPin() {
        return channelPin;
    }

    protected void setAnalogValue(int analogValue) {
        this.analogValue = analogValue;
        //updateStringVersion();
    }

    public int getAnalogValue() {
        return analogValue;
    }

    public boolean isToErase() {
        return toErase;
    }

    public MessageTypes getType() {
        return type;
    }

    public String getStringVersion() {
        return stringVersion;
    }

    public int getCheckSum() {
        return checkSum;
    }



    //object overrides
    @Override
    public String toString() {
        return stringVersion;
    }

}
