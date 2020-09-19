package com.wb.gainpicture;

import android.Manifest;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v4.content.FileProvider;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import static android.content.pm.PackageManager.PERMISSION_GRANTED;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class MainActivity extends Activity {

    private static final int TAKE_PHOTO = 11;// 拍照 RequestCode
    public final int TYPE_TAKE_PHOTO = 11;//Uri获取类型判断

    private static final int CROP_PHOTO = 12;// 裁剪图片
    private static final int LOCAL_CROP = 13;// 本地图库

    //两个选择图片的按钮
    private TextView btn_choose_picture1,btn_choose_picture2;
    //三个显示图片的ImageView
    private ImageView iv_show_id1,iv_show_id2,iv_show_drivingLicence1;
    // 拍照时的图片uri
    private Uri imageUri;

    //再次判断，用来判断用户选择“禁止+不再询问”后在打开的界面是否有勾选权限，万一仍然没选呢。。。
    private static final int NOT_NOTICE = 2;
    private AlertDialog alertDialog;
    private AlertDialog mDialog;
    private String[] gain_permission=new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.CAMERA,
            Manifest.permission.INTERNET,Manifest.permission.CALL_PHONE};

    //创建一个mPermissionList，逐个判断哪些权限未授予，未授予的权限存储到mPerrrmissionList中
    List<String> mPermissionList = new ArrayList<>();

    //有两个区域显示图片，确定在哪个区域显示  1-显示身份证区  2-显示驾驶证区
    int In_which_part;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        myRequetPermission();//获取应用会用到的权限

        setViews();// 初始化控件

        setListeners();// 设置监听
    }

    /**
     * 公用
     * 控件初始化
     */
    private void setViews() {
        btn_choose_picture1 = (TextView) findViewById(R.id.btn_choose_picture1);
        btn_choose_picture2 = (TextView) findViewById(R.id.btn_choose_picture2);
        iv_show_id1 = (ImageView)findViewById(R.id.iv_show_id1);
        iv_show_id2 = (ImageView)findViewById(R.id.iv_show_id2);
        iv_show_drivingLicence1=(ImageView)findViewById(R.id.iv_show_drivingLicence1);
    }

    /**
     * 1 设置监听
     */
    private void setListeners() {

        // 展示图片按钮点击事件
        btn_choose_picture1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                takePhotoOrSelectPicture();// 拍照或者调用图库
                In_which_part=1;//在身份证区显示图片

            }
        });

        // 展示图片按钮点击事件
        btn_choose_picture2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {

                takePhotoOrSelectPicture();// 拍照或者调用图库
                In_which_part=2;//在驾驶证区显示图片

            }
        });


    }

    /**
     *1.1 获取图片 A-拍照  B-图库
     */
    private void takePhotoOrSelectPicture() {
        CharSequence[] items = {"拍照","图库"};// 裁剪items选项

        // 弹出对话框提示用户拍照或者是通过本地图库选择图片
        new AlertDialog.Builder(MainActivity.this)
                .setItems(items, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                        switch (which){
                            // 选择了拍照
                            case 0:
                                startTakePhoto();
                                break;
                            // 调用系统图库
                            case 1:
                                // 创建Intent，用于打开手机本地图库选择图片
                                Intent intent1 = new Intent(Intent.ACTION_PICK,
                                        android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
                                // 启动intent打开本地图库
                                startActivityForResult(intent1,LOCAL_CROP);
                                break;
                        }
                    }
                }).show();
    }


    /**
     * 公用onActivityResult
     * 调用startActivityForResult方法启动一个intent后，
     * 可以在该方法中拿到返回的数据
     */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        switch (requestCode){

            case TAKE_PHOTO:// 拍照

                if(resultCode == RESULT_OK){
                    // 创建intent用于裁剪图片
                    Intent intent = new Intent("com.android.camera.action.CROP");
                    // 获取图库所选图片的uri
                    Uri uri = data.getData();
                    intent.setDataAndType(uri,"image/*");

                    // 允许裁剪
                    intent.putExtra("scale",true);

                    //  设置裁剪图片的宽高
                    intent.putExtra("outputX", 300);
                    intent.putExtra("outputY", 300);
                    // 裁剪后返回数据
                    intent.putExtra("return-data", true);
                    // 启动intent，开始裁剪

                    startActivityForResult(intent, CROP_PHOTO);
                }
                break;
            case LOCAL_CROP:// 系统图库

                if(resultCode == RESULT_OK){
                    // 创建intent用于裁剪图片
                    Intent intent1 = new Intent("com.android.camera.action.CROP");
                    // 获取图库所选图片的uri
                    Uri uri = data.getData();
                    intent1.setDataAndType(uri,"image/*");
                    //  设置裁剪图片的宽高
                    intent1.putExtra("outputX", 300);
                    intent1.putExtra("outputY", 300);
                    // 裁剪后返回数据
                    intent1.putExtra("return-data", true);
                    // 启动intent，开始裁剪
                    startActivityForResult(intent1, CROP_PHOTO);
                }

                break;
            case CROP_PHOTO:// 展示裁剪后图片
                if(resultCode == RESULT_OK){
                    try{
                        // 展示拍照后裁剪的图片
                        if(imageUri != null){
                            // 创建BitmapFactory.Options对象
                            BitmapFactory.Options option = new BitmapFactory.Options();
                            // 属性设置，用于压缩bitmap对象
                            option.inSampleSize = 2;
                            option.inPreferredConfig = Bitmap.Config.RGB_565;
                            // 根据文件流解析生成Bitmap对象
                            Bitmap bitmap = BitmapFactory.decodeStream(getContentResolver().openInputStream(imageUri));//, null, option);
                                 //bitmap = BitmapFactory.decodeStream(getContentResolver().openInputStream(photoUri));
                            // 展示图片
                            showPictureInWhichView(In_which_part).setImageBitmap(bitmap);
                        }

                        // 展示图库中选择裁剪后的图片
                        if(data != null){
                            // 根据返回的data，获取Bitmap对象
                            Bitmap bitmap = data.getExtras().getParcelable("data");
                            // 展示图片
                            showPictureInWhichView(In_which_part).setImageBitmap(bitmap);

                        }

                    }catch (Exception e){
                        e.printStackTrace();
                    }
                }
                break;






            case NOT_NOTICE:
                if(resultCode==RESULT_OK){
                    if(requestCode==NOT_NOTICE){  //Not_NOTICE=2 自定义的
                        myRequetPermission();//由于不知道是否选择了允许所以需要再次判断(虽然打开了后台界面，但可能用户仍然没有选择)
                    }
                }
                break;
        }

    }

    /**
     * 公用onActivityResult.1
     * 检查iv_show_id1,id_show_id2中是否显示图片，返回没显示图片的ImageView，在 公用onActivityResult 中显示图片
     * which_part 有两个区域显示图片，确定在哪个区域显示
     */
    public ImageView showPictureInWhichView(int which_part)
    {
        switch (which_part) {
            //显示身份证区
            case 1:
                //Bitmap bitmap = ((BitmapDrawable)imageView.getDrawable()).getBitmap();
                if ((BitmapDrawable) iv_show_id1.getDrawable() == null) {
                    return iv_show_id1;
                } else {
                    if ((BitmapDrawable) iv_show_id2.getDrawable() == null)
                        return iv_show_id2;
                }
            break;
            //显示驾驶证区
            case 2:
                if((BitmapDrawable)iv_show_drivingLicence1.getDrawable()==null)
                {
                    return iv_show_drivingLicence1;
                }
                break;
        }
        return null;
    }



