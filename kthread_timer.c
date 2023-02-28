#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/kthread.h>
////////////////////////////////////////////////////
// lab1 make simple timer k-module with k threads //
////////////////////////////////////////////////////

unsigned long log_sec = 1;
unsigned long log_nsec = 0;
module_param(log_sec, ulong, 0644);
module_param(log_nsec,ulong, 0644);

struct task_struct * k_thread;

static ktime_t timer_interval;
static struct hrtimer timer;

static int thread_fn(void * payload){
    printk(KERN_ALERT "thread_fn running \n");
    return 0;
}

static enum hrtimer_restart expiration(struct hrtimer *timer){
	printk(KERN_ALERT "exipred \n");
	return hrtimer_forward(timer, ktime_get(), timer_interval);
}


/*
 * skm_lab1_init - init function returns 0 on success
 */

static int skm_lab1_init(void){
	timer_interval = ktime_set(log_sec, log_nsec);
	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);	
	timer.function = expiration;
	hrtimer_start(&timer, timer_interval, HRTIMER_MODE_REL);


	k_thread = kthread_run(thread_fn, NULL, "k_thread");
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
	printk(KERN_ALERT "timer lab1 module offloaded \n");
	kthread_stop(k_thread);
	//use hrtimer_cancel() to cancel the module's timer
	
	if (hrtimer_cancel(&timer) == 0){
		printk(KERN_ALERT "timer was not active \n");
	}else{
		printk(KERN_ALERT "timer was active \n");
	}

}




module_init(skm_lab1_init);
module_exit(skm_lab1_exit);

MODULE_AUTHOR("kenthehacker");
MODULE_DESCRIPTION("lab1 timer kmodule");
MODULE_LICENSE("GPL"); 

