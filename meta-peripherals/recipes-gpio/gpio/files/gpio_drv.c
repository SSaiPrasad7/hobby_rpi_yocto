#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kdev_t.h>
#include <linux/delay.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/err.h>

#define GPIO_21 (21)

#define DEV_NUMS 1
#define DRV_NAME "dev_gpio"

static int dev_open(struct inode *inod, struct file *file);
static int dev_close(struct inode *inod, struct file *file);
static ssize_t gpio_read(struct file *file, char *buf, size_t count, loff_t *offset);
static ssize_t gpio_write(struct file *file, const char *buf, size_t count, loff_t *offset);

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_close,
    .read = gpio_read,
    .write = gpio_write
};

// Device context information
typedef struct
{
    char buffer[100];
    int numofbytes;
    struct cdev mycdev; // cdev object
    struct class *dev_class //device class object
} mydev_t;

// Object of device context info
mydev_t mydev;

// Device number for device file
dev_t dev_num = 0;

static int __init init_mychar(void)
{

    // Dynamic Register the module as character driver
    int ret = alloc_chrdev_region(&dev_num,0,DEV_NUMS,DRV_NAME);
    if (ret < 0)
    {
        printk(KERN_ALERT "Cannot allocate the major number.\n");
        goto _unregister_device;
    }
     printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev_num), MINOR(dev_num));


    // Initialize the file operations pointer of cdev object with fops variable's base address.
    cdev_init(&mydev.mycdev, &fops);

    // Register the cdev object with OS
    ret = cdev_add(&mydev.mycdev, dev_num, DEV_NUMS);
    if (ret < 0)
    {
        printk(KERN_ALERT "Failed to register to the system.\n");
        goto _cdev_del;
    }
    // Creating struct class
    mydev.dev_class = class_create(THIS_MODULE,"dev_gpio_class");
    
    if(IS_ERR(mydev.dev_class))
    {
    	printk(KERN_ALERT "Cannot create the class.\n");
        goto _class_create;
    }
    
    // Creating device
    struct device *ret_dev = device_create(mydev.dev_class, NULL, dev_num, NULL, DRV_NAME);
    
    if(IS_ERR(ret_dev))
    {
        printk(KERN_ALERT "Cannot create the device file\n");
        goto _device_create;
    }

    bool gpio_ret = gpio_is_valid(GPIO_21);

    if(gpio_ret == false)
    {
        printk(KERN_ALERT "GPIO %d is not valid\n", GPIO_21);
        goto _device_create;
    }

    //Requesting the GPIO
    if(gpio_request(GPIO_21,"GPIO_21") < 0){
        printk(KERN_ALERT "ERROR: GPIO %d request\n", GPIO_21);
        goto _gpio_free;
    }
  
    //configure the GPIO as output
    gpio_direction_output(GPIO_21, 0);

    gpio_export(GPIO_21, false);
    printk(KERN_INFO "Driver loaded.\n");
    return 0;

_gpio_free:
    gpio_free(GPIO_21);
_device_create:
    device_destroy(mydev.dev_class,dev_num);
_class_create:
    class_destroy(mydev.dev_class);
_cdev_del:
    cdev_del(&mydev.mycdev);
_unregister_device:
    unregister_chrdev_region(dev_num, DEV_NUMS);
    return -1;
}

static void __exit cleanup_mychar(void)
{
    gpio_unexport(GPIO_21);
    gpio_free(GPIO_21);
    device_destroy(mydev.dev_class,dev_num);
    class_destroy(mydev.dev_class);
    // Remove the cdev object from OS.
    cdev_del(&mydev.mycdev);

    // Unregister the module as character device.
    unregister_chrdev_region(dev_num, DEV_NUMS);

    printk(KERN_INFO "Driver unloaded successfully.\n");
}


static int dev_open(struct inode *inod, struct file *file)
{
    //Increase the module reference count
    try_module_get(THIS_MODULE);
    //store the device context info to private_data
    file->private_data = &mydev;

    printk(KERN_INFO "device opened\n");
    
    return 0;
}

static int dev_close(struct inode *inod, struct file *file)
{
    module_put(THIS_MODULE);

    printk(KERN_INFO "device closed\n");

    return 0;
}

static ssize_t gpio_read(struct file *file, char *buf, size_t count, loff_t *offset)
{
    // Get access to device context info
    mydev_t *temp = file -> private_data;

    //reading GPIO value
    temp->buffer[0] = gpio_get_value(GPIO_21);

    // Copy the data from kernel to user
    copy_to_user( buf , temp->buffer, count);

    printk(KERN_INFO "Copied data from driver to user\n");

    //Manipulate the number of bytes copied from user
    temp->numofbytes = 0;

    return (ssize_t)count;
}

static ssize_t gpio_write(struct file *file, const char *buf, size_t count, loff_t *offset)
{
    // Get access to device context info
    mydev_t *temp = file -> private_data;

    // Copy the data from userspace to driver buffer
    copy_from_user(temp->buffer , buf , count);
    
    printk(KERN_INFO "Copied data from user to driver\n");

    if (temp->buffer[0]=='1') {
        //set the GPIO value to HIGH
        gpio_set_value(GPIO_21, 1);
    } 
    else if (temp->buffer[0]=='0') {
        //set the GPIO value to LOW
        gpio_set_value(GPIO_21, 0);
    }
    else {
    pr_err("Unknown command : Please provide either 1 or 0 \n");
   }

    //Manipulate the number of bytes copied from user
    temp->numofbytes = count;

    return (ssize_t)count;
}

module_init(init_mychar);
module_exit(cleanup_mychar);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO driver");
MODULE_AUTHOR("Sai Prasad Samudrala");
MODULE_VERSION("0.1");