/**************************拍照时用到的函数********************************/

    /**
     *1.2 通过intent启动拍照界面拍照----回调函数返回数据
     */
    public void startTakePhoto()
    {
        if (Build.VERSION.SDK_INT >= 24) {
            Intent takeIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
            Uri photoUri = get24MediaFileUri(TYPE_TAKE_PHOTO);
            takeIntent.putExtra(MediaStore.EXTRA_OUTPUT, photoUri);
            startActivityForResult(takeIntent, CROP_PHOTO);
        } else {
            Intent takeIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
            Uri photoUri = getMediaFileUri();
            takeIntent.putExtra(MediaStore.EXTRA_OUTPUT, photoUri);
            startActivityForResult(takeIntent, CROP_PHOTO);
        }

    }

    /**
     * 1.2.1获取API24以上的保存照片资源的Uri
     * @param type
     * @return
     */
    public Uri get24MediaFileUri(int type) {
        File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES), "相册名字");
        if (!mediaStorageDir.exists()) {
            if (!mediaStorageDir.mkdirs()) {
                return null;
            }
        }
        //创建Media File
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        File mediaFile;
        if (type == TYPE_TAKE_PHOTO) {
            mediaFile = new File(mediaStorageDir.getPath() + File.separator + "IMG_" + timeStamp + ".jpg");
        } else {
            return null;
        }

        imageUri = FileProvider.getUriForFile(this, getPackageName()+".fileprovider", mediaFile);
        return imageUri;
    }

    /**
     * 1.2.2
     * 获取API24以下的保存照片资源的Uri
     */
    public Uri getMediaFileUri() {
        File takePhotoImage = new File(Environment.getExternalStorageDirectory(), "take_photo_image.jpg");
        try {
            // 文件存在，删除文件
            if(takePhotoImage.exists()){
                takePhotoImage.delete();
            }
            // 根据路径名自动的创建一个新的空文件
            takePhotoImage.createNewFile();
        }catch (Exception e){
            e.printStackTrace();
        }
        // 获取图片文件的uri对象
        imageUri = Uri.fromFile(takePhotoImage);
        return imageUri;
    }


/*********************************权限获取***************************************/
    /**
     * 2 获取权限
     */
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

    //2.1
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

}




