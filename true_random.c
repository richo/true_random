#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

MODULE_LICENSE("BSD");
MODULE_AUTHOR("Richo Healey <richo@psych0tik.net>");
MODULE_DESCRIPTION("An ieee approved random number module for linux");

/* IEEE Approved */
const int random_number = '4';
const char *node_name = "true_random";

static int __init rand_init(void);
static void __exit rand_exit(void);
int dev_major = 0;

module_init(rand_init);
module_exit(rand_exit);

int rand_open(struct inode *inode,struct file *filep);
int rand_release(struct inode *inode,struct file *filep);
ssize_t rand_read(struct file *filep,char *buff,size_t count,loff_t *offp );
ssize_t rand_write(struct file *filep,const char *buff,size_t count,loff_t *offp );

struct file_operations rand_fops={
    .open = NULL,
    .read = rand_read,
    .write = rand_write,
    .release = NULL,
};

ssize_t rand_read(struct file *filep,char *buff,size_t count,loff_t *offp )
{
    int i;
    for(i = 0; i < count; i++) {
      put_user(random_number, buff+i);
        }
    return count;

}

ssize_t rand_write(struct file *filep,const char *buff,size_t count,loff_t *offp) {
   return -EINVAL;
}

static int __init rand_init(void)
{
    int errno = 0;
    int ret = 0;

    if ((ret = register_chrdev(0, node_name, &rand_fops)) < 0) {
        errno++;
    }

    if (errno > 0)
        return -1;

    dev_major = ret;

    printk(KERN_INFO "Loading ieee approved random number generator with major: %d\n", dev_major);
    return 0;
}
static void __exit rand_exit(void)
{
    unregister_chrdev(dev_major, node_name);
    printk(KERN_INFO "Unloading random number generator\n");
}
