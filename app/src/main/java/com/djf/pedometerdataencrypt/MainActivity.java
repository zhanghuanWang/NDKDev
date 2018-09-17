package com.djf.pedometerdataencrypt;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(NDKUtils.stringFromJNI());
        tv.setOnClickListener(this);
    }

    @Override
    public void onClick(View v) {
        byte[] source=new byte[]{0x00, (byte) 0xfa, (byte) 0xfa,0x00,0x04,0x04,0x02,0x01, (byte) 0xc1,0x10,0x0a,0x0b,0x0c, 0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13};
        byte[] encrypt=new byte[20];
        System.out.println("加密前："+NDKUtils.bytes2String(source));
        NDKUtils.dataAesEncrypt(source,encrypt);
        System.out.println("加密后："+NDKUtils.bytes2String(encrypt));

        source=new byte[]{0x00, (byte) 0xfa, (byte) 0xfa,0x00,0x04,0x04,0x02,0x00, 0x00, (byte) 0xd0,
                0x0a,0x0b,0x0c, 0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13};
        System.out.println("加密前："+NDKUtils.bytes2String(source));
        NDKUtils.dataAesEncrypt(source,encrypt);
        System.out.println("加密后："+NDKUtils.bytes2String(encrypt));

         source=new byte[]{(byte) 0xb1, (byte) 0xd4, (byte) 0xc2,0x1e, (byte) 0x84, (byte) 0xca, (byte) 0x86, (byte) 0x9a, (byte) 0xd4, (byte) 0xfd,
                 (byte) 0xe3, (byte) 0x97, (byte) 0xb3, (byte) 0x95, (byte) 0xcc, (byte) 0xc6,0x11, (byte) 0xa2,0x58,0x2e};
        System.out.println("解密前："+NDKUtils.bytes2String(source));
        NDKUtils.dataAesDecrypt(source,encrypt);
        System.out.println("解密后："+NDKUtils.bytes2String(encrypt));
        NDKUtils.dataAesEncrypt(encrypt,source);
        System.out.println("加密后："+NDKUtils.bytes2String(source));

    }
}
