package com.wb.tcpclient;

import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity implements OnClickListener{
    private Button button_left;
    private Button button_right;
    private Button button_up;
    private Button button_down;
    private Button button_stop;
    private Button button_start;
    private Button button_status;
    private String command;//按钮发送的命令

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);//指定了当前活动的布局，这里表示将从res/layout目录中找到activity_main.xml文件作为本例的布局文件使用。
        button_left=(Button)findViewById(R.id.button_left);
        button_right=(Button)findViewById(R.id.button_right);
        button_up=(Button)findViewById(R.id.button_up);
        button_down=(Button)findViewById(R.id.button_down);
        button_start=(Button)findViewById(R.id.button_start);
        button_stop=(Button)findViewById(R.id.button_stop);
        button_status=(Button)findViewById(R.id.button_status);

        button_left.setOnClickListener(this); //监听按键
        button_right.setOnClickListener(this); //监听按键
        button_up.setOnClickListener(this); //监听按键
        button_down.setOnClickListener(this); //监听按键
        button_start.setOnClickListener(this); //监听按键
        button_stop.setOnClickListener(this); //监听按键
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public void onClick(View arg0) {
        switch (arg0.getId()){
            case R.id.button_left:
                command = "L";
                break;
            case R.id.button_right:
                command = "R";
                break;
            case R.id.button_up:
                command = "U";
                break;
            case R.id.button_down:
                command = "D";
                break;
            case R.id.button_start:
                command = "B";
                break;
            case R.id.button_stop:
                command = "E";
                break;
            default:
                command = " ";//在按了其他按键的情况下命令置为空格
                break;
        }
        new Thread(){
            @Override
            public void run(){
                sendData();
            }
        }.start();
    }

    private void sendData(){

        Socket socket = null;//创建Socket实例，并绑定连接远端IP地址和端口
        try {
            socket = new Socket("24o39366m4.wicp.vip",32656);
            Log.e("线程反馈","创建成功！");
            DataOutputStream out = new DataOutputStream(socket.getOutputStream());
            out.writeUTF(command);
            socket.close();
            button_status.setText(command+"/n");

			/*OutputStream ops = socket.getOutputStream();//定义一个输出流，来自于Socket输出流
			String b="a\n";
			byte[] bytes = b.getBytes();
			ops.write(bytes);//向输出流中写入数据
			Log.v("线程反馈","发送成功！");
			ops.flush();//刷行输出流
			 */
        } catch (IOException e) {
            e.printStackTrace();
            Log.e("线程反馈","线程异常！");
        }
    }
}