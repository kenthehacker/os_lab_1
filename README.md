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






