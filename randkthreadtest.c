#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>

MODULE_LICENSE("GPL");

static struct task_struct *my_thread;
static int my_thread_func(void *data)
{
    printk(KERN_INFO "YO Kernel thread started\n");

    // Do whatever work needs to be done in the kernel thread

    return 0;
}

static int __init my_init(void)
{
    my_thread = kthread_run(my_thread_func, NULL, "my_thread");

    if (IS_ERR(my_thread)) {
        printk(KERN_ERR "Failed to create kernel thread\n");
        return PTR_ERR(my_thread);
    }

    printk(KERN_INFO "Module loaded successfully\n");
    return 0;
}

static void __exit my_exit(void)
{
    kthread_stop(my_thread);

    printk(KERN_INFO "Module unloaded\n");
}

module_init(my_init);
module_exit(my_exit);
