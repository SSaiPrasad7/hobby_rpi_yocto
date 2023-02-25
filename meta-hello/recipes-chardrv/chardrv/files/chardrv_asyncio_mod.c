#include <linux/kernel.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/fs.h>

#define MAJOR_NUM 42
#define MINOR_NUM 0
#define DEV_NUMS 1
#define DEV_NAME "chardevice"

static int dev_open(struct inode *inod, struct file *file);
static int dev_close(struct inode *inod, struct file *file);
static ssize_t dev_read(struct file *file, char *buf, size_t count, loff_t *offset);
static ssize_t dev_write(struct file *file, const char *buf, size_t count, loff_t *offset);
static int dev_fasync(int fd, struct file *file, int mode);

static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_close,
    .read = dev_read,
    .write = dev_write,
    .fasync = dev_fasync
};

struct fasync_struct *as_q;

// Device context information
typedef struct
{
    char buffer[100];
    int numofbytes;
    struct cdev mycdev; // cdev object
} mydev_t;

// Object of device context info
mydev_t mydev;

// Device number for device file
dev_t dev_num = 0;

static int __init init_mychar(void)
{
    // create device number
    dev_num = MKDEV(MAJOR_NUM, MINOR_NUM);

    // Register the module as character driver
    int ret = register_chrdev_region(dev_num, DEV_NUMS, DEV_NAME);
    if (ret < 0)
    {
        printk(KERN_ALERT "Failed to register the driver as character driver.\n");
        return ret;
    }

    // Initialize the file operations pointer of cdev object with fops variable's base address.
    cdev_init(&mydev.mycdev, &fops);

    // Register the cdev object with OS
    ret = cdev_add(&mydev.mycdev, dev_num, DEV_NUMS);
    if (ret < 0)
    {
        printk(KERN_ALERT "Failed to register to the cdev object.\n");
        goto _exit_module;
    }
    printk(KERN_INFO "Module has been registered and loaded successfully.\n");
    return 0;

_exit_module:
    unregister_chrdev_region(dev_num, DEV_NUMS);
    return ret;
}

static void __exit cleanup_mychar(void)
{
    // Remove the cdev object from OS.
    cdev_del(&mydev.mycdev);

    // Unregister the module as character device.
    unregister_chrdev_region(dev_num, DEV_NUMS);

    printk(KERN_INFO "Module has been unloaded successfully.\n");
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

static ssize_t dev_read(struct file *file, char *buf, size_t count, loff_t *offset)
{
    // Get access to device context info
    mydev_t *temp = file -> private_data;

    // Copy the data from kernel to user
    copy_to_user( buf , temp->buffer , count);

    printk(KERN_INFO "Copied data from driver to user\n");

    //Manipulate the number of bytes copied from user
    temp->numofbytes = 0;

    return (ssize_t)count;
}

static ssize_t dev_write(struct file *file, const char *buf, size_t count, loff_t *offset)
{
    // Get access to device context info
    mydev_t *temp = file -> private_data;

    // Copy the data from userspace to driver buffer
    copy_from_user(temp->buffer , buf , count);
    
    printk(KERN_INFO "Copied data from user to driver\n");

    //Manipulate the number of bytes copied from user
    temp->numofbytes = count;

    if(as_q)
    {
        kill_fasync(&as_q,SIGIO,POLL_IN);
        printk(KERN_INFO"Signal sent to all processes\n");
    }

    return (ssize_t)count;
}

static int dev_fasync(int fd, struct file *file, int mode)
{
    printk(KERN_INFO"Fasync method invoked\n");
    return fasync_helper(fd, file, mode, &as_q);
}

module_init(init_mychar);
module_exit(cleanup_mychar);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Character driver with async I/O driven");
MODULE_AUTHOR("Sai Prasad Samudrala");
MODULE_VERSION("0.4");