package com.makinarium.makinariumanimatronickeysystem;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.Spinner;

import com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities.Constants;

import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public class headActivity extends AppCompatActivity {
    private enum SpinnerType {REMOTE,RECEIVER}
    private Button continueButton;
    private String headMac;
    private String remote1Mac = Constants.macEyesBT;
    private String remote2Mac = Constants.macMouthBT;
    private EditText remoteEdit;
    private EditText headEdit;
    private List<String> macArray;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_head);
        continueButton = (Button)findViewById(R.id.continuebutton);
        remoteEdit = findViewById(R.id.editremote1Mac);
        headEdit = findViewById(R.id.editHeadMac);
        continueButton.setAlpha(.5f);
        continueButton.setEnabled(false);
        continueButton.setClickable(false);

        List<String> spinnerArray = new ArrayList<>();
        macArray = new ArrayList<>();
        BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        for(BluetoothDevice bt : pairedDevices)
        {
            String deviceStr = "";
            deviceStr += bt.getName() + " - ";
            deviceStr += bt.getAddress();
            macArray.add(bt.getAddress());
            spinnerArray.add(deviceStr);
        }

        ArrayAdapter<String> adapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, spinnerArray);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        Spinner remote1MacSpinner = (Spinner) findViewById(R.id.remote1Macspinner);
        Spinner receiverMacSpinner = (Spinner) findViewById(R.id.receivermacspinner);
        remote1MacSpinner.setAdapter(adapter);
        remote1MacSpinner.setOnItemSelectedListener(new MacSpinnerActivity(SpinnerType.REMOTE));
        receiverMacSpinner.setAdapter(adapter);
        receiverMacSpinner.setOnItemSelectedListener(new MacSpinnerActivity(SpinnerType.RECEIVER));
    }


    public void onRadioButtonClicked(View view) {
        // Is the button now checked?
        boolean checked = ((RadioButton) view).isChecked();
        continueButton.setAlpha(1);
        continueButton.setEnabled(true);
        continueButton.setClickable(true);
        // Check which radio button was clicked
        int i = view.getId();
        if (i == R.id.head01button) {
            if (checked)
            {
                headMac = Constants.macHead01BT;
            }

        } else if (i == R.id.head02button) {
            if (checked)
                headMac = Constants.macHead02BT;

        }
    }



    public void ctnBtn(View view){

        Intent mainActivityIntent = new Intent(headActivity.this, MainActivity.class);


        String headMacStr = headEdit.getText().toString();
        if (BluetoothAdapter.checkBluetoothAddress(headMacStr)){
            headMac = headMacStr;
            Log.i("MACS","got head: "+headMacStr);
        }
        mainActivityIntent.putExtra("head_mac", headMac);

        String remote1MacStr = remoteEdit.getText().toString();
        if (BluetoothAdapter.checkBluetoothAddress(remote1MacStr)){
            remote1Mac = remote1MacStr;

            Log.i("MACS","got remote: "+remote1MacStr);
        }
        mainActivityIntent.putExtra("remote1_mac", remote1Mac);
        mainActivityIntent.putExtra("remote2_mac", remote2Mac);
        Log.i("MACS",  "head: '"+headMac+"' remote1:'"+remote1Mac+"'"+"' remote2:'"+remote2Mac+"'");
        startActivity(mainActivityIntent);
    }

    class MacSpinnerActivity extends Activity implements AdapterView.OnItemSelectedListener {

        SpinnerType type;
        public MacSpinnerActivity(SpinnerType type){
            this.type = type;
        }

        public void onItemSelected(AdapterView<?> parent, View view,
                                   int pos, long id) {
            if(type == SpinnerType.REMOTE) {
                remote1Mac = macArray.get(pos);
                Log.i("MACS", "chosen rmt: " + remote1Mac);
            }else if (type == SpinnerType.RECEIVER){
                headMac = macArray.get(pos);
                Log.i("MACS", "chosen rec: " + headMac);
            }
        }

        public void onNothingSelected(AdapterView<?> parent) {
            // Another interface callback
        }
    }
}
