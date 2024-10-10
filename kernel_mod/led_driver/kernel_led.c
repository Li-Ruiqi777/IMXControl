#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <asm/mach/map.h>
#include <asm/uaccess.h>
#include <asm/io.h>

struct gpioled_dev
{
    dev_t devid;            // 设备号
    int major;              // 主设备号
    int minor;              // 次设备号
    struct cdev cdev;       // 字符设备
    struct class *class;    // 设备类
    struct device *device;  // 设备实例
    struct device_node *nd; // 设备数中的节点
    int led_gpio;           // led对应gpio的序号
};

struct gpioled_dev gpioled;

#define LEDOFF 0 /* 关灯 */
#define LEDON 1  /* 开灯 */

void led_init(void);
static int led_open(struct inode *inode, struct file *file);
static int led_release(struct inode *inode, struct file *file);
static ssize_t led_read(struct file *file, char __user *buf, size_t count, loff_t *ppos);
static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos);
static int led_probe(struct platform_device *dev);
static int led_remove(struct platform_device *dev);
static int led_module_init(void);
static void led_module_exit(void);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .release = led_release,
    .read = led_read,
    .write = led_write,
};

static int led_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "LED opened\n");
    file->private_data = &gpioled;
    return 0;
}

static int led_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "LED closed\n");
    return 0;
}

static ssize_t led_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    return 0;
}

static ssize_t led_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
    if (buf == NULL)
    {
        return -EINVAL;
    }

    int ret = 0;
    uint8_t databuf[1];
    ret = __copy_from_user(databuf, buf, count);
    struct gpioled_dev *dev = file->private_data;

    if (ret < 0)
    {
        printk(KERN_INFO "write LED status failed! \n");
        return EFAULT;
    }

    if (databuf[0] == LEDON)
    {
        gpio_set_value(dev->led_gpio, 0);
    }
    else
    {
        gpio_set_value(dev->led_gpio, 1);
    }
}

// 初始化LED
void led_init(void)
{
    u32 val = 0;
    int ret;
    u32 regdata[14];
    const char *str;
    struct property *proper;

    /* 获取设备树中的属性数据 */
    /* 1、获取设备节点 */
    gpioled.nd = of_find_node_by_path("/gpioled");
    if (gpioled.nd == NULL)
    {
        printk("gpioled node nost find!\r\n");
        return -EINVAL;
    }
    else
    {
        printk("gpioled node find!\r\n");
    }

    /* 2、获取设备树的gpio子系统节点的属性,得到led的所使用的gpio编号 */
    gpioled.led_gpio = of_get_named_gpio(gpioled.nd, "led-gpio", 0);
    if (gpioled.led_gpio < 0)
    {
        printk("can't get led-gpio! \r\n");
        return -EINVAL;
    }
    printk("led-gpio num = %d\r\n", gpioled.led_gpio);

    /* 5.设置该GPIO为输出,默认高电平 */
    gpio_request(gpioled.led_gpio, "led0");
    gpio_direction_output(gpioled.led_gpio, 1);
}

int led_probe(struct platform_device *dev)
{
    int result;
    //* 动态分配设备号
    if (gpioled.major)
    {
        gpioled.devid = MKDEV(gpioled.major, 0);
        register_chrdev_region(gpioled.devid, 1, "gpio_led");
    }
    else
    {                                                          /* 没有定义设备号 */
        alloc_chrdev_region(&gpioled.devid, 0, 1, "gpio_led"); /* 申请设备号 */
        gpioled.major = MAJOR(gpioled.devid);                  /* 获取分配号的主设备号 */
        gpioled.minor = MINOR(gpioled.devid);                  /* 获取分配号的次设备号 */
    }
    printk("gpioled major=%d,minor=%d\r\n", gpioled.major, gpioled.minor);

    //* 注册此类字符设备
    gpioled.cdev.owner = THIS_MODULE;
    cdev_init(&gpioled.cdev, &fops);
    result = cdev_add(&gpioled.cdev, gpioled.devid, 1);
    if (result)
    {
        printk(KERN_NOTICE "Error %d adding gpio_led", result);
        unregister_chrdev_region(gpioled.devid, 1);
        return result;
    }

    //* 创建类
    gpioled.class = class_create(THIS_MODULE, "gpio_led");
    if (IS_ERR(gpioled.class))
    {
        return PTR_ERR(gpioled.class);
    }

    //* 创建字符设备实例
    gpioled.device = device_create(gpioled.class, NULL, gpioled.devid, NULL, "gpio_led");
    if (IS_ERR(gpioled.device))
    {
        return PTR_ERR(gpioled.device);
    }

    printk(KERN_INFO "gpio_led module loaded \n");

    led_init();
    return 0;
}

static int led_remove(struct platform_device *dev)
{
    gpio_direction_output(gpioled.led_gpio, 1);

    device_destroy(gpioled.class, gpioled.devid);
    class_destroy(gpioled.class);
    cdev_del(&gpioled.cdev);
    unregister_chrdev_region(gpioled.devid, 1);
    printk(KERN_INFO "gpio_led module unloaded \n");
}

static const struct of_device_id led_of_match[] = {
    {.compatible = "gpio-led"},
    {}};

static struct platform_driver led_driver = {
    .driver = {
        .name = "imx6ul-led",
        .of_match_table = led_of_match,
    },
    .probe = led_probe,
    .remove = led_remove,
};

static int led_module_init(void)
{
    return platform_driver_register(&led_driver);
}

static void led_module_exit(void)
{
    platform_driver_unregister(&led_driver);
}

module_init(led_module_init);
module_exit(led_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Simple Character Device Driver");
MODULE_VERSION("0.1");
