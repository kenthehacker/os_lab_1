# os_lab_1

### 1 Names: no.1

Kenichi Matsuo 

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


### Multi-threading Design and Evaluation no.5

Default params:
[61505.947910] multi_kthread: loading out-of-tree module taints kernel.
[61505.949302] GOOD NEWS EVERYONE: timer lab1 module loaded 
[61506.949335] exipred 
[61506.949402] cpu:1 nvcsw: 1 nivcsw: 0 
[61506.949409] cpu:0 nvcsw: 1 nivcsw: 0 
[61506.949415] cpu:2 nvcsw: 1 nivcsw: 0 
[61506.949428] cpu:3 nvcsw: 1 nivcsw: 0 
[61507.949363] exipred 
[61507.949428] cpu:1 nvcsw: 2 nivcsw: 0 
[61507.949435] cpu:0 nvcsw: 2 nivcsw: 0 
[61507.949441] cpu:2 nvcsw: 2 nivcsw: 0 
[61507.949448] cpu:3 nvcsw: 2 nivcsw: 0 
[61508.949391] exipred 
[61508.949457] cpu:0 nvcsw: 3 nivcsw: 0 
[61508.949464] cpu:1 nvcsw: 3 nivcsw: 0 
[61508.949471] cpu:2 nvcsw: 3 nivcsw: 0 
[61508.949478] cpu:3 nvcsw: 3 nivcsw: 0 
[61509.949420] exipred 
[61509.949477] cpu:1 nvcsw: 4 nivcsw: 0 
[61509.949484] cpu:2 nvcsw: 4 nivcsw: 0 
[61509.949491] cpu:3 nvcsw: 4 nivcsw: 0 

To implement this, i simpley just added more task_structs to hold the 3 other kthreads like so:
struct task_struct * thread_a;
struct task_struct * thread_b;
struct task_struct * thread_c;
struct task_struct * thread_d;
And to start the threads i wrote the following lines of code:
    thread_a = kthread_create(thread_fn, NULL, "thread_a");
    thread_b = kthread_create(thread_fn, NULL, "thread_b");
    thread_c = kthread_create(thread_fn, NULL, "thread_c");
    thread_d = kthread_create(thread_fn, NULL, "thread_d");

    kthread_bind(thread_a,0);
    kthread_bind(thread_b,1);
    kthread_bind(thread_c,2);
    kthread_bind(thread_d,3);
They all use the same thread_fn, meaning that when wake_up_process(thread_*) is called,
all 4 threads running on all 4 cores are made to run the thread_fn.

log_sec=1 log_nsec=100000000:
[62344.923675] GOOD NEWS EVERYONE: timer lab1 module loaded 
[62346.023792] exipred 
[62346.023858] cpu:1 nvcsw: 1 nivcsw: 0 
[62346.023864] cpu:2 nvcsw: 1 nivcsw: 0 
[62346.023883] cpu:0 nvcsw: 1 nivcsw: 0 
[62346.024567] cpu:3 nvcsw: 1 nivcsw: 1 
[62347.123913] exipred 
[62347.123975] cpu:0 nvcsw: 2 nivcsw: 0 
[62347.123982] cpu:1 nvcsw: 2 nivcsw: 0 
[62347.123989] cpu:3 nvcsw: 2 nivcsw: 1 
[62347.123996] cpu:2 nvcsw: 2 nivcsw: 0 
[62348.224031] exipred 
[62348.224088] cpu:0 nvcsw: 3 nivcsw: 0 
[62348.224095] cpu:1 nvcsw: 3 nivcsw: 0 
[62348.224102] cpu:2 nvcsw: 3 nivcsw: 0 
[62348.224109] cpu:3 nvcsw: 3 nivcsw: 1 
[62349.324143] exipred 

