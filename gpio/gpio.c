#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

// pic a non-used dev when cat /proc/devices
#define DEV_NO 8 



/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("eyas");
MODULE_DESCRIPTION(" register device numver and add callback func LKM");

/* buffer for the data */
static char buffer [255];
static int buffer_ptr; // current buffer size
/* functions prototypes */
static int driver_open(struct inode* device_file, struct file * instance);
static int driver_close(struct inode* device_file, struct file * instance);
static ssize_t driver_read(struct file* File, char* user_buffer, size_t buff_size,loff_t* offs);
static ssize_t driver_write(struct file* File, const char* user_buffer, size_t buff_size,loff_t* offs);
/* file operations file_ops */
static struct file_operations file_ops = {
	.owner  = THIS_MODULE,
	.open   = driver_open,
	.release= driver_close,
	.read = driver_read,
	.write = driver_write,
};

/* vars for dev and dev class */
static dev_t my_device_number;
static struct class * my_class;
static struct cdev my_device;
#define DRIVER_NAME "eyasdriver"
#define DRIVER_CLASS "eyasmoduleclass"

/**
* @brief reads data out of the buffer
* @param file* File pointer to the file
* @param char* buffer  pointer to the user buffer
* @param size_t count size of the buffer
* @param loff_t* offs offset
*/
static ssize_t driver_read(struct file* File, char* user_buffer, size_t buff_size, loff_t* offs)
{
	int to_copy, not_copied, delta;

	/* get the amount of data that needs to be copied */
	to_copy = min (buff_size, buffer_ptr);
	/* copy data to user*/
	not_copied = copy_to_user(user_buffer, buffer,to_copy); // returns amount of user_data not copied

	/* calculate data */
	delta = to_copy - not_copied;
	printk("reading------>to_copy=%d, not_copied=%d, delta=%d",to_copy,not_copied,delta);
	return delta;
}

/**
* @brief writes data out of the buffer
* @param file* File pointer to the file
* @param const char* buffer  pointer to the user buffer (note it is const)
* @param size_t count size of the buffer
* @param loff_t* offs offset
*/
static ssize_t driver_write(struct file* File, const char* user_buffer, size_t buff_size,loff_t* offs)
{
	int to_copy, not_copied, delta;

	/* get the amount of data that needs to be copied */
	to_copy = min (buff_size, sizeof(buffer));
	/* copy data from user*/
	not_copied = copy_from_user(buffer, user_buffer,to_copy);
	buffer_ptr = to_copy;

	/* calculate data */
	delta = to_copy - not_copied;
	printk("writing------>to_copy=%d, not_copied=%d, delta=%d",to_copy,not_copied,delta);
	return delta;

}


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

	//int ret_val;
	printk("device number --- open was called\n\n---------------\n");
	printk("hello this module is loaded now!!!\n\n\n yay!!\n");


	// allocate a device number
	if(alloc_chrdev_region(&my_device_number,0,1, DRIVER_NAME)<0)
	{
		printk(" device number could not be allocated! \n");
		return -1;
	}
	printk("device number allocated correctly");
	printk("read_write - device number Majro %d, Minor %d is registered now!\n",my_device_number>>20,my_device_number);
	


	/* create device class */
	if ((my_class = class_create(THIS_MODULE, DRIVER_CLASS))==NULL )
	{
		printk("Device class can not be created! \n");
		/*remove device class and return -1*/
		unregister_chrdev_region(my_device_number,1);
		return -1;
	}
	/* create device file */
	if(device_create(my_class,NULL,my_device_number,NULL, DRIVER_NAME)==NULL)
	{
		printk("devuce file can not be created!\n");
		/* remove device file and device class and return -1*/
		class_destroy(my_class);
		unregister_chrdev_region(my_device_number,1);
		return -1;
	}
	/* now try to initialize the device file*/
	cdev_init(&my_device,&file_ops);

	/* if all is good then register  device file to kernel*/
	if(cdev_add(&my_device, my_device_number,1)  == -1)
	{
		printk("registering to kernel fail\n");
		device_destroy(my_class,my_device_number);
		class_destroy(my_class);
		unregister_chrdev(my_device_number,DRIVER_NAME);
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
	cdev_del(&my_device);
	device_destroy(my_class,my_device_number);
	class_destroy(my_class);
	unregister_chrdev(my_device_number,DRIVER_NAME);
	printk("removing device with name %s  number %d \n",DRIVER_NAME,my_device_number);
	printk("good bye!!!!!\n");
}



module_init(ModuleInit);
module_exit(ModuleExit);




