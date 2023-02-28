#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
////////////////////////////////////////////////////
// lab1 make simple timer k-module with k threads //
////////////////////////////////////////////////////


static struct task_struct * k_thread;

static int thread_fn(void * payload){
    printk(KERN_ALERT "thread_fn running \n");
    return 0;
}


/*
 * skm_lab1_init - init function returns 0 on success
 */

static int skm_lab1_init(void){
	k_thread = kthread_create(thread_fn, NULL, "k_thread");

	if (IS_ERR(k_thread)){	
		printk(KERN_ERR "ERROR: K_thread failed to be created \n");
		return -1;
	}
	
	printk(KERN_ALERT "GOOD NEWS EVERYONE: timer lab1 module loaded \n");
	return 0;
}



/*
 * skm_lab1_exit - exit func returns 0 on success
 */
static void skm_lab1_exit(void){
	kthread_stop(k_thread);
    printk(KERN_ALERT "timer lab1 module offloaded \n");

}




module_init(skm_lab1_init);
module_exit(skm_lab1_exit);

MODULE_AUTHOR("kenthehacker");
MODULE_DESCRIPTION("lab1 timer kmodule");
MODULE_LICENSE("GPL"); 

