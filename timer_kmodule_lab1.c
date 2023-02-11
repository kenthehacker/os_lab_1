#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
/////////////////////////////////////
// lab1 make simple timer k-module //
/////////////////////////////////////

unsigned long log_sec = 1;
unsigned long log_nsec = 0;
module_param(log_sec, ulong, 0644);
module_param(log_nsec,ulong, 0644);


static ktime_t timer_interval;
static struct hrtimer timer;


static enum hrtimer_restart expiration(struct hrtimer *timer){
	printk(KERN_ALERT "exipred \n");
	return hrtimer_forward(timer, ktime_get(), timer_interval);
}


/*
 * skm_lab1_init - init function returns 0 on success
 */

static int skm_lab1_init(void){
	printk(KERN_ALERT "timer lab1 module loaded \n");

	//pass the static unsigned long variables for the module parameters into a call to ktime_set(), 
	//and assign that function's result to the module's static timer interval variable
	timer_interval = ktime_set(log_sec, log_nsec);

	//Call hrtimer_init() with the module's timer variable and the CLOCK_MONOTONIC and HRTIMER_MODE_REL flags;
	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);	

	//Set the function field of the module's timer variable (struct) to point to the timer expiration function
	timer.function = expiration;

	//Call hrtimer_start() with the module's timer and timer interval variables and the HRTIMER_MODE_REL flag, 
	//to schedule the first expiration of the timer (which then will forward the scheduling of 
	//its next expiration etc. until the module is unloaded
	hrtimer_start(&timer, timer_interval, HRTIMER_MODE_REL);


	return 0;
}



/*
 * skm_lab1_exit - exit func returns 0 on success
 */
static void skm_lab1_exit(void){
	printk(KERN_ALERT "timer lab1 module offloaded \n");

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

