package com.wb.tcpclienttest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    Button bt_send_data;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        bt_send_data=(Button) findViewById(R.id.bt_send_data);
        bt_send_data.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        Socket socket=null;
                        try {
                            socket = new Socket("24o39366m4.wicp.vip", 33965);//192.168.59.1
                            if (socket.isConnected()) {
                                Log.d("---------->", "连接成功");
                            }
                            DataOutputStream out = new DataOutputStream(socket.getOutputStream());
                            //OutputStream out = socket.getOutputStream();
                            out.writeUTF("hello\n");
                            out.close();
                            socket.close();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }).start();
            }
        });
    }
}


