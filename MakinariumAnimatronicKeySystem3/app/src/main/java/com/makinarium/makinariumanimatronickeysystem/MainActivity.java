package com.makinarium.makinariumanimatronickeysystem;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v4.content.res.ResourcesCompat;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;


import com.google.gson.Gson;
import com.google.gson.reflect.TypeToken;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.presetthings.AbstractPerformance;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.presetthings.ButtonPerformance;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.presetthings.ButtonsContainer;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.presetthings.PerformancePiece;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.presetthings.PresetPerformance;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.undo.UndoManager;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities.Constants;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities.IDFactory;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.lang.reflect.Type;
import java.nio.charset.Charset;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public class MainActivity extends AppCompatActivity {


    private static final int stopMillisPerformance = 2;
    private static final String TAG = "Makinarium";
    private Button stopButton;
    private TimerForRecorder timeTask;
    private TextView cronometro;
    private boolean performRegistrationMode = false;
    private boolean presetRegistrationMode = false;
    private boolean mouthActiveController = true;
    private boolean eyesActiveController = true;
    private boolean canIChangeNames = false;

    private Switch eyesSwitch;

    private Switch mouthSwitch;
    private Switch nameSwitch;

    private ButtonsContainer<byte[]> container;
    private UndoManager<byte[]> undoManager;
    private ButtonPerformance bInRec;
    private PresetPerformance presetInRec;
    private long timePresetRec = 0;
    private HashSet<Integer> performanceFilter;

    private long previousPerformancePieceTime = 0;
    private String headMac;
    private String remote1Mac;
    private String remote2Mac;
    
    //--------- bluetooth variables for remotes and receivers
    private BluetoothAdapter mBluetoothAdapter;

    private BluetoothDevice mBTDeviceHead;
    private BluetoothConnectionService mBluetoothConnectionHead;
    
    
    private BluetoothDevice mBTDeviceEyes;
    private BluetoothConnectionService mBluetoothConnectionEyes;

    private BluetoothDevice mBTDeviceMouth;
    private BluetoothConnectionService mBluetoothConnectionMouth;


    //playback speed slider
    private double multiplicator = 1.0;
    private SeekBar multBar;
    private TextView multText;

    private TextView eyesStatus;
    private TextView headStatus;
    private TextView mouthStatus;
    private Gson gson;

    private int recActiveColor = 0;
    int recEmptyColor = 0;
    private int presetColor = 0;

    private Executor myExecutor;
    private CheckConnectionsThread checkThread;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        IDFactory.initializeButtons();
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.activity_main);
        LocalBroadcastManager.getInstance(this).registerReceiver(mReceiver, new IntentFilter((Constants.incomingMessageIntent)));


        Intent headIntent = getIntent();
        headMac = headIntent.getStringExtra("head_mac");
        remote1Mac = headIntent.getStringExtra("remote1_mac");
        remote2Mac = headIntent.getStringExtra("remote2_mac");

        recActiveColor = ResourcesCompat.getColor(getResources(), R.color.activePerform, null);

        recEmptyColor = ResourcesCompat.getColor(getResources(), R.color.performToRec, null);

        presetColor = ResourcesCompat.getColor(getResources(), R.color.firstcolumn, null);

        gson = new Gson();
        String filePath = Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS)+"/maks/"+Constants.SaveFileName;
        final File newFile = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS)+"/maks");
        newFile.mkdir();
        Log.i("FILE_L",filePath);

        try (FileInputStream inputStream = new FileInputStream(filePath)) {
            String json = IOUtils.toString(inputStream, "UTF-8");
            Log.i("FILE_L",json);
            Type containerType = new TypeToken<ButtonsContainer<byte[]>>(){}.getType();
            container = gson.fromJson(json, containerType);
            if(container != null) {

                Log.i("FILE_L","load_ok");
                container.setActiveColor(recActiveColor);
                container.setEmptyColor(recEmptyColor);
                initializeAllButtons();
                container.updateAllColorsAndNames();

            }
            else
            {
                Log.i("FILE_L","load_fail");
                container = new ButtonsContainer<>(recActiveColor, recEmptyColor);
                initializeAllButtons();
            }
        } catch (IOException e) {
            container = new ButtonsContainer<>(recActiveColor, recEmptyColor);
            initializeAllButtons();
        }

        undoManager = new UndoManager<>(container);

        performanceFilter = new HashSet<>();

        cronometro = findViewById(R.id.cronometro);
        stopButton = findViewById(R.id.stopButton);
        stopButton.setClickable(false);
        stopButton.setEnabled(false);
        
        eyesSwitch = findViewById(R.id.eyesSwitch);
        mouthSwitch = findViewById(R.id.mouthSwitch);
        nameSwitch = findViewById(R.id.nameSwitch);

        eyesSwitch.setChecked(true);
        mouthSwitch.setChecked(true);
        eyesSwitch.setOnCheckedChangeListener(this::onCheckEyes);
        mouthSwitch.setOnCheckedChangeListener(this::onCheckMouth);
        nameSwitch.setOnCheckedChangeListener(this::onCheckNames);

        multBar = findViewById(R.id.seekBar);
        multText = findViewById(R.id.multText);

        multBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener(){

            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                multiplicator = progress / 10.0;
                multText.setText(" ×"+multiplicator+" ");
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
        
        eyesStatus = findViewById(R.id.eyesStatus);
        mouthStatus = findViewById(R.id.mouthStatus);
        headStatus = findViewById(R.id.headStatus);

        checkThread = new CheckConnectionsThread(this,eyesStatus,headStatus,mouthStatus,recActiveColor,presetColor);

        connectionBluetooth();

        myExecutor = Executors.newFixedThreadPool(7);

        checkThread.start();
    }


    private void connectionBluetooth()
    {
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        for(BluetoothDevice bt : pairedDevices)
        {
            if (bt.getAddress().equals(headMac)) {
                Log.d(TAG, bt.getName());

                mBTDeviceHead = bt;
                mBluetoothConnectionHead = new BluetoothConnectionService(MainActivity.this, Constants.HeadID);
                startBTConnection(mBTDeviceHead, mBluetoothConnectionHead);
            }
            if (bt.getAddress().equals(remote1Mac)) {
                mBTDeviceEyes = bt;
                mBluetoothConnectionEyes = new BluetoothConnectionService(MainActivity.this, Constants.eyesID);
                startBTConnection(mBTDeviceEyes, mBluetoothConnectionEyes);
            }

            if (bt.getAddress().equals(remote2Mac)) {
                mBTDeviceMouth = bt;
                mBluetoothConnectionMouth = new BluetoothConnectionService(MainActivity.this, Constants.MouthID);
                startBTConnection(mBTDeviceMouth, mBluetoothConnectionMouth);
            }


        }
    }

    private void initializeAllButtons()
    {
        initializePresetButton(R.id.preset_01,R.id.presetPB01, R.id.presetText01);
        initializePresetButton(R.id.preset_02,R.id.presetPB02, R.id.presetText02);
        initializePresetButton(R.id.preset_03,R.id.presetPB03, R.id.presetText03);
        initializePresetButton(R.id.preset_04,R.id.presetPB04, R.id.presetText04);
        initializePresetButton(R.id.preset_05,R.id.presetPB05, R.id.presetText05);
        initializePresetButton(R.id.preset_06,R.id.presetPB06, R.id.presetText06);
        initializePresetButton(R.id.preset_07,R.id.presetPB07, R.id.presetText07);
        initializePresetButton(R.id.preset_08,R.id.presetPB08, R.id.presetText08);
        initializePresetButton(R.id.preset_09,R.id.presetPB09, R.id.presetText09);
        initializePresetButton(R.id.preset_10,R.id.presetPB10, R.id.presetText10);
        initializePresetButton(R.id.preset_11,R.id.presetPB11, R.id.presetText11);
        initializePresetButton(R.id.preset_12,R.id.presetPB12, R.id.presetText12);

        FaceSector row1sector = FaceSector.EAR;

        initializeButton(R.id.row1Btn01 , row1sector, R.id.row1PB01, R.id.row1Text01);
        initializeButton(R.id.row1Btn02 , row1sector, R.id.row1PB02, R.id.row1Text02);
        initializeButton(R.id.row1Btn03 , row1sector, R.id.row1PB03, R.id.row1Text03);
        initializeButton(R.id.row1Btn04 , row1sector, R.id.row1PB04, R.id.row1Text04);
        initializeButton(R.id.row1Btn05 , row1sector, R.id.row1PB05, R.id.row1Text05);
        initializeButton(R.id.row1Btn06 , row1sector, R.id.row1PB06, R.id.row1Text06);
        initializeButton(R.id.row1Btn07 , row1sector, R.id.row1PB07, R.id.row1Text07);
        initializeButton(R.id.row1Btn08 , row1sector, R.id.row1PB08, R.id.row1Text08);
        initializeButton(R.id.row1Btn09 , row1sector, R.id.row1PB09, R.id.row1Text09);
        initializeButton(R.id.row1Btn10 , row1sector, R.id.row1PB10, R.id.row1Text10);
        initializeButton(R.id.row1Btn11 , row1sector, R.id.row1PB11, R.id.row1Text11);
        initializeButton(R.id.row1Btn12 , row1sector, R.id.row1PB12, R.id.row1Text12);


        FaceSector row2sector = FaceSector.EYEBROWS;

        initializeButton(R.id.row2Btn01 , row2sector, R.id.row2PB01, R.id.row2Text01);
        initializeButton(R.id.row2Btn02 , row2sector, R.id.row2PB02, R.id.row2Text02);
        initializeButton(R.id.row2Btn03 , row2sector, R.id.row2PB03, R.id.row2Text03);
        initializeButton(R.id.row2Btn04 , row2sector, R.id.row2PB04, R.id.row2Text04);
        initializeButton(R.id.row2Btn05 , row2sector, R.id.row2PB05, R.id.row2Text05);
        initializeButton(R.id.row2Btn06 , row2sector, R.id.row2PB06, R.id.row2Text06);
        initializeButton(R.id.row2Btn07 , row2sector, R.id.row2PB07, R.id.row2Text07);
        initializeButton(R.id.row2Btn08 , row2sector, R.id.row2PB08, R.id.row2Text08);
        initializeButton(R.id.row2Btn09 , row2sector, R.id.row2PB09, R.id.row2Text09);
        initializeButton(R.id.row2Btn10 , row2sector, R.id.row2PB10, R.id.row2Text10);
        initializeButton(R.id.row2Btn11 , row2sector, R.id.row2PB11, R.id.row2Text11);
        initializeButton(R.id.row2Btn12 , row2sector, R.id.row2PB12, R.id.row2Text12);

        FaceSector row3sector = FaceSector.EYELIDS;

        initializeButton(R.id.row3Btn01 , row3sector, R.id.row3PB01, R.id.row3Text01);
        initializeButton(R.id.row3Btn02 , row3sector, R.id.row3PB02, R.id.row3Text02);
        initializeButton(R.id.row3Btn03 , row3sector, R.id.row3PB03, R.id.row3Text03);
        initializeButton(R.id.row3Btn04 , row3sector, R.id.row3PB04, R.id.row3Text04);
        initializeButton(R.id.row3Btn05 , row3sector, R.id.row3PB05, R.id.row3Text05);
        initializeButton(R.id.row3Btn06 , row3sector, R.id.row3PB06, R.id.row3Text06);
        initializeButton(R.id.row3Btn07 , row3sector, R.id.row3PB07, R.id.row3Text07);
        initializeButton(R.id.row3Btn08 , row3sector, R.id.row3PB08, R.id.row3Text08);
        initializeButton(R.id.row3Btn09 , row3sector, R.id.row3PB09, R.id.row3Text09);
        initializeButton(R.id.row3Btn10 , row3sector, R.id.row3PB10, R.id.row3Text10);
        initializeButton(R.id.row3Btn11 , row3sector, R.id.row3PB11, R.id.row3Text11);
        initializeButton(R.id.row3Btn12 , row3sector, R.id.row3PB12, R.id.row3Text12);

        FaceSector row4sector = FaceSector.SNOUT;

        initializeButton(R.id.row4Btn01 , row4sector, R.id.row4PB01, R.id.row4Text01);
        initializeButton(R.id.row4Btn02 , row4sector, R.id.row4PB02, R.id.row4Text02);
        initializeButton(R.id.row4Btn03 , row4sector, R.id.row4PB03, R.id.row4Text03);
        initializeButton(R.id.row4Btn04 , row4sector, R.id.row4PB04, R.id.row4Text04);
        initializeButton(R.id.row4Btn05 , row4sector, R.id.row4PB05, R.id.row4Text05);
        initializeButton(R.id.row4Btn06 , row4sector, R.id.row4PB06, R.id.row4Text06);
        initializeButton(R.id.row4Btn07 , row4sector, R.id.row4PB07, R.id.row4Text07);
        initializeButton(R.id.row4Btn08 , row4sector, R.id.row4PB08, R.id.row4Text08);
        initializeButton(R.id.row4Btn09 , row4sector, R.id.row4PB09, R.id.row4Text09);
        initializeButton(R.id.row4Btn10 , row4sector, R.id.row4PB10, R.id.row4Text10);
        initializeButton(R.id.row4Btn11 , row4sector, R.id.row4PB11, R.id.row4Text11);
        initializeButton(R.id.row4Btn12 , row4sector, R.id.row4PB12, R.id.row4Text12);

        FaceSector row5sector = FaceSector.MOUTH;

        initializeButton(R.id.row5Btn01 , row5sector, R.id.row5PB01, R.id.row5Text01);
        initializeButton(R.id.row5Btn02 , row5sector, R.id.row5PB02, R.id.row5Text02);
        initializeButton(R.id.row5Btn03 , row5sector, R.id.row5PB03, R.id.row5Text03);
        initializeButton(R.id.row5Btn04 , row5sector, R.id.row5PB04, R.id.row5Text04);
        initializeButton(R.id.row5Btn05 , row5sector, R.id.row5PB05, R.id.row5Text05);
        initializeButton(R.id.row5Btn06 , row5sector, R.id.row5PB06, R.id.row5Text06);
        initializeButton(R.id.row5Btn07 , row5sector, R.id.row5PB07, R.id.row5Text07);
        initializeButton(R.id.row5Btn08 , row5sector, R.id.row5PB08, R.id.row5Text08);
        initializeButton(R.id.row5Btn09 , row5sector, R.id.row5PB09, R.id.row5Text09);
        initializeButton(R.id.row5Btn10 , row5sector, R.id.row5PB10, R.id.row5Text10);
        initializeButton(R.id.row5Btn11 , row5sector, R.id.row5PB11, R.id.row5Text11);
        initializeButton(R.id.row5Btn12 , row5sector, R.id.row5PB12, R.id.row5Text12);
    }

    private void initializeButton(int id, FaceSector sector, int pbID, int textID)
    {
        Button b = findViewById(id);
        ProgressBar pb = findViewById(pbID);
        TextView t = findViewById(textID);
        b.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                performClick(v);
            }
        });

        b.setOnLongClickListener(new View.OnLongClickListener() {
            public boolean onLongClick(View v) {
                recClick(v);
                return true;
            }
        });

        container.addPerformanceButton(id, b, sector, pb, t);
    }

    private void initializePresetButton(int id, int pbID, int textID)
    {
        Button b = findViewById(id);
        ProgressBar pb = findViewById(pbID);
        TextView t = findViewById(textID);

        b.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {

                presetPerformClick(v);
            }
        });

        b.setOnLongClickListener(new View.OnLongClickListener() {
            public boolean onLongClick(View v) {
                presetRecClick(v);
                return true;
            }
        });

        container.addPresetButton(id, b, FaceSector.PRESET, pb, t);
    }

    /*
    private void testButton()//only for debug
    {

        byte[] arrayTest = {50,20,70};

        container.getButtonPerformance(R.id.eyes_01).addPerformancePiece(arrayTest,2000);
        container.getButtonPerformance(R.id.eyes_01).addPerformancePiece(arrayTest,1000);
        container.getButtonPerformance(R.id.eyes_02).addPerformancePiece(arrayTest,500);
        container.getButtonPerformance(R.id.eyes_02).addPerformancePiece(arrayTest,800);
        container.getButtonPerformance(R.id.mouth_01).addPerformancePiece(arrayTest,200);
        container.getButtonPerformance(R.id.mouth_01).addPerformancePiece(arrayTest,4000);
    }
    //*/

    public void startBTConnection(BluetoothDevice device, BluetoothConnectionService connection)
    {
        connection.startClient(device);
    }

    // Create a BroadcastReceiver for ACTION_FOUND
    private final BroadcastReceiver mBroadcastReceiver1 = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            // When discovery finds a device
            if (action.equals(mBluetoothAdapter.ACTION_STATE_CHANGED)) {
                final int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, mBluetoothAdapter.ERROR);

                switch(state){
                    case BluetoothAdapter.STATE_OFF:
                        Log.d(TAG, "onReceive: STATE OFF");
                        break;
                    case BluetoothAdapter.STATE_TURNING_OFF:
                        Log.d(TAG, "mBroadcastReceiver1: STATE TURNING OFF");
                        break;
                    case BluetoothAdapter.STATE_ON:
                        Log.d(TAG, "mBroadcastReceiver1: STATE ON");
                        break;
                    case BluetoothAdapter.STATE_TURNING_ON:
                        Log.d(TAG, "mBroadcastReceiver1: STATE TURNING ON");
                        break;
                }
            }
        }
    };

    @Override
    protected void onDestroy() {
        if(timeTask != null)
            timeTask.cancel(true);

        unregisterReceiver(mBroadcastReceiver1);
        unregisterReceiver(mReceiver);
        checkThread.stopChecking();
        mBluetoothConnectionEyes.stopClient();
        mBluetoothConnectionHead.stopClient();
        super.onDestroy();

    }

    //TODO: Toast con nome, occhio ai preset
    public void performClick(View v)
    {

        int id = v.getId();
        ButtonPerformance clickedButton = container.getButtonPerformance(id);

        if(!clickedButton.canPerform()) {
            Toast.makeText(this, Constants.emptyPerformance, Toast.LENGTH_SHORT).show();
            return;
        }

        Toast.makeText(this, Constants.performing, Toast.LENGTH_SHORT).show();
        container.deactivatesButtonSectorButton(clickedButton.getFaceSector());

        if(presetRegistrationMode) {
            presetInRec.setButtonPerformance(container.getButtonPerformance(id));
            timePresetRec = System.currentTimeMillis();
        }

        performanceFilter.addAll(clickedButton.getChannels());
        performanceThread pt = new performanceThread();
        pt.executeOnExecutor(myExecutor,clickedButton);
        //pt.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, bp);

    }

    public void recClick(View v)
    {

        if(presetRegistrationMode)
            return;

        Toast.makeText(this, Constants.RegistrationString, Toast.LENGTH_LONG).show();

        //timeTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
        stopButton.setClickable(true);
        stopButton.setEnabled(true);

        performRegistrationMode = true;
        bInRec = container.getButtonPerformance(v.getId());
        undoManager.addLastEdit(bInRec);
        bInRec.deletePerformance();
        previousPerformancePieceTime = System.currentTimeMillis();

        if(timeTask != null)
            timeTask.killTimer();
        timeTask = new TimerForRecorder();
        timeTask.executeOnExecutor(myExecutor,bInRec);
    }


    public void presetRecClick(View v)
    {
        Toast.makeText(this, Constants.RegistrationString, Toast.LENGTH_LONG).show();

        //timeTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
        stopButton.setClickable(true);
        stopButton.setEnabled(true);
        timePresetRec = System.currentTimeMillis();

        presetRegistrationMode = true;
        presetInRec = container.getPresetPerformance(v.getId());
        undoManager.addLastEdit(presetInRec);
        presetInRec.deletePerformance();
        previousPerformancePieceTime = System.currentTimeMillis();

        if(timeTask != null)
            timeTask.killTimer();
        timeTask = new TimerForRecorder();
        timeTask.executeOnExecutor(myExecutor, presetInRec);
    }

    public void presetPerformClick(View v)
    {
        int id = v.getId();
        PresetPerformance bp = container.getPresetPerformance(id);
        if(!bp.canPerform())
        {
            Toast.makeText(this, Constants.emptyPerformance, Toast.LENGTH_SHORT).show();
            return;
        }
        Toast.makeText(this, Constants.performing, Toast.LENGTH_SHORT).show();
        container.deactivatesButtonSectorButton(bp.getFaceSector());
        preSetThread pt = new preSetThread();
        pt.executeOnExecutor(myExecutor, bp);
        //pt.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR, bp);
    }

    public void speedBtnClick(View v){
        int progress = Integer.valueOf((String) v.getTag());
        multiplicator = (double) progress / 10.0;
        multBar.setProgress(progress);
    }


    public void stopButton(View v)
    {
        timeTask.killTimer();
        timeTask = null;
        performRegistrationMode = false;
        presetRegistrationMode = false;
        if(bInRec != null) {
            bInRec.updateColor();
            bInRec.compressMessage();
        }
        bInRec = null;

        if(presetInRec != null)
            presetInRec.updateColor();
        presetInRec = null;
        stopButton.setClickable(false);
        stopButton.setEnabled(false);
        timePresetRec = 0;
        Toast.makeText(this, Constants.stopRecording, Toast.LENGTH_LONG).show();
        container.saveMe(this, gson);

    }

    private void hideSystemUI() {
        // Enables regular immersive mode.
        // For "lean back" mode, remove SYSTEM_UI_FLAG_IMMERSIVE.
        // Or for "sticky immersive," replace it with SYSTEM_UI_FLAG_IMMERSIVE_STICKY
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE
                        // Set the content to appear under the system bars so that the
                        // content doesn't resize when the system bars hide and show.
                        | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                        // Hide the nav bar and status bar
                        | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_FULLSCREEN
                          );
    }

    // Shows the system bars by removing all the flags
