#include <jni.h>
#include "base64.h"
#include "md5.h"
#include <string>
#include<android/log.h>

using namespace std;

static const char *BEFORE = "info";
static const char *AFTER = "after_";
static const char *correct = "请输入正确的包名";
static int is_oauth = 0;
//暂时写死包名
static const char *PACKAGE_NAME = "com.example.androidnativeencrypt";
//签名写死
static const char *APP_SIGNATURE = "308201dd30820146020101300d06092a864886f70d01010505003037311630"
                                   "1406035504030c0d416e64726f69642044656275673110300e060355040a0c07"
                                   "416e64726f6964310b3009060355040613025553301e170d31393038333030373"
                                   "43830355a170d3439303832323037343830355a30373116301406035504030c0d"
                                   "416e64726f69642044656275673110300e060355040a0c07416e64726f6964310"
                                   "b300906035504061302555330819f300d06092a864886f70d010101050003818d0"
                                   "030818902818100be3718b7686f68fa2781ab54120bc9fe2868f34279cd3915a0a"
                                   "2ad59d1b35700b960de059fbed1748ab86dbd708334e0936e4450b73fb98ae54fe0"
                                   "4c1dc7f23be120927a861d2407f6040eeefe2c51be1ccd1b5a9ffbe4d2816773867"
                                   "13bb20c16d7ddd434fe15874a23104c9b504c046ae866ae1ecb95d23624b28968a"
                                   "f1a230203010001300d06092a864886f70d010105050003818100332dd569fe2bac"
                                   "ddfbefc3373f43c12a586f24a15da2c9e01cb916f6ab8ed9dc174e6b0e898d5c4f2"
                                   "c92ce4c21f9cda0ee0f05cba25da96b872f7d0c08e80a28123063dce21e9534a10a"
                                   "bde30277d6cab682154a57543a02de877d90e785dbc6c5033554e61ca19c242bdb1"
                                   "ac312bd61c2dc22a6febf1a6fdb28623c03813355";

void getMd5String(unsigned char *input, char charArray[32]) {
    MD5_CTX *ctx = new MD5_CTX();
    MD5Init(ctx);
    MD5Update(ctx, input, strlen((const char *) input));
    unsigned char dest[16] = {0};
    MD5Final(dest, ctx);
    for (int i = 0; i < 16; i++) {
        sprintf(charArray, "%s%02x", charArray, dest[i]);
    }
    delete (ctx);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_androidnativeencrypt_NativeEncryptManager_oauthVerify(JNIEnv *env,
                                                                       jobject instance,
                                                                       jobject context) {
    //获取 PackageManager
    jclass j_clz = env->GetObjectClass(context);
    jmethodID j_mid = env->GetMethodID(j_clz, "getPackageManager",
                                       "()Landroid/content/pm/PackageManager;");
    jobject pm = env->CallObjectMethod(context, j_mid);
    // 获取 PackageInfo，getPackageName
    j_mid = env->GetMethodID(j_clz, "getPackageName", "()Ljava/lang/String;");
    jstring j_pack_name = (jstring) env->CallObjectMethod(context, j_mid);
    const char *c_pack_name = env->GetStringUTFChars(j_pack_name, NULL);
    // 先比较包名是否相等，包名不想等返回 返回值是 -1,0,1
    if (strcmp(c_pack_name, PACKAGE_NAME)) {
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "包名不一致:%s", c_pack_name);
        return;
    }
    __android_log_print(ANDROID_LOG_ERROR, "JNI", "包名一致:%s", c_pack_name);
    // 再去比较签名是否一致  getPackageInfo在packageManager的类里面
    //64代表0x00000040  是 public static final int GET_SIGNATURES=0x00000040;
    j_clz = env->GetObjectClass(pm);
    j_mid = env->GetMethodID(j_clz, "getPackageInfo",
                             "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject j_pack_info = env->CallObjectMethod(pm, j_mid, j_pack_name, 64);
    // 获取 Signature[],获取 packageInfo 的 signatures 属性
    j_clz = env->GetObjectClass(j_pack_info);
    jfieldID j_fid = env->GetFieldID(j_clz, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatures_array = (jobjectArray) env->GetObjectField(j_pack_info, j_fid);
    // 获取第0个位置 signatures[0]
    jobject signature_first = env->GetObjectArrayElement(signatures_array, 0);
    // 然后调用 toCharsString 方法
    j_clz = env->GetObjectClass(signature_first);
    j_mid = env->GetMethodID(j_clz, "toCharsString", "()Ljava/lang/String;");
    jstring j_signature = (jstring) env->CallObjectMethod(signature_first, j_mid);
    const char *c_signature = env->GetStringUTFChars(j_signature, NULL);
    //校验签名秘钥
    __android_log_print(ANDROID_LOG_ERROR, "JNI", "APP签名是%s", c_signature);
    if (strcmp(APP_SIGNATURE, c_signature)) {
        // 认证失败
        is_oauth = 0;
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "认证失败");
    } else {
        // 认证成功
        is_oauth = 1;
        __android_log_print(ANDROID_LOG_ERROR, "JNI", "认证成功");
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_androidnativeencrypt_NativeEncryptManager_EncryptMd5(JNIEnv *env, jobject instance,
                                                                      jstring string_) {
    if (is_oauth) {
        const char *string = env->GetStringUTFChars(string_, 0);
        char destString[32] = {0};
        getMd5String((unsigned char *) string, destString);
        env->ReleaseStringUTFChars(string_, string);
        return env->NewStringUTF(destString);
    } else {
        return env->NewStringUTF(correct);
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_androidnativeencrypt_NativeEncryptManager_EncryptBase64(JNIEnv *env,
                                                                         jobject instance,
                                                                         jstring str) {
    if (is_oauth) {
        const char *strSrc = env->GetStringUTFChars(str, 0);
        string in(strSrc);
        char *out = b64_encode((const unsigned char *) in.c_str(), in.length());
        env->ReleaseStringUTFChars(str, strSrc);
        return env->NewStringUTF(out);
    } else {
        return env->NewStringUTF(correct);
    }
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_androidnativeencrypt_NativeEncryptManager_EncryptString(JNIEnv *env,
                                                                         jobject instance,
                                                                         jstring string_) {
    const char *str = env->GetStringUTFChars(string_, 0);

    // TODO 综合加密
    // 字符串首加上 BEFORE
    // 字符串尾巴加上AFTER
    // 然后BASE64

    env->ReleaseStringUTFChars(string_, str);

    return env->NewStringUTF("");
}