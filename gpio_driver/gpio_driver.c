#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/bitops.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/ioctl.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/gpio.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/poll.h>
#include <linux/delay.h>



#define DEVICE_NAME            "gpiodrv"
#define GPIO_MAJOR            200
#define GPIO_IOC_MAGIC            'g'

/* general APIs - GPIO_IOC_MAGIC */
enum {
    IOC_OUTPUT_CLR,
    IOC_OUTPUT_SET,
    IOC_SET_INPUT,
};

#define GPIO_IOC_OUTPUT_LOW        _IOW(GPIO_IOC_MAGIC, IOC_OUTPUT_CLR, unsigned int)
#define GPIO_IOC_OUTPUT_HIG        _IOW(GPIO_IOC_MAGIC, IOC_OUTPUT_SET, unsigned int)
#define GPIO_IOC_INPUT            _IOR(GPIO_IOC_MAGIC, IOC_SET_INPUT, unsigned int)




static struct cdev cdev;
static struct class *gpio_class;
static dev_t devno;

#define DBG_PRINT (printk("simba: %s,%u \t", __FILE__, __LINE__), printk) 

/*OPEN*/
static int gpio_open(struct inode *inode, struct file *filp)
{
    int ret = 0;

    filp->private_data = &cdev;

    return ret;
}

/*RELEASE*/
static int gpio_release(struct inode *inode, struct file *filp)
{
    return 0;
}

/*READ*/
static ssize_t gpio_read(struct file *filp, char __user *buff,
                size_t count, loff_t *offp)
{
    return 0;
}

/*IOCTL*/
static long gpio_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    unsigned int ret = 0,err = 0;

    if (_IOC_TYPE(cmd) != GPIO_IOC_MAGIC)
        return -EINVAL;

    if (arg > 200)
        return -EINVAL;
    
    //申请gpio引脚
    printk("arg = %ld!\n",arg);
    err = gpio_request(arg,NULL);
    if(err)
    {
        printk("err = %x!\n",err);        
    }
    DBG_PRINT("%d\n",cmd);
    switch(cmd) {
    case GPIO_IOC_OUTPUT_LOW:
        DBG_PRINT("\n");
        gpio_direction_output(arg,0);
        break;

    case GPIO_IOC_OUTPUT_HIG:
        DBG_PRINT("\n");
        gpio_direction_output(arg,1);
        break;

    case GPIO_IOC_INPUT:
        DBG_PRINT("\n");
        gpio_direction_input(arg);
        ret = gpio_get_value(arg);
        break;

    default:
        DBG_PRINT("%d\n",cmd);
        ret = -EINVAL;
        break;
    }
    DBG_PRINT("GPIO_OUT_LOW %ld\n",GPIO_IOC_OUTPUT_LOW);
    DBG_PRINT("GPIO_OUT_HIG %ld\n",GPIO_IOC_OUTPUT_HIG);
    DBG_PRINT("GPIO_INPUT %ld\n",GPIO_IOC_INPUT);

    return ret;
}

static struct file_operations gpio_fops = {
    .owner = THIS_MODULE,
    .open = gpio_open,
    .release = gpio_release,
    .read = gpio_read,
    .unlocked_ioctl = gpio_ioctl,

};

/*DEV SETUP*/
static int gpio_setup(struct cdev *cdevp, dev_t dev)
{
    int ret = 0;
    //初始化cdevp的成员，病建立cdevp和gpio_fops的关系。
    cdev_init(cdevp, &gpio_fops);
    cdevp->owner = THIS_MODULE;
    cdevp->ops = &gpio_fops;
    //向系统添加一个cdev设备
    ret = cdev_add(cdevp, dev, 1);
    if (ret)
        printk(KERN_ALERT"add gpio setup failed!\n");

    return ret;
}

void call_func2_with_irq_off(void)
{
    // mdelay(1000);
    volatile unsigned int i = 0,j;
    for (i = 0; i < 0xffff; i++)
    {
        
        j++;
    }
        // DBG_PRINT("\n");
}

void call_func1_with_irq_off(void)
{
    int i = 0;
    // DBG_PRINT("\n");
    for (i = 0; i < 0xff*30; i++)
    {
        // DBG_PRINT("\n");
        call_func2_with_irq_off();
    } 
    asm volatile(""); 
    // DBG_PRINT("\n");
}


/*DEV INIT*/
static int __init gpio_init(void)
{
    struct device *dev;
    int ret;
    unsigned int gpio_major;

    printk("init gpio driver module...\n");
    //1.申请主次设备号
    devno = MKDEV(GPIO_MAJOR, 0);
    gpio_major = MAJOR(devno);
    if (gpio_major)
        ret = register_chrdev_region(devno, 1, DEVICE_NAME);
    else  //如果主设备好已经被使用，就由系统动态申请一个
        ret = alloc_chrdev_region(&devno, 0,1, DEVICE_NAME);

    if (ret < 0) {
        printk(KERN_ALERT"failed in registering dev.\n");
        return ret;
    }
    //2.加入字符设备结构体
    ret = gpio_setup(&cdev, devno);
    if (ret < 0) {
        printk(KERN_ALERT"failed in setup dev.\n");
        return ret;
    }
    //3.在class目录中创建文件
    gpio_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(gpio_class)) {
        printk(KERN_ALERT"failed in creating class.\n");
        return -1;
    }
    //4.生成设备节点
    dev = device_create(gpio_class, NULL, devno, NULL, DEVICE_NAME);
    if (IS_ERR(dev)) {
        printk(KERN_ALERT"failed in creating class.\n");
        return -1;
    }
    local_irq_disable();
    call_func1_with_irq_off();
    local_irq_enable();

    return ret;
}

/*DEV EXIT*/
static void __exit gpio_exit(void)
{
    printk("deinit gpio driver module...\n");
    cdev_del(&cdev);
    unregister_chrdev_region(devno, 1);
    device_destroy(gpio_class, devno);
    class_destroy(gpio_class);
}

module_init(gpio_init);
module_exit(gpio_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Simba");
MODULE_DESCRIPTION("GPIO driver for test");