// except for the ones that make the content appear under the system bars.
    private void showSystemUI() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                        | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                        | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.mainmenu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int itemThatWasClickedId = item.getItemId();

        if (itemThatWasClickedId == R.id.action_changehead) {
            Context context = MainActivity.this;
            Intent mainActivityIntent = new Intent(context, headActivity.class);
            startActivity(mainActivityIntent);
            return true;
        }

        if (itemThatWasClickedId == R.id.action_undo) {
            Context context = MainActivity.this;
            String textToShow =  undoManager.undo() ? "undo done" : "There was nothing to undo";
            container.saveMe(this, gson);
            Toast.makeText(context, textToShow, Toast.LENGTH_SHORT).show();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }


    public class TimerForRecorder extends AsyncTask<AbstractPerformance, String, String> {
        private final String zeroedTime = "00:00:00";
        private long performanceStartTime = -1;
        private AbstractPerformance registeringPerformance;
        private boolean blinkState = true;
        private int timeDarkColor = ResourcesCompat.getColor(getResources(), R.color.textDark, null);
        private int timeLightColor = ResourcesCompat.getColor(getResources(), R.color.textLight, null);
        private int timeColorCurrent = timeDarkColor;
        private int timeColor = timeDarkColor;
        private boolean stop = false;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
        }

        @Override
        protected String doInBackground(AbstractPerformance... params) {
            long startTime = System.currentTimeMillis();
            registeringPerformance = params[0];
            while(!isCancelled() && !stop) {
                //show elapsed time while registering input
                if(registeringPerformance.canPerform()) {
                    if (performanceStartTime <= 0){
                        performanceStartTime = System.currentTimeMillis();
                        timeColor = timeDarkColor;
                        publishProgress(zeroedTime);
                    }

                    try {
                        Thread.sleep(10);
                    } catch (InterruptedException e) {
                        //e.printStackTrace();
                        Log.i("TIMERTHREAD", "timer stopped");
                    }
                    long timePassed = System.currentTimeMillis() - performanceStartTime;
                    String timeString = generateTimerString(timePassed);
                    publishProgress(timeString);

                }else{ //blink the timer while waiting for input
                    long timePassed = System.currentTimeMillis() - startTime;
                    if(timePassed % 1000 > 500) {
                        if (blinkState) {
                            blinkState = false;
                            timeColor = timeDarkColor;
                            publishProgress(zeroedTime);
                        }
                    }else{
                        if (!blinkState) {
                            blinkState = true;
                            timeColor = timeLightColor;
                            publishProgress(zeroedTime);
                        }
                    }
                }
            }
            return null;
        }

        @Override
        protected void onProgressUpdate(String...values) {
            //String timeStr = " " + printNumberForTimer(values[0]) + ":" + printNumberForTimer(values[1]) + ":" + values[2] + values[3];
            cronometro.setText(values[0]);
            if (timeColor != timeColorCurrent) {
                cronometro.setTextColor(timeColor);
                timeColorCurrent = timeColor;
            }
        }

        @Override
        protected void onPostExecute(String result) {
            cronometro.setTextColor(timeDarkColor);
            if(performanceStartTime > 0 ){
                String timeString = generateTimerString(previousPerformancePieceTime-performanceStartTime);
                Log.i("TIMER_POST",String.valueOf(previousPerformancePieceTime-performanceStartTime)+" prev: "+previousPerformancePieceTime);
                cronometro.setText(timeString);
                return;
            }
            cronometro.setText(zeroedTime);
        }

        //utils

        private String generateTimerString(long millisElapsed){
            if (millisElapsed < 0){
                return zeroedTime;
            }
            String timerString = "";
            millisElapsed /= 10;
            int centesimi = (int) millisElapsed % 10;
            millisElapsed /= 10;
            int decimi = (int) millisElapsed % 10;
            millisElapsed /= 10;
            int seconds = (int) millisElapsed % 60;
            int minutes = (int) millisElapsed / 60;
            timerString = twodigitFromInt(minutes)+":" + twodigitFromInt(seconds)+":" + String.valueOf(decimi) + String.valueOf(centesimi);
            return timerString;
        }
        private String twodigitFromInt(int n) {
            if(n > 9)
                return ""+n;
            return "0"+n;
        }

        public void killTimer(){
            stop = true;
        }
    }

    private void addPerforamancePieceToRec(byte[] action, String text)
    {
        int time = (int) (System.currentTimeMillis() - previousPerformancePieceTime);
        previousPerformancePieceTime = System.currentTimeMillis();

        bInRec.addPerformancePiece(action, time, text);
    }


    public void onCheckEyes(CompoundButton buttonView, boolean isChecked) {
        if(isChecked) {
            eyesActiveController = true;
            Toast.makeText(this, Constants.controllerActivated, Toast.LENGTH_LONG).show();
        } else {
            eyesActiveController = false;
            Toast.makeText(this, Constants.controllerDisactived, Toast.LENGTH_LONG).show();
        }
    }

    public void onCheckMouth(CompoundButton buttonView, boolean isChecked) {
        if(isChecked) {
            mouthActiveController = true;
            Toast.makeText(this, Constants.controllerActivated, Toast.LENGTH_LONG).show();
        } else {
            mouthActiveController = false;
            Toast.makeText(this, Constants.controllerDisactived, Toast.LENGTH_LONG).show();
        }
    }
    

    public void onCheckNames(CompoundButton buttonView, boolean isChecked) {
        if(isChecked) {
            canIChangeNames = true;
            Toast.makeText(this, Constants.YesICAN, Toast.LENGTH_LONG).show();
        } else {
            canIChangeNames = false;
            Toast.makeText(this, Constants.NOICAN, Toast.LENGTH_LONG).show();
        }
    }


    BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            int id = intent.getIntExtra(Constants.intentIDProp, 0);
            String text = intent.getStringExtra("Message");
            long currentTime = System.currentTimeMillis();
            switch (id){
                case Constants.eyesID:
                    checkThread.setLastTimeEyesAlive(currentTime);
                    break;
                case Constants.HeadID:
                    checkThread.setLastTimeHeadAlive(currentTime);
                    break;
                case Constants.MouthID:
                    checkThread.setLastTimeMouthAlive(currentTime);
                    break;
                default:
                    break;
            }

            if(text.contains("ALI"))
                return;
            byte[] bytes;

            FaceSector f = FaceSector.fromChar(text.charAt(0));
            int packetChannel = PerformancePiece.getChannelFromPacket(text);
            if(performanceFilter.contains(packetChannel))
                return;

            Log.i("BT_REC: ",text);
            switch (id){
                case Constants.eyesID:
                    if(!eyesActiveController)
                        return;
                    bytes = text.getBytes(Charset.defaultCharset());

                    if(performRegistrationMode)
                    {
                        if(f == bInRec.getFaceSector())
                            addPerforamancePieceToRec(bytes, text);
                    }
                    if (checkThread.getReceiverStatus()) {
                        mBluetoothConnectionHead.write(bytes);
                    }
                    break;
                case Constants.MouthID:
                    if(!mouthActiveController)
                        return;
                    bytes = text.getBytes(Charset.defaultCharset());

                    if(performRegistrationMode)
                    {
                        if(f == bInRec.getFaceSector())
                            addPerforamancePieceToRec(bytes, text);
                    }
                    if (checkThread.getReceiverStatus()) {
                        mBluetoothConnectionHead.write(bytes);
                    }
                    break;
                case Constants.HeadID:
                    //IMPOSSIBLE
                    break;
                default:
                    break;
            }
        }
    };


    public class performanceThread extends AsyncTask<ButtonPerformance, Integer, ButtonPerformance> {

        private ButtonPerformance bpThread;

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
        }

        @Override
        protected ButtonPerformance doInBackground(ButtonPerformance... params) {

            bpThread = params[0];
            List<PerformancePiece<byte[]>> performance = bpThread.getPerformance();
            double playback_multiplier = multiplicator;
            long btn_start_us = System.currentTimeMillis()*1000;
            long btn_current_us = btn_start_us;
            Log.i("PERFT_G","start: "+ btn_start_us/1000+" with duration:"+bpThread.getDuration()/ playback_multiplier);
            double mslostcount = 0;
            boolean inPerformance = true;
            PerformancePiece<byte[]> currentPiece = performance.get(0);


            int currentIndex = 0;
            long send_time = 0;
            long us_to_action_left = 0;
            long next_action_us = btn_start_us + (long)(currentPiece.getMillisToAction()*1000 / playback_multiplier);
            while(inPerformance)
            {

                try {
                    btn_current_us = System.currentTimeMillis()*1000;
                    us_to_action_left = 0;
                    if(next_action_us > btn_current_us){
                        us_to_action_left = next_action_us - btn_current_us;
                    }
                    if(us_to_action_left >2000){
                        long pre_time =  System.currentTimeMillis();
                        Thread.sleep(2);
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                if(btn_current_us > next_action_us)
                {
                    byte[] bytes = currentPiece.getAction();
                    byte[] bytesToSend;
                    String messageToSend = new String(bytes);
                    messageToSend = "r" + messageToSend;
                    bytesToSend = messageToSend.getBytes(Charset.defaultCharset());
                    if (checkThread.getReceiverStatus()) {
                        mBluetoothConnectionHead.write(bytesToSend);
                    }
                    //Log.i("PERFT_P","i" +currentIndex);
                    currentIndex++;

                    if(currentIndex >= performance.size()) {
                        long realDuration = (btn_current_us - btn_start_us)/1000;
                        Log.i("PERFT_G", "end-> dur: " + (realDuration)+" r_err: "+(realDuration-bpThread.getDuration()/playback_multiplier));
                        Log.i("PERFT_P", "pkg/s: " +( performance.size()/(realDuration/1000)));
                        inPerformance = false;
                    }else{
                        currentPiece = performance.get(currentIndex);
                        long wait = (long)((currentPiece.getMillisToAction()*1000 ) / playback_multiplier);
                        next_action_us = next_action_us + wait;
                    }
                }
                long btn_elapsed_us = btn_current_us - btn_start_us;
                int percentProgress = (int) ((100 * btn_elapsed_us) / (int)(bpThread.getDuration()*1000 / playback_multiplier));


                publishProgress(percentProgress);
                send_time = System.currentTimeMillis()*1000 - btn_current_us;
            }
            return bpThread;
        }

        @Override
        protected void onProgressUpdate(Integer... values)
        {
            bpThread.getProgressBar().setProgress(values[0]);
        }

        @Override
        protected void onPostExecute(ButtonPerformance bp) {

            container.activatesButtonSectorButton(bp.getFaceSector());
            performanceFilter.removeAll(bp.getChannels());
            bpThread.getProgressBar().setProgress(0);
        }

    }

    public class preSetThread extends AsyncTask<PresetPerformance, Integer, PresetPerformance> {

        private PresetPerformance bpThread;
        @Override
        protected PresetPerformance doInBackground(PresetPerformance... presetPerformances) {
            bpThread = presetPerformances[0];
            int duration = (int)(bpThread.getDuration() / multiplicator);
            List<Integer> buttonToPress = bpThread.getButtonsToPress();
            publishProgress(buttonToPress.toArray(new Integer[buttonToPress.size() + 1]));//Trucchetto se ho solo un bottone da premere
            long startTime = System.currentTimeMillis();
            long time = startTime;
            long endTime = time + duration;

            while(time < endTime)
            {
                try {
                    Thread.sleep(stopMillisPerformance);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                time = System.currentTimeMillis();
                long progressTime = time - startTime;
                int percentProgress = (int) ((100 * progressTime) / (int)(bpThread.getDuration() / multiplicator));
                publishProgress(percentProgress);
            }
            return null;
        }

        @Override
        protected void onProgressUpdate(Integer... values)
        {

            if(values.length > 1)
            {
                for(int i = 0; i < values.length - 1; i++)//Trucchetto se ho solo un bottone da premere
                {
                    Button b = container.getButtonPerformanceFromLogic(values[i]).getButton();
                    b.performClick();
                }
                return;

            }

            bpThread.getProgressBar().setProgress(values[0]);
        }


        @Override
        protected void onPostExecute(PresetPerformance p) {
            container.activatesButtonSectorButton(bpThread.getFaceSector());
            bpThread.getProgressBar().setProgress(0);
        }

    }


    public void changeName(final View v) {

        if(!canIChangeNames)
            return;

        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setTitle("Change button name");
// I'm using fragment here so I'm using getView() to provide ViewGroup
// but you can provide here any other instance of ViewGroup from your Fragment / Activity
        ViewGroup view = findViewById(android.R.id.content);
        View viewInflated = LayoutInflater.from(this).inflate(R.layout.popupname, view, false);
// Set up the input
        final EditText input = viewInflated.findViewById(R.id.insertName);
// Specify the type of input expected; this, for example, sets the input as a password, and will mask the text
        builder.setView(viewInflated);

// Set up the buttons
        builder.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.dismiss();
                AbstractPerformance ap = container.getGenericAbstractPerformance(IDFactory.convertTextToButton(v.getId()));
                undoManager.addLastEdit(ap, ap.getName());
                ap.setName(input.getText().toString());
                container.saveMe(getBaseContext() ,gson);
            }
        });
        builder.setNegativeButton(android.R.string.cancel, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.cancel();
            }
        });

        builder.show();
    }



    public void shutdownServo(final View v) {
        String messageToSend = "G;0;78\n";
        messageToSend = "r" + messageToSend;
        byte[] bytesToSend = messageToSend.getBytes(Charset.defaultCharset());
        Log.i("SERVO_STOP","sending: "+ messageToSend);
        if (checkThread.getReceiverStatus()) {
            mBluetoothConnectionHead.write(bytesToSend);
        }
    }

}
