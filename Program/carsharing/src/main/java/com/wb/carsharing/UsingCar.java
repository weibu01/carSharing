package com.wb.carsharing;

import android.app.Activity;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;
import java.util.Locale;

public class UsingCar extends Activity {
    public Button bt_use_car;
    public Button bt_return_car;
    //public Button bt_lock_car;
    public EditText et_plate_number;
    public Socket socket;
    public boolean isDefine = false;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_using);

        init();

        setListeners();

    }

    public void init()
    {
        bt_use_car = (Button)findViewById(R.id.bt_use_car);
        //bt_lock_car = (Button)findViewById(R.id.bt_lock_car);
        bt_return_car = (Button)findViewById(R.id.bt_return_car);
        et_plate_number = (EditText)findViewById(R.id.et_plate_number);
    }
    public void setListeners()
    {
        bt_use_car.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {      //flag+怎么处理车+车牌号
                String car_state ="flag_usingCarInfo"+"|"+"useCar"+"|"+et_plate_number.getText().toString();
                sendCarState(car_state);
            }
        });
        bt_return_car.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String car_state ="flag_usingCarInfo"+"|"+"return"+"|"+et_plate_number.getText().toString();
                sendCarState(car_state);
            }
        });
    }

    public void sendCarState(final String car_state)
    {
        new Thread()
        {
            @Override
            public void run() {
                //super.run();
                if(!isDefine) {
                    socket = ((MySocket) getApplication()).getSocket();//获取之前连接到服务器的socket
                     //out = null;
                }
                try {
                    DataOutputStream out = new DataOutputStream(socket.getOutputStream());
                    out.writeUTF(car_state);
//                    out.flush();
//                    try {
//                        Thread.sleep(1000);
//                    } catch (InterruptedException e) {
//                        e.printStackTrace();
//                    }
                    //out = null;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }.start();
    }
}
