#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
////////////////////////////////////////////////////////////////////
// simple_kernel_module_lab1.c - lab1 part 2 make simple k-module //
////////////////////////////////////////////////////////////////////




/*
 * skm_lab1_init - init function returns 0 on success
 */

static int skm_lab1_init(void){
	printk(KERN_ALERT "Howdie, this is part 2 of lab1 \n");
	return 0;
}



/*
 * skm_lab1_exit - exit func returns 0 on success
 */
static int skm_lab1_exit(void){
	printk(KERN_ALERT "skm_lab1 module offloaded \n");
	return 0;
}

module_init(skm_lab1_init);
module_exit(skm_lab1_exit);

MODULE_AUTHOR("kenthehacker");
MODULE_DESCRIPTION("lab1 part 2");

