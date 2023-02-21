# os_lab_1

### Names:

1) Kenichi Matsuo ken.m@wustl.edu

### Module Design:

I merely declared ulongs for log_sec and log_nsec set to 1 and 0 respectively,
which would then be overwritten by the call to module_params. So if I altered the params
then the variables won't have the default values. I also just created
an expiration method that printk'ed the word "expired \n" so we can see
what time the print out happened.
To load my module I just took the same format as the hello world simple
kernel module from the textbook and altered the names of the init and exit
functions



### Timer Design and Evaluation

Implementing this was simple, in order to see what the time stamp in which the timer
was reset, all i had to do was to call on printk() every time the timer needed to be 
resent whenever it expired. By doing so, the time stamp would be displayed right before
the kernel print logs. 

When we call on the skm_lab1_init, or the module's init, we set the timer.function 
field to be the expiration function. This is what enables us to call on printk
whenevr the clock has expired and needs to be reset. 

The following is a snippet of the print out:

[241260.933076] exipred 
[241261.033078] exipred 
[241261.133083] exipred 
[241261.233093] exipred 
[241261.333098] exipred 
[241261.433099] exipred 
[241261.533106] exipred 
[241261.633122] exipred 
[241261.733116] exipred 
[241261.833126] exipred 
[241261.933130] exipred 
[241262.033140] exipred 
[241262.133142] exipred 
[241262.233150] exipred 
[241262.333154] exipred 
[241262.433163] exipred 

We can see that the word 'expired' is printed every 0.1 seconds













### Development Effort
I worked on it by myself



