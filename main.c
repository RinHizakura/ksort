#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include "sort.h"

MODULE_LICENSE("GPL");

#define DEVICE_NAME "ksort"

static dev_t dev = -1;
static struct cdev cdev;
static struct class *class;

struct workqueue_struct *workqueue;

static int ksort_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int ksort_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t ksort_read(struct file *file,
                          char *buf,
                          size_t size,
                          loff_t *offset)
{
    return 0;
}

static ssize_t ksort_write(struct file *file,
                           const char *buf,
                           size_t size,
                           loff_t *offset)
{
    unsigned long len;

    void *sort_buffer = kmalloc(size, GFP_KERNEL);
    if (!sort_buffer)
        return 0;

    len = copy_from_user(sort_buffer, buf, size);
    if (len != 0)
        return 0;

    sort_main(sort_buffer, size);

    kfree(sort_buffer);
    return size;
}

static const struct file_operations fops = {
    .read = ksort_read,
    .write = ksort_write,
    .open = ksort_open,
    .release = ksort_release,
    .owner = THIS_MODULE,
};

static int __init ksort_init(void)
{
    struct device *device;

    printk(KERN_INFO DEVICE_NAME ": loaded\n");

    if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0)
        return -1;

    class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(class)) {
        goto error_unregister_chrdev_region;
    }

    device = device_create(class, NULL, dev, NULL, DEVICE_NAME);
    if (IS_ERR(device)) {
        goto error_class_destroy;
    }

    cdev_init(&cdev, &fops);
    if (cdev_add(&cdev, dev, 1) < 0)
        goto error_device_destroy;

    workqueue = alloc_workqueue("ksortq", 0, WQ_MAX_ACTIVE);
    if (!workqueue)
        goto error_cdev_del;

    return 0;

error_cdev_del:
    cdev_del(&cdev);
error_device_destroy:
    device_destroy(class, dev);
error_class_destroy:
    class_destroy(class);
error_unregister_chrdev_region:
    unregister_chrdev_region(dev, 1);

    return -1;
}

static void __exit ksort_exit(void)
{
    /* Since this will do drain_workqueue, we don't need to flush it explicitly
     */
    destroy_workqueue(workqueue);

    cdev_del(&cdev);
    device_destroy(class, dev);
    class_destroy(class);
    unregister_chrdev_region(dev, 1);

    printk(KERN_INFO DEVICE_NAME ": unloaded\n");
}

module_init(ksort_init);
module_exit(ksort_exit);
