package com.wb.mypermission;

import android.Manifest;
import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AlertDialog;
import android.os.Bundle;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.List;

import static android.content.pm.PackageManager.PERMISSION_GRANTED;

public class MainActivity extends Activity {

    private static final int NOT_NOTICE = 2;//再次判断，用来判断用户选择“禁止+不再询问”后在打开的界面是否有勾选权限，万一仍然没选呢。。。
    private AlertDialog alertDialog;
    private AlertDialog mDialog;
    private int i;//后面的循环判断，剔除gain_permission[]中不是从mPermissionList中拷贝的字符串
    private String[] gain_permission=new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.CAMERA,
            Manifest.permission.INTERNET,Manifest.permission.CALL_PHONE,Manifest.permission.ACCESS_WIFI_STATE};
    //private String[] un_gain_permission;//初次获取权限时没获取到的权限
    //private ArrayList arrayList;
    //创建一个mPermissionList，逐个判断哪些权限未授予，未授予的权限存储到mPerrrmissionList中
    List<String> mPermissionList = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        myRequetPermission();//获取应用会用到的权限
    }

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

    //处理权限请求响应
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


    //从启动的活动中返回数据  并 判断用户是否勾选了权限
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(requestCode==NOT_NOTICE){  //Not_NOTICE=2 自定义的
            myRequetPermission();//由于不知道是否选择了允许所以需要再次判断(虽然打开了后台界面，但可能用户仍然没有选择)
        }
    }
}
