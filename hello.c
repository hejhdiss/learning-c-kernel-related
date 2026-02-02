#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL")
MODULE_AUTHOR("YOU")
MODULE_DESCRIPTION("HJKHJK")

static int __init hello_init(void){
    pr_info("hi:loaded\n");
    return 0;
}
static void __exit hello_exit(void){
    pr_info("unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);


/*  
*   obj-m +=hello.o
*all:
*   make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
*clean:
*   make -C  /lib/modules/$(shell uname -r)/build  M=$(PWD) clean
*/
