package com.wb.carsharing;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Application;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.ref.WeakReference;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

import static android.content.pm.PackageManager.PERMISSION_GRANTED;

public class MainActivity extends Activity implements View.OnClickListener {
    /*************** 这部分变量是申请android权限时需要用到的要用到的变量************/
    private static final int NOT_NOTICE = 2;//再次判断，用来判断用户选择“禁止+不再询问”后在打开的界面是否有勾选权限，万一仍然没选呢。。。
    private AlertDialog alertDialog;
    private AlertDialog mDialog;
    private String[] gain_permission = new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE, Manifest.permission.CAMERA};
    //private ArrayList arrayList;
    //创建一个mPermissionList，逐个判断哪些权限未授予，未授予的权限存储到mPerrrmissionList中
    List<String> mPermissionList = new ArrayList<>();

    /**************这部分变量用来判断是否通过审核***************/
    boolean isCheck = false;//用户通过审核设为true

    int sendWhat;

    private Button btn_register;
    private Button btn_logIn;

    public EditText accountEdit;
    public EditText passwordEdit;

    private Handler m_handler;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        btn_register = (Button) findViewById(R.id.btn_register);
        btn_logIn = (Button) findViewById(R.id.btn_logIn);
        btn_register.setOnClickListener(this);
        btn_logIn.setOnClickListener(this);

        accountEdit = (EditText) findViewById(R.id.accountEdit);
        passwordEdit = (EditText) findViewById(R.id.passwordEdit);

        //获取应用会用到的权限
        myRequetPermission();


        //Handler 处理线程发送来的消息
        m_handler = new Handler(new Handler.Callback() {
            @Override
            public boolean handleMessage(Message msg) {
                //super.handleMessage(msg);
                if (msg.obj.toString().equals("登录成功,通过审核") || msg.obj.toString().equals("登录成功,未通过审核"))//发来的消息==“登录成功，说明账号+密码通过了验证”
                {
                    if(msg.obj.toString().equals("登录成功,通过审核"))
                    {
                        isCheck=true;//isCheck默认为false,通过审核后设为true
                    }

                    if (!isCheck) {
                        String account = accountEdit.getText().toString();
                        Intent intent = new Intent(MainActivity.this, InformationCheck.class);
                        //intent对象”意图“   activity通过intent对象表达自己意图
                        Bundle bundle=new Bundle();
                        bundle.putCharSequence("account",account);
                        intent.putExtras(bundle);
                        //启动activity
                        startActivity(intent);
                    } else {  //通过审核直接进入用车界面
                        Intent intent = new Intent(MainActivity.this, UsingCar.class);
                        //intent对象”意图“   activity通过intent对象表达自己意图
                        //启动activity
                        startActivity(intent);
                    }
                }
                return false;
            }
        });
    }

    /**********************用户权限申请用到的函数***********************/
    private void myRequetPermission() {
        mPermissionList.clear();
        //逐个判断你要的权限是否已经通过
        for (int i = 0; i < gain_permission.length; i++) {
            if (ContextCompat.checkSelfPermission(this, gain_permission[i]) != PERMISSION_GRANTED) {
                mPermissionList.add(gain_permission[i]);
            }
        }

        //大于0说明有权限没被允许，权限申请
        if (mPermissionList.size() > 0) {
            //没有权限，申请权限
            String[] noPermission = mPermissionList.toArray(new String[mPermissionList.size()]);//将List转为数组
            ActivityCompat.requestPermissions(this, noPermission, 1);
            //这里会打印要申请的权限提示框
        } else {
            //有,跳过权限申请
            Toast.makeText(this, "您已经申请了所有权限!", Toast.LENGTH_SHORT).show();
        }
    }

    //(1) 处理权限请求响应
    //请求权限后回调的方法
    //参数： requestCode  是我们自己定义的权限请求码
    //参数： permissions  是我们请求的权限名称数组
    //参数： grantResults 是我们在弹出页面后是否允许权限的标识数组，数组的长度对应的是权限名称数组的长度，
    // 数组中元素值为0表示允许权限，-1表示我们点击了禁止权限
    @Override
    public void onRequestPermissionsResult(final int requestCode, String[] permissions, int[] grantResults) {

        super.onRequestPermissionsResult(requestCode, permissions, grantResults);//这步做了啥，可以注释掉？注释了没啥影响

        //requestCode=1 permissions[0]=android.permission.WRITE_EXTERNAL_STORAGE
        if (requestCode == 1) {
            for (int i = 0; i < permissions.length; i++) {
                if (grantResults[i] == PERMISSION_GRANTED) {//选择了“始终允许”  PERMISSION_GRANTED=0
                    Toast.makeText(this, "" + "权限" + permissions[i] + "申请成功", Toast.LENGTH_SHORT).show();
                } else {
                    //用户选择了禁止不再询问
                    if (!ActivityCompat.shouldShowRequestPermissionRationale(this, permissions[i])) {
                        AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                        builder.setTitle("permission")//设置弹出来的对话框的标题
                                .setMessage("点击允许才可以使用我们的app哦")      //对话框显示的内容
                                .setPositiveButton("去允许", new DialogInterface.OnClickListener() {  //对话框上的按钮 及 对话框界面监听器
                                    public void onClick(DialogInterface dialog, int id) {
                                        if (mDialog != null && mDialog.isShowing()) {
                                            mDialog.dismiss();
                                        }

                                        //这里的uri应该是用来保存该包获取了哪些权限，
                                        Intent intent = new Intent(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
                                        Uri uri = Uri.fromParts("package", getPackageName(), null);//注意就是"package",不用改成自己的包名
                                        intent.setData(uri);
                                        //应该会启动界面让用户可以勾选权限  根据上面intent的信息，启动Android活动，选择权限
                                        startActivityForResult(intent, NOT_NOTICE);
                                    }
                                });
                        mDialog = builder.create();
                        mDialog.setCanceledOnTouchOutside(false);
                        mDialog.show();
                        //应该用来显示上面新建的对象builder


                    } else {//选择禁止
                        AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
                        builder.setTitle("permission")
                                .setMessage("点击允许才可以使用我们的app哦")
                                .setPositiveButton("去允许", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        if (alertDialog != null && alertDialog.isShowing()) {
                                            alertDialog.dismiss();
                                        }
                                        //监听 禁止按钮 -> 重新申请  未申请的权限
                                        myRequetPermission();
                                    }
                                });
                        alertDialog = builder.create();
                        alertDialog.setCanceledOnTouchOutside(false);
                        alertDialog.show();
                        //应该用来显示上面新建的对象builder
                    }
                }
            }//for循环结束
        }
    }


    // () 从启动的活动中返回数据  并 判断用户是否勾选了权限
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode==NOT_NOTICE){  //Not_NOTICE=2 自定义的
            myRequetPermission();//由于不知道是否选择了允许所以需要再次判断(虽然打开了后台界面，但可能用户仍然没有选择)
        }
    }

    //()...
    @Override
    public void onClick(View view) {
        switch(view.getId())
        {
            case R.id.btn_register:
                sendWhat=0; //注册按钮 发送注册信息
                break;
            case R.id.btn_logIn:
                sendWhat=1; //登录按钮 发送登录信息
                break;
            default:
                sendWhat=888;//不做处理，随便的一个数
                break;
        }
        new Thread(){
            @Override
            public void run() {
                Looper.prepare();
                sendReceiveData(sendWhat);
                Looper.loop();
            }
        }.start();
    }

    /**
     * (1)处
     */
    public void sendReceiveData(int sendWhat) {
        Socket socket = null;//创建Socket实例，并绑定连接远端IP地址和端口
        try {
            socket = new Socket("24o39366m4.wicp.vip", 33965);
            if (socket.isConnected()) {
                Log.e("线程反馈", "连接服务器成功！");
            }
            DataOutputStream out = new DataOutputStream(socket.getOutputStream());
            DataInputStream in = new DataInputStream(socket.getInputStream());

            ((MySocket)getApplication()).setSocket(socket);
            //发送数据
            switch (sendWhat) {
                //发送注册信息
                case 0:
                    String register_info;
                    //将注册信息拼接起来
                    register_info = "flag_registerInfo";
                    register_info += "|";
                    register_info += accountEdit.getText().toString();
                    register_info += "|";
                    register_info += passwordEdit.getText().toString();

                    out.writeUTF(register_info);
                    break;
                //发送登录信息
                case 1:
                    String logIn_info;
                    //将登录信息拼接起来
                    logIn_info = "flag_logInInfo";
                    logIn_info += "|";
                    logIn_info += accountEdit.getText().toString();
                    logIn_info += "|";
                    logIn_info += passwordEdit.getText().toString();

                    out.writeUTF(logIn_info);
                    break;
            }

            //接收数据
            byte[] buff = new byte[1024];
            int len = in.read(buff);
            String receiveStr="";
            if (len > 0) {
                receiveStr = new String(buff, 0, len);
                Toast.makeText(MainActivity.this, receiveStr, Toast.LENGTH_SHORT).show();
                Log.e("debug", receiveStr);

                if(receiveStr.equals("登录成功,通过审核") || receiveStr.equals("登录成功,未通过审核"))
                {
                    Message message=Message.obtain();
                    message.obj=receiveStr;
                    m_handler.sendMessage(message);
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
            Log.e("线程反馈", "线程异常"+e.toString());
        }

//        finally {
//            if (socket != null) {
//                try {
//                    socket.close();
//                } catch (IOException e) {
//                    e.printStackTrace();
//                }
//            }
//        }
    }

}//MainActivity.class结尾




