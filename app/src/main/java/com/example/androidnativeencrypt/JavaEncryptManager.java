package com.example.androidnativeencrypt;

import android.util.Base64;

import java.io.UnsupportedEncodingException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

/***
 * @date 2019/9/4 14:15
 * @author BoXun.Zhao
 * @description java实现加密
 */
public class JavaEncryptManager {
    private JavaEncryptManager() {

    }

    public static JavaEncryptManager getInstance() {
        return JavaEncryptManagerHolder.javaEncryptManager;
    }

    public String EncryptMd5(String str) {
        byte[] hash;
        try {
            hash = MessageDigest.getInstance("MD5").digest(str.getBytes("UTF-8"));
        } catch (NoSuchAlgorithmException e) {
            throw new RuntimeException("NoSuchAlgorithmException", e);
        } catch (UnsupportedEncodingException e) {
            throw new RuntimeException("UnsupportedEncodingException", e);
        }
        //对生成的16字节数组进行补零操作
        StringBuilder hex = new StringBuilder(hash.length * 2);
        for (byte b : hash) {
            if ((b & 0xFF) < 0x10) {
                hex.append("0");
            }
            hex.append(Integer.toHexString(b & 0xFF));
        }
        return hex.toString();
    }

    public String EncryptBase64(String string) {
        String out = null;
        try {
            out = new String(Base64.encode(string.getBytes("utf-8"), Base64.NO_WRAP), "utf-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        return out;
    }

    private static class JavaEncryptManagerHolder {
        private static JavaEncryptManager javaEncryptManager = new JavaEncryptManager();
    }
}
