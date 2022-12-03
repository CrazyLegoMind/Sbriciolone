package com.makinarium.makinariumanimatronickeysystem;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioButton;

import com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities.Constants;
import com.makinarium.makinariumanimatronickeysystem.com.makinarium.utilities.MacAddressFactory;

public class headActivity extends AppCompatActivity {

    private Button continueButton;
    private String headMac;
    private String remoteMac = Constants.macEyesBT;
    private EditText remoteEdit;
    private EditText headEdit;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_head);
        continueButton = (Button)findViewById(R.id.continuebutton);
        remoteEdit = findViewById(R.id.editRemoteMac);
        headEdit = findViewById(R.id.editHeadMac);
        continueButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ctnBtn(v);
            }
        });

        continueButton.setAlpha(.5f);
        continueButton.setEnabled(false);
        continueButton.setClickable(false);

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



    private void ctnBtn(View view){

        Intent mainActivityIntent = new Intent(headActivity.this, MainActivity.class);


        String headMacStr = headEdit.getText().toString();
        if (BluetoothAdapter.checkBluetoothAddress(headMacStr)){
            headMac = headMacStr;
            Log.i("MACS","got head: "+headMacStr);
        }
        mainActivityIntent.putExtra("head_mac", headMac);

        String remoteMacStr = remoteEdit.getText().toString();
        if (BluetoothAdapter.checkBluetoothAddress(remoteMacStr)){
            remoteMac = remoteMacStr;

            Log.i("MACS","got remote: "+remoteMacStr);
        }
        mainActivityIntent.putExtra("remote_mac", remoteMac);
        Log.i("MACS",  "head: '"+headMac+"' remote:'"+remoteMac+"'");
        startActivity(mainActivityIntent);
    }
}
