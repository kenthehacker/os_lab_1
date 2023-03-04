# os_lab_1

### 1 Names: no.1

Kenichi Matsuo ken.m@wustl.edu

### 2 Module Design: no.2

I merely declared ulongs for log_sec and log_nsec set to 1 and 0 respectively,
which would then be overwritten by the call to module_params. So if I altered the params
then the variables won't have the default values. I also just created
an expiration method that printk'ed the word "expired \n" so we can see
what time the print out happened.
To load my module I just took the same format as the hello world simple
kernel module from the textbook and altered the names of the init and exit
functions



### 3 Timer Design and Evaluation no.3

Implementing this was simple, in order to see what the time stamp in which the timer
was reset, all i had to do was to call on printk() every time the timer needed to be 
resent whenever it expired. By doing so, the time stamp would be displayed right before
the kernel print logs. 

When we call on the skm_lab1_init, or the module's init, we set the timer.function 
field to be the expiration function. This is what enables us to call on printk
whenevr the clock has expired and needs to be reset. 

The following is a snippet of the print out:

Feb 14 16:16:45 kenpi kernel: [10439.695734] exipred 
Feb 14 16:16:45 kenpi kernel: [10439.795736] exipred 
Feb 14 16:16:45 kenpi kernel: [10439.895742] exipred 
Feb 14 16:16:45 kenpi kernel: [10439.995747] exipred 
Feb 14 16:16:45 kenpi kernel: [10440.095784] exipred 
Feb 14 16:16:45 kenpi kernel: [10440.195749] exipred 
Feb 14 16:16:46 kenpi kernel: [10440.295753] exipred 
Feb 14 16:16:46 kenpi kernel: [10440.395754] exipred 
Feb 14 16:16:46 kenpi kernel: [10440.495762] exipred 
Feb 14 16:16:46 kenpi kernel: [10440.595768] exipred 
Feb 14 16:16:46 kenpi kernel: [10440.695770] exipred 
Feb 14 16:16:46 kenpi kernel: [10440.795768] exipred 
Feb 14 16:16:46 kenpi kernel: [10440.895776] exipred 
Feb 14 16:16:46 kenpi kernel: [10440.995780] exipred 

We can see that the word 'expired' is printed every 0.1 seconds


### Thread Design and Evaluation no.4
my thread function:
static int thread_fn(void * payload){
	while(!kthread_should_stop()){
		printk(KERN_ALERT "thread_fn loop nvcsw: %lu nivcsw: %lu \n",current->nvcsw, current->nivcsw);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();
	}
	printk(KERN_ALERT "thread_fn EXITING \n");
    return 0;
}
inside of my init function, all i did was create the kthread
The thread function that was passed in was thread_fn()
which takes in a void * and continues to run until the stopping conditions are met
the thread function actually doesn't run continuously, it runs everytime the 
hrtimer is restarted since inside of the expiration function i call wake_up_process
the schedule() function ensures that we continue in the while loop once 
the wake_up_process() call is processed. 
upon offloading the k-module, we stop the hrtimer and also call on kthread_stop


fragment of system log with default time intervals fed:
[Mar 3 14:53] GOOD NEWS EVERYONE: timer lab1 module loaded 
[  +1.000031] exipred 
[  +0.000088] thread_fn loop nvcsw: 1 nivcsw: 0 
[  +0.999938] exipred 
[  +0.000087] thread_fn loop nvcsw: 2 nivcsw: 0 
[  +0.999938] exipred 
[  +0.000104] thread_fn loop nvcsw: 3 nivcsw: 0 
[  +0.999929] exipred 


log_sec=0 log_nsec=200000000
[Mar 3 15:04] GOOD NEWS EVERYONE: timer lab1 module loaded 
[  +0.200013] exipred 
[  +0.000089] thread_fn loop nvcsw: 1 nivcsw: 0 
[  +0.199915] exipred 
[  +0.000081] thread_fn loop nvcsw: 2 nivcsw: 0 
[  +0.199919] exipred 
[  +0.000074] thread_fn loop nvcsw: 3 nivcsw: 0 
[  +0.199930] exipred 
[  +0.000060] thread_fn loop nvcsw: 4 nivcsw: 0 

log_sec=0 log_nsec=500000000
[  +1.600204] GOOD NEWS EVERYONE: timer lab1 module loaded 
[  +0.500019] exipred 
[  +0.000830] thread_fn loop nvcsw: 1 nivcsw: 1 
[  +0.499179] exipred 
[  +0.000064] thread_fn loop nvcsw: 2 nivcsw: 1 
[  +0.499949] exipred 
[  +0.000084] thread_fn loop nvcsw: 3 nivcsw: 1 

above we see that the print outs are spaced apart quite evenly with an accuracy
of around +- 0.001 units of time. in the shorter time periods we had 
0 involuntary context switches but when i increased the log_nsec param, 
i was able to observe more involuntary context switches. when i inputted short
time spans the time used by the program was too short for an involuntary
context switch to happen, but if we allot a bigger time slice, then
the kernel might preempt this current process to another process w/higher
priority or multiple processes might be running on the same CPU
even if the current task is running the kernel switches to something else
we obviously increment the number of voluntary context switches by 1 every 
time the timer expires since we're forcing this to happen when the timer
gets forwarded and the thread loops again.


### no.5


### no.6

### no.7

### Development Effort no.8
I worked on it by myself



