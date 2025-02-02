#include <linux/module.h>
#include <linux/init.h>
#include <linux/gpio/consumer.h>


static struct gpio_desc * led;
#define GPIO_LED 2
#define GPIO_OFFSET 0
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("eyas");
MODULE_DESCRIPTION("Example for using gpio without dev tree");

/**
 * @brief this function is called as module is loaded into the kernel(insmod)
 */


static int __init ModuleInit(void){
	int status;
	led = gpio_to_desc(GPIO_LED + GPIO_OFFSET);
	if (!led){
		printk("Failed to get pin 2 \n");
		return ENODEV;
	}


	status = gpiod_direction_output(led, 0);
	if (status){
		printk("Failed to set direction to output\n");
		return status;
	}


	gpiod_set_value(led, 1);

	printk("\n Eyas GPIO module is loaded now!!!\n");
	return 0;
}


/** 
 * @brief this function is called , when the module is removed using (rmmod)
 */
static void __exit ModuleExit(void){
	gpiod_set_value(led, 0);
	printk("I'm setting back the LED to 0\n");
	printk("I'm removed from the kernal when used rmmod, bye now!!!\n\n\n");
	// no need to return as I'm a void function
}



module_init(ModuleInit);
module_exit(ModuleExit);




