#include "linux/module.h"
#include "linux/kernel.h"

static int init_hello(void)
{
    printk(KERN_INFO "Hello module loaded Successfully\n");
    return 0;
}

static void cleanup_hello(void)
{
    printk(KERN_INFO "Module unloaded Successfully\n");  
}

module_init(init_hello);
module_exit(cleanup_hello);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello module");
MODULE_AUTHOR("Sai Prasad Samudrala");
MODULE_VERSION("0.1");