# os_lab_1

### 1 Names:

Kenichi Matsuo ken.m@wustl.edu

### 2 Module Design:

I merely declared ulongs for log_sec and log_nsec set to 1 and 0 respectively,
which would then be overwritten by the call to module_params. So if I altered the params
then the variables won't have the default values. I also just created
an expiration method that printk'ed the word "expired \n" so we can see
what time the print out happened.
To load my module I just took the same format as the hello world simple
kernel module from the textbook and altered the names of the init and exit
functions



### 3 Timer Design and Evaluation

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


### 4 


