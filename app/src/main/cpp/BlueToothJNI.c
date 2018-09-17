#include <jni.h>
#include "data_aes.h"
#include "android/log.h"

#define  LOG_TAG    "native-dev"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
JNIEXPORT void JNICALL
Java_com_djf_pedometerdataencrypt_NDKUtils_dataAesEncrypt(JNIEnv *env, jclass type,
                                                          jbyteArray rawBlePacket,
                                                          jbyteArray encryptDataPacket) {
//    LOGI("dataAesEncrypt = %d\n", rawBlePacket);


    jbyte *mRawBlePacket_ = (*env)->GetByteArrayElements(env, rawBlePacket, 0);

    jbyte *mEncryptDataPacket = (*env)->GetByteArrayElements(env, rawBlePacket, 0);

    ble_pack_encrypt((unsigned char *) mRawBlePacket_, (unsigned char *) mEncryptDataPacket);

    (*env)->ReleaseByteArrayElements(env, rawBlePacket, mRawBlePacket_, 0);

    (*env)->ReleaseByteArrayElements(env, encryptDataPacket, mEncryptDataPacket, 0);

}

JNIEXPORT void JNICALL
Java_com_djf_pedometerdataencrypt_NDKUtils_dataAesDecrypt(JNIEnv *env, jclass type,
                                                          jbyteArray receiveBlePacket,
                                                          jbyteArray decryptDataPacket) {
    jbyte *mReceiveBlePacket = (*env)->GetByteArrayElements(env, receiveBlePacket, NULL);

    jbyte *mDecryptDataPacket = (*env)->GetByteArrayElements(env, decryptDataPacket, NULL);

    ble_pack_decrypt((unsigned char *) mReceiveBlePacket, (unsigned char *) mDecryptDataPacket);

    (*env)->ReleaseByteArrayElements(env, receiveBlePacket, mReceiveBlePacket, 0);

    (*env)->ReleaseByteArrayElements(env, decryptDataPacket, mDecryptDataPacket, 0);
}