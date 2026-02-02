#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("YOU");
MODULE_DESCRIPTION("HJKHJK");

static struct kobject *my_kobj; // if not craeting like /sys/kernel/ cna use &THIS_MODULE->mobj.kobj; 
int i,b;
#define SHOW(a,b) sprintf(b,"%d\n",a)
#define STORE(a,b) kstrtoint(b,10,&a)
int ab;
static ssize_t i_show(struct kobject *o,struct kobj_attribute *a,char *buf){
    return SHOW(i,buf);
}
static ssize_t i_store(struct kobject *o,struct kobj_attribute *a,const char *buf,size_t count){
    STORE(i,buf);
    return count;
}

static ssize_t b_show(struct kobject *o,struct kobj_attribute *a,char *buf){
    return SHOW(b,buf);
}
static ssize_t b_store(struct kobject *o,struct kobj_attribute *a,const char *buf,size_t count){
    STORE(b,buf);
    return count;
}

static int ab_read(struct seq_file *f,void *v){
    seq_printf(f,"%d\n",ab);
    return 0;
}

static int ab_open(struct inode *inode,struct file *file){
    return single_open(file,ab_read,NULL);
}


static ssize_t ab_write(struct file *f,const char __user *buffer,size_t count,loff_t *pos){
    char buff[20];
    if (count>=sizeof(buff)) return -EINVAL;
    if (copy_from_user(buff,buffer,count)){
        return -EFAULT;
    }
    buff[count]='\0';
    STORE(ab,buff);
    return count;

}
static const struct proc_ops ab_proc={
    .proc_read=seq_read,
    .proc_lseek=seq_lseek,
    .proc_open=ab_open,
    .proc_write=ab_write,
    .proc_release=single_release,
};
static struct kobject_attribute i_attr=__ATTR(i,0660,i_show,i_store);
static struct kobject_attribute b_attr=__ATTR(b,0660,b_show,b_store);

static struct attribute *attrs[]={
    &i_attr.attr,
    &b_attr.attr,
    NULL,

};
static struct attribute_group attr_g={
    .attrs=attrs,
};

static int __init hello_init(void){
    int retval;
    my_kobj=kobject_create_and_add("myone",kernel_kobj);
    if(!my_kobj) return -ENOMEM;
    retval=sysfs_create_group(my_kobj,&attr_g);
    if(retval){
        kobject_put(my_kobj);
        return retval;
    }
    proc_create("ab",0644,NULL,&ab_proc);
    pr_info("hi:loaded\n");
    return retval;
}
static void __exit hello_exit(void){
    kobject_put(my_kobj);
    remove_proc_entry("ab",NULL);
    pr_info("unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);


/*  
*   obj-m +=hello_.o
*all:
*   make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
*clean:
*   make -C  /lib/modules/$(shell uname -r)/build  M=$(PWD) clean
*/