log_sec=0 log_nsec=500000000:
[62414.163497] GOOD NEWS EVERYONE: timer lab1 module loaded 
[62414.663522] exipred 
[62414.663582] cpu:0 nvcsw: 1 nivcsw: 0 
[62414.663604] cpu:3 nvcsw: 1 nivcsw: 0 
[62414.663611] cpu:2 nvcsw: 1 nivcsw: 0 
[62414.663618] cpu:1 nvcsw: 1 nivcsw: 0 
[62415.163548] exipred 
[62415.163618] cpu:0 nvcsw: 2 nivcsw: 0 
[62415.163625] cpu:1 nvcsw: 2 nivcsw: 0 
[62415.163632] cpu:2 nvcsw: 2 nivcsw: 0 
[62415.163639] cpu:3 nvcsw: 2 nivcsw: 0 
[62415.663575] exipred 
[62415.663699] cpu:3 nvcsw: 3 nivcsw: 0 
[62415.663706] cpu:0 nvcsw: 3 nivcsw: 0 
[62415.663713] cpu:2 nvcsw: 3 nivcsw: 0 
[62415.663736] cpu:1 nvcsw: 3 nivcsw: 0 
[62416.163595] exipred 
[62416.163656] cpu:0 nvcsw: 4 nivcsw: 0 
[62416.163669] cpu:2 nvcsw: 4 nivcsw: 0 
[62416.163681] cpu:3 nvcsw: 4 nivcsw: 0 
[62416.163697] cpu:1 nvcsw: 4 nivcsw: 0 
[62416.663618] exipred 

log_sec=5 log_nsec=000000000:
[Mar 4 12:11] GOOD NEWS EVERYONE: timer lab1 module loaded 
[  +5.000091] exipred 
[  +0.000075] cpu:0 nvcsw: 1 nivcsw: 0 
[  +0.000007] cpu:1 nvcsw: 1 nivcsw: 0 
[  +0.000007] cpu:2 nvcsw: 1 nivcsw: 0 
[  +0.000007] cpu:3 nvcsw: 1 nivcsw: 0 
[  +4.999977] exipred 
[  +0.000058] cpu:0 nvcsw: 2 nivcsw: 0 
[  +0.000006] cpu:1 nvcsw: 2 nivcsw: 0 
[  +0.000008] cpu:2 nvcsw: 2 nivcsw: 0 
[  +0.000007] cpu:3 nvcsw: 2 nivcsw: 0 
[  +4.999988] exipred 
[  +0.000073] cpu:2 nvcsw: 3 nivcsw: 0 
[  +0.000009] cpu:3 nvcsw: 3 nivcsw: 0 
[  +0.000013] cpu:1 nvcsw: 3 nivcsw: 0 
[  +0.000027] cpu:0 nvcsw: 3 nivcsw: 0 
[  +4.999931] exipred 
[  +0.000065] cpu:3 nvcsw: 4 nivcsw: 0 
[  +0.000008] cpu:2 nvcsw: 4 nivcsw: 0 
[  +0.000013] cpu:1 nvcsw: 4 nivcsw: 0 
[  +0.000008] cpu:0 nvcsw: 4 nivcsw: 0 

when increasing the number of time, it's more likley as demonstrated to have
increased number of involuntary context switches. as explained in the prior 
question, the kernel is waiting for a longer period of time meaning higher chance
something with a higher priority needs to run so we make the CPU run another
process while the timer is expiring; however, the number of involuntary context
switches did not increase with multi threading, most likely because we're spreading 
out our work over 4 different cores and these threads won't interefere with 
each other as well as the fact that the PI is not running anything intensive
meaning there's no need for involuntary switches. involuntary switches is a bad thing.
Obviously the voluntary context switch numbers won't be affected since we increment
with every single time the timer expires. The timer variation doesn't seem to be affected
for example when i ran it with log_sec=5 and log_nsec=0 we had a += 0.001 time 
which is consistent throughout all of the trials and when compared between kthread.c and my
multithread.c programs. 

### Screenshot is attached no.6

### System Performance no.7

On one of the screenshots we can see that thread_a runs for a bit longer than thread_b. 
This is a clear indication that the threads don't necessarily run to completion all the time.
Since they're in k-mode, schedulers and interrupts can interrupt them just as how 
thread_b was interrupted by sched_switch in the screenshot. I also ran dense_mm at the same time
hence, we can see dense_mm in the kernelshark window. 

Jitter min: 0.000004
Jitter max: 0.000008
Jitter Mean: 0.00000305

over 5 wake ups for thread_a:
thread_a min: 4.50000079581514e-05
thread_a max: 0.00015300000086426735
thread_a mean: 0.00011520000407472253


### Development Effort no.8
I worked on it by myself



