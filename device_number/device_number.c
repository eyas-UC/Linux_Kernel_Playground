#include <linux/module.h>
#include <linux/init.h>



/* Meta Information */
MODULE_LICENSE("gpl");
MODULE_AUTHOR("eyas");
MODULE_DESCRIPTION("A HELLO WORLD LKM");

/**
 * @brief this function is called as module is loaded into the kernel(insmod)
 */


static int __init ModuleInit(void){
	printk("hello this module is loaded now!!!\n\n\n yay!!\n");
	return 0;
}


/** 
 * @brief this function is called , when the module is removed using (rmmod)
 */
static void __exit ModuleExit(void){
	printk("I'm removed from the kernal when used rmmod, bye now!!!\n\n\n");
	// no need to return as I'm a void function
}



module_init(ModuleInit);
module_exit(ModuleExit);




