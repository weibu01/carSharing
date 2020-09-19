package com.wb.testdemo;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import java.util.ArrayList;
import java.util.List;


//package com.liwenzhi.example.record;



public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //这里我直接在页面创建的时候请求权限，其实不太好，这里只是为了演示
        //一般是在触发某个事件的时候再请求动态权限，比如点击按钮跳转到一个拍照页面，如果权限通过就跳转，否者吐司说没有权限！
        if (Build.VERSION.SDK_INT >= 23) {//6.0才用动态权限
            initPermission();
        }

    }


