package com.example.androidnativeencrypt;

import android.content.Context;

/***
 * @date 2019/9/4 13:42
 * @author BoXun.Zhao
 * @description C++实现加密
 */
public class NativeEncryptManager {
    static {
        System.loadLibrary("native-lib");
    }

    private NativeEncryptManager() {

    }

    public native void oauthVerify(Context context);

    public native String EncryptMd5(String string);

    public native String EncryptBase64(String string);

    public native String EncryptString(String string);

    public static NativeEncryptManager getInstance(){
        return NativeEncryptManagerHolder.nativeEncryptManager;
    }

    private static class NativeEncryptManagerHolder {
        private static NativeEncryptManager nativeEncryptManager = new NativeEncryptManager();
    }
}
