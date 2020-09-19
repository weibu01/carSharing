package com.wb.carsharing;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.PixelFormat;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.provider.MediaStore;
import android.support.v4.content.FileProvider;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class InformationCheck extends Activity {

    private static final int TAKE_PHOTO = 11;// 拍照 RequestCode
    public final int TYPE_TAKE_PHOTO = 11;//Uri获取类型判断

    private static final int CROP_PHOTO = 12;// 裁剪图片
    private static final int LOCAL_CROP = 13;// 本地图库

    //两个选择图片的按钮
    private TextView btn_choose_picture1,btn_choose_picture2;
    //三个显示图片的ImageView
    private ImageView iv_show_id1,iv_show_id2,iv_show_drivingLicence1;
    // 拍照时的图片uri
    private Button btn_upload_image,btn_start_useCar;
    private EditText et_name;
    private Uri imageUri;

    private Handler handler;//处理线程发来的信息

    //有两个区域显示图片，确定在哪个区域显示  1-显示身份证区  2-显示驾驶证区
    int In_which_part;

    String account;
    String isPassCheck = "";

    long sendSize=0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_check);

        Intent intent=getIntent();
        //获取传递过来的bundle
        Bundle bundle=intent.getExtras();
        account = bundle.getString("account");

        setViews();// 初始化控件

        setListeners();// 设置监听

        handler = new Handler(new Handler.Callback() {
            @Override
            public boolean handleMessage(Message message) {
                Toast.makeText(InformationCheck.this,message.obj.toString(),Toast.LENGTH_LONG).show();
                if(message.obj.toString().equals("审核通过"))
                {
                    isPassCheck = "审核通过";
                }
                return false;
            }
        });
    }

    /**
     * 控件初始化
     */
    private void setViews() {
        btn_choose_picture1 = (TextView) findViewById(R.id.btn_choose_picture1);
        btn_choose_picture2 = (TextView) findViewById(R.id.btn_choose_picture2);
        iv_show_id1 = (ImageView)findViewById(R.id.iv_show_id1);
        iv_show_id2 = (ImageView)findViewById(R.id.iv_show_id2);
        iv_show_drivingLicence1=(ImageView)findViewById(R.id.iv_show_drivingLicence1);
        btn_upload_image = (Button)findViewById(R.id.btn_upload_image);
        btn_start_useCar = (Button)findViewById(R.id.btn_start_useCar);
        et_name = (EditText)findViewById(R.id.et_name);
    }

    /**
     * 设置监听
     */
    private void setListeners() {
        // 展示图片按钮点击事件
        btn_choose_picture1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                In_which_part=1;//在身份证区显示图片
                takePhotoOrSelectPicture();// 拍照或者调用图库
            }
        });
        // 展示图片按钮点击事件
        btn_choose_picture2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                In_which_part=2;//在驾驶证区显示图片
                takePhotoOrSelectPicture();// 拍照或者调用图库
            }
        });


        // 上传图片
        btn_upload_image.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                uploadImage();
            }
        });


        //开启用车界面
        btn_start_useCar.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(isPassCheck.equals("审核通过"))
                {
                    Intent intent = new Intent(InformationCheck.this,UsingCar.class);
                    startActivity(intent);
                }
                else {
                    Toast.makeText(InformationCheck.this,"抱歉,您还未通过审核,审核通过后才能用车",Toast.LENGTH_SHORT).show();
                }
            }
        });

    }

    /**
     * (1)获取图片 A-拍照  B-图库
     */
    private void takePhotoOrSelectPicture() {
        CharSequence[] items = {"拍照","图库"};// 裁剪items选项

        // 弹出对话框提示用户拍照或者是通过本地图库选择图片
        new AlertDialog.Builder(InformationCheck.this)
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

    /**************************拍照时用到的函数********************************/
    /**
     *(1) 通过intent启动拍照界面拍照----回调函数返回数据
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
     * (1 获取API24以上的保存照片资源的Uri
     * @param type
     * @return
     */
    public Uri get24MediaFileUri(int type) {
        File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES), "共享汽车");
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
     *(2 获取API24以下的保存照片资源的Uri
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
                            //showPictureInWhichView返回的参数类型是ImageView
                        }

                        // 展示图库中选择裁剪后的图片
                        if(data != null){
                            // 根据返回的data，获取Bitmap对象
                            Bitmap bitmap = data.getExtras().getParcelable("data");
                            // 展示图片
                            showPictureInWhichView(In_which_part).setImageBitmap(bitmap);
                            //showPictureInWhichView返回的参数类型是ImageView
                        }

                    }catch (Exception e){
                        e.printStackTrace();
                    }
                }
                break;
        }

    }
    /**
     * (1)检查iv_show_id1,id_show_id2中是否显示图片，返回没显示图片的ImageView，在 公用onActivityResult 中显示图片
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

    public void uploadImage()
    {
        //获取三个图片转为Drawable
        Drawable db1= iv_show_id1.getDrawable();
        Drawable db2= iv_show_id2.getDrawable();
        Drawable db3= iv_show_drivingLicence1.getDrawable();

        if(db1!=null && db2!=null && db3!=null)
        {
            //Drawable 转 bitmap
            Bitmap bitmap1=drawableToBitmap(db1);
            Bitmap bitmap2=drawableToBitmap(db2);
            Bitmap bitmap3=drawableToBitmap(db3);

            final String[] strings=new String[3];
            strings[0] = saveImage("id1",bitmap1);
            strings[1] = saveImage("id2",bitmap2);
            strings[2] = saveImage("drivingLicence",bitmap3);
            Log.d("路径----",strings[0]);

            String[] stringSplit0=strings[0].split("\\|");//图片path|图片大小
            String[] stringSplit1=strings[1].split("\\|");
            String[] stringSplit2=strings[2].split("\\|");

            final String[] imagePath={stringSplit0[0],stringSplit1[0],stringSplit2[0]};
            final String[] imageSize={stringSplit0[1],stringSplit1[1],stringSplit2[1]};

            //Looper.prepare();
            new Thread(){
                @Override
                public void run() {
                    Socket socket=((MySocket)getApplication()).getSocket();
                    try {
                        for(int i=0; i<3; i++)//发送3个文件
                        {
                            if(imagePath[i]!="") {
                                FileInputStream fileInputStream = new FileInputStream(imagePath[i]);
                                OutputStream out1 = socket.getOutputStream();
                                int len = 0;
                                byte[] buf = new byte[1024];
                                // 文件头三部分：name account 图片尺寸    只有一个 '|' 只分解成两部分
                                String fileHead=et_name.getText() + account + '|' + imageSize[i];
                                out1.write(fileHead.getBytes());//发送文件头部信息
                                out1.flush(); //防止黏包

                                try {
                                    Thread.sleep(1000);//延时发送头
                                } catch (InterruptedException e) {
                                    e.printStackTrace();
                                }

                                while ((len = fileInputStream.read(buf, 0, buf.length)) != -1) {
                                    out1.write(buf, 0, len);
                                    sendSize+=len;
                                    Log.e("大小", String.valueOf(len));
                                    out1.flush();
                                }
                                try {
                                    Thread.sleep(1000);//延时发送真正的文件内容
                                } catch (InterruptedException e) {
                                    Log.e("time------",e.toString());
                                }
                                fileInputStream.close();
                            }
                        }

                        //读取两次发来的数据，阻塞式读取，一直阻塞在这里，只有当发数据来读取后才继续执行
                        for(int i=0; i<2; i++) {
                            //接收数据
                            InputStream in1 = socket.getInputStream();
                            byte[] buff = new byte[1024];
                            int len = in1.read(buff);
                            String receiveStr = "";
                            if (len > 0) {
                                receiveStr = new String(buff, 0, len);
                                //Toast.makeText(InformationCheck.this, receiveStr, Toast.LENGTH_LONG).show();
                                Log.e("debug", receiveStr);

                                Message message=Message.obtain();
                                message.obj=receiveStr;
                                handler.sendMessage(message);
                            }
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                        Log.d("发送图片异常", e.toString());
                    }

                }
            }.start();
            //Looper.loop();
        }
    }
    // (1)将Drawable转化为Bitmap
    public Bitmap drawableToBitmap(Drawable drawable) {
        int width = drawable.getIntrinsicWidth();
        int height = drawable.getIntrinsicHeight();
        Bitmap bitmap = Bitmap.createBitmap(width, height, drawable
                .getOpacity() != PixelFormat.OPAQUE ? Bitmap.Config.ARGB_8888
                : Bitmap.Config.RGB_565);
        Canvas canvas = new Canvas(bitmap);
        drawable.setBounds(0, 0, width, height);
        drawable.draw(canvas);
        return bitmap;
    }

    //（2）保存图片到应用目录下的
    public static String saveImage(String fileName,Bitmap bmp) {
        File appDir;
        File file;
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            appDir = new File(Environment.getExternalStorageDirectory(), "carSharingImage");
            if (!appDir.exists()) {
                appDir.mkdir();
            }

            file = new File(appDir, fileName + ".jpg");
            try {
                FileOutputStream fos = new FileOutputStream(file);
                bmp.compress(Bitmap.CompressFormat.JPEG, 100, fos);
                fos.flush();
                fos.close();
            } catch (FileNotFoundException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
            Log.e("文件大小为----", String.valueOf(file.length()));
            return file.getAbsolutePath()+"|"+String.valueOf(file.length());//返回文件路径+文件大小
        }
        return "";
    }

}
