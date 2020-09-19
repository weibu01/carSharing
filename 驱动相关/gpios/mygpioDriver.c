#include <linux/init.h>
#include <linux/module.h>

/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>
/*注册杂项设备头文件*/
#include <linux/miscdevice.h>
/*注册设备节点的文件结构体*/
#include <linux/fs.h>

/*Linux中申请GPIO的头文件*/
#include <linux/gpio.h>
/*三星平台的GPIO配置函数头文件*/
/*三星平台EXYNOS系列平台，GPIO配置参数宏定义头文件*/
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
/*三星平台4412平台，GPIO宏定义头文件*/
#include <mach/gpio-exynos4.h>

#define DRIVER_NAME "mygpio"   //驱动注册名
#define DEVICE_NAME "mygpio"  //设备节点名  设备节点名和驱动注册名、设备注册名可以不同，这里相同，提高可读性

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("TOPEET");

static int led_gpios[] = {   //2个gpio    
	EXYNOS4_GPX2(7),EXYNOS4_GPL0(1)   //gpio0 - UART:14  gpio1 - CAMERA:3
};

#define LED_NUM		ARRAY_SIZE(led_gpios)


static long hello_ioctl( struct file *files, unsigned int cmd, unsigned int arg){
	printk("cmd is %d,arg is %d\n",cmd,arg);
	
	switch(cmd)
	{
		case 0:
		case 1:
			if (arg > LED_NUM) {
				return -EINVAL;    //只有命令0、1有效，其他的，EINVAL表示无效的参数？？
			}

			gpio_set_value(led_gpios[arg], cmd);  //led_gpios[arg] 给数组里的哪个gpio发命令
			break;

		default:
			return -EINVAL;
	}
	return 0;
}

static int hello_release(struct inode *inode, struct file *file){
	printk(KERN_EMERG "hello release\n");
	return 0;
}

static int hello_open(struct inode *inode, struct file *file){
	printk(KERN_EMERG "hello open\n");
	return 0;
}

static struct file_operations hello_ops = {
	.owner = THIS_MODULE,
	.open = hello_open,
	.release = hello_release,
	.unlocked_ioctl = hello_ioctl,
};

static  struct miscdevice hello_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &hello_ops,
};


static int hello_probe(struct platform_device *pdv){
	int ret,i;
	
	printk(KERN_EMERG "\t enter probe\n");
	printk(KERN_EMERG "\tinitialized\n");
	
	for(i=0; i<LED_NUM; i++) //for循环，申请使用32个gpio
	{
		ret = gpio_request(led_gpios[i], "LED");
		if (ret) {
			printk("%s: request GPIO %d for LED failed, ret = %d\n", DRIVER_NAME,
					i, ret);
			}
		else{
			s3c_gpio_cfgpin(led_gpios[i], S3C_GPIO_OUTPUT);
			gpio_set_value(led_gpios[i], 0);  //初始化后都设为低电平了		
		}
	}
	
	misc_register(&hello_dev);  
	if(ret<0)
	{
		printk("leds:register device failed!\n");
		goto exit;
	}
	return 0;

exit:                              
	misc_deregister(&hello_dev);
	return ret;
	//return 0;
}

static int hello_remove(struct platform_device *pdv){
	int i;
	
	printk(KERN_EMERG "\tremove\n");
	
	for(i=0; i<LED_NUM; i++)
	{
		gpio_free(led_gpios[i]);  //卸载驱动时，注意释放gpio
	}
	
	misc_deregister(&hello_dev);
	return 0;
}

static void hello_shutdown(struct platform_device *pdv){
	
	;
}

static int hello_suspend(struct platform_device *pdv,pm_message_t pmt){
	
	return 0;
}

static int hello_resume(struct platform_device *pdv){
	
	return 0;
}

struct platform_driver hello_driver = {
	.probe = hello_probe,
	.remove = hello_remove,
	.shutdown = hello_shutdown,
	.suspend = hello_suspend,
	.resume = hello_resume,
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	}
};


static int hello_init(void)
{
	int DriverState;
	
	printk(KERN_EMERG "HELLO WORLD enter!\n");
	DriverState = platform_driver_register(&hello_driver);
	
	printk(KERN_EMERG "\tDriverState is %d\n",DriverState);
	return 0;
}


static void hello_exit(void)
{
	void free_irq(unsigned int irq,void *dev_id);
	
	printk(KERN_EMERG "HELLO WORLD exit!\n");	
	
	platform_driver_unregister(&hello_driver);	
}

module_init(hello_init);
module_exit(hello_exit);
