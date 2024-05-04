#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>


// pic a non-used dev when cat /proc/devices
#define DEV_NO 8 



/* Meta Information */
MODULE_LICENSE("gpl");
MODULE_AUTHOR("eyas");
MODULE_DESCRIPTION(" register device numver and add callback func LKM");


static int driver_open(struct inode* device_file, struct file * instance);
static int driver_close(struct inode* device_file, struct file * instance);
static struct file_operations file_ops = {
	.owner  = THIS_MODULE,
	.open   = driver_open,
	.release= driver_close,
};
/**
 * @brief this function is called when openning our device file
 */
static int driver_open(struct inode* device_file, struct file * instance){
	printk("device number --- open was called\n\n---------------\n");
	return 0;
}


/**
 * @brief this function is called when closing our device file
 */
static int driver_close(struct inode* device_file, struct file * instance){
	printk("---------------------------\n device number is closed now\n\n\n\n------------------------\n");
	return 0;
}



/**
 * @brief this function is called when openning our device file
 */
static int __init ModuleInit(void){

	int ret_val;
	printk("device number --- open was called\n\n---------------\n");
	printk("hello this module is loaded now!!!\n\n\n yay!!\n");
	// now lets set the device number we defined above
	ret_val = register_chrdev(DEV_NO,"my_driver_number",&file_ops);
	if(ret_val ==0)
	{
		printk("device number is registered major is: %d, and Minor is %d----\n",DEV_NO,0);
	}
	else if(ret_val>0)
	{
		
		printk("device number is registered major is: %d, and Minor is %d----\n",DEV_NO>>20,ret_val&0xfffff0);
	}
	else
	{
		printk("could not register the device number");
		return -1;
	}
	return 0;
}




/** 
 * @brief this function is called , when the module is removed using (rmmod)
 */
static void __exit ModuleExit(void){
	printk("I'm removed from the kernal when used rmmod, bye now!!!\n\n\n");
	// no need to return as I'm a void function
	// let's unregister the device no at exit
	unregister_chrdev(DEV_NO,"my_driver_number");
	printk("good bye!!!!!\n");
}



module_init(ModuleInit);
module_exit(ModuleExit);




