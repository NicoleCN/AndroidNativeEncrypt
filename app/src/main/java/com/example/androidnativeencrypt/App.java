package com.example.androidnativeencrypt;

import android.app.Application;

/***
 * @date 2019/9/4 14:59
 * @author BoXun.Zhao
 * @description
 */
public class App extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        NativeEncryptManager.getInstance().oauthVerify(this);
    }
}
