package com.example.androidnativeencrypt;

import android.os.Bundle;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    private TextView jb64Tv;
    private TextView jm5Tv;
    private TextView nb64Tv;
    private TextView nm5Tv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        initView();
        String base64 = "nicole";
        String md5 = "zhaoboxun";
        jb64Tv.setText(jb64Tv.getText().toString()+JavaEncryptManager.getInstance().EncryptBase64(base64));
        nb64Tv.setText(nb64Tv.getText().toString()+NativeEncryptManager.getInstance().EncryptBase64(base64));
        jm5Tv.setText(jm5Tv.getText().toString()+JavaEncryptManager.getInstance().EncryptMd5(md5));
        nm5Tv.setText(nm5Tv.getText().toString()+NativeEncryptManager.getInstance().EncryptMd5(md5));
    }

    private void initView() {
        jb64Tv = findViewById(R.id.java_base64_text);
        jm5Tv = findViewById(R.id.java_md5_text);
        nb64Tv = findViewById(R.id.native_base64_text);
        nm5Tv = findViewById(R.id.native_md5_text);
    }

}
