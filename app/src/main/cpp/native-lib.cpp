#include <jni.h>
#include "base64.h"
#include "md5.h"
#include <string>

using namespace std;


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

    // TODO app签名校验

}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_androidnativeencrypt_NativeEncryptManager_EncryptMd5(JNIEnv *env, jobject instance,
                                                                      jstring string_) {
    const char *string = env->GetStringUTFChars(string_, 0);
    char destString[32] = {0};
    getMd5String((unsigned char *) string, destString);
    env->ReleaseStringUTFChars(string_, string);
    return env->NewStringUTF(destString);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_androidnativeencrypt_NativeEncryptManager_EncryptBase64(JNIEnv *env,
                                                                         jobject instance,
                                                                         jstring str) {
    const char *strSrc = env->GetStringUTFChars(str, 0);
    string in(strSrc);
    char *out = b64_encode((const unsigned char *) in.c_str(), in.length());
    env->ReleaseStringUTFChars(str, strSrc);
    return env->NewStringUTF(out);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_androidnativeencrypt_NativeEncryptManager_EncryptString(JNIEnv *env,
                                                                         jobject instance,
                                                                         jstring string_) {
    const char *str = env->GetStringUTFChars(string_, 0);

    // TODO 综合加密

    env->ReleaseStringUTFChars(string_, str);

    return env->NewStringUTF("");
}