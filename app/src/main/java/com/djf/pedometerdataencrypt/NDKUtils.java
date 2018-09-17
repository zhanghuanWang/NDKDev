package com.djf.pedometerdataencrypt;

/**
 * Created by djf on 2018/9/14.
 */

public class NDKUtils {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("data_aes");
    }

    public static native String stringFromJNI();

    /**
     * 数据包加密
     *
     * @param source
     * @param encrypt
     * @return
     */
    public static native void dataAesEncrypt(byte[] source, byte[] encrypt);

    /**
     * 数据包解密
     *
     * @param source
     * @param decrypt
     * @return
     */
    public static native void dataAesDecrypt(byte[] source, byte[] decrypt);

    public static String bytes2String(byte[] bytes) {
        if (bytes == null || bytes.length <= 0) {
            return "null";
        } else {
            StringBuilder sb=new StringBuilder();
            sb.append("[");
            for (int i = 0; i <bytes.length ; i++) {
                if (i==bytes.length-1){
                    sb.append(byteToHex(bytes[i]));
                }else {
                    sb.append(byteToHex(bytes[i]));
                    sb.append(",");
                }
            }
            sb.append("]");
            return sb.toString();
        }
    }


    /**
     * 字节转十六进制
     * @param b 需要进行转换的byte字节
     * @return  转换后的Hex字符串
     */
    public static String byteToHex(byte b){
        String hex = Integer.toHexString(b & 0xFF);
        if(hex.length() < 2){
            hex = "0" + hex;
        }
        return "0x"+hex;
    }
}
