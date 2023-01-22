#include <linux/init.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Vasylkevych Ivan");
MODULE_DESCRIPTION("Lab_work_6");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int greetnum=1;

module_param(greetnum,uint,S_IRUGO);

MODULE_PARM_DESC(greetnum, "Hellow world prints num");

struct event_list {
    struct list_head list;
    ktime_t event_time;


};

static struct list_head event_list_head;


static LIST_HEAD(event_list_head);
void new_event(void);
static int __init hello_init(void)
{
    if (greetnum==0 || (greetnum>=5 && greetnum<=10)) {
        printk(KERN_WARNING "Param is 0 or 5-10");}

    if (greetnum>10) {
        printk(KERN_ERR "Parame<10");
        return -EINVAL;}
  
    int i;
    for (i = 0; i <greetnum; i++) {
        printk(KERN_EMERG "Hello, World!\n");
        new_event();}
  return 0;}

static void __exit hello_exit(void) {
  struct event_list *md, *tmp;
  list_for_each_entry_safe(md, tmp, &event_list_head, list) {
    printk(KERN_EMERG "Time: %lld\n", md->event_time);
    list_del(&md->list);
    kfree(md);
  }
}

void new_event(void) {
    struct event_list *element = NULL;
    element = kmalloc(sizeof(struct event_list), GFP_KERNEL);
    element->event_time = ktime_get();
    list_add_tail(&element->list, &event_list_head);
}

module_init(hello_init);
module_exit(hello_exit);
