#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_AUTHOR("Vasilkevych Ivan IO-06");
MODULE_DESCRIPTION("lab 7");
MODULE_LICENSE("Dual BSD/GPL");


struct head_list {
	ktime_t time;
	struct list_head list_node;
};

static struct list_head my_list_head = LIST_HEAD_INIT(my_list_head);

static int greetnum = 1;
module_param(greetnum, uint, 0444);
MODULE_PARM_DESC(greetnum, "String: how many times Hello world was printed");

static int __init hello_init(void)
{
	uint i = 0;
	struct head_list *tail;
	
	if (greetnum == 0) {pr_warn("param equals 0");}
       	else if (greetnum >= 5 && greetnum <= 10) {pr_warn("param is 5-10");} 
	BUG_ON(greetnum > 10);

	for (i = 0 ; i < greetnum; i++) {
		
		tail = kmalloc(sizeof(struct head_list), GFP_KERNEL); 
		if (i == 5)
		    tail = 0;
		if(ZERO_OR_NULL_PTR(tail))
			goto error;
		tail->time = ktime_get();
		list_add_tail(&(tail->list_node), &my_list_head);
		pr_info("Hello world\n");}

	return 0;

error:
	{
		struct head_list *md, *tmp;
		pr_err("kmalloc out of memory");
		list_for_each_entry_safe(md, tmp, &my_list_head, list_node) {
			list_del(&md->list_node);
			kfree(md);
		}
		BUG();
		return -ENOMEM;
	}
	
}

static void __exit hello_exit(void)
{
	struct head_list *md, *tmp;

	list_for_each_entry_safe(md, tmp, &my_list_head, list_node) {
		pr_info("time: %lld\n", md->time);
		list_del(&md->list_node);
		kfree(md);
	}
}
module_init(hello_init);
module_exit(hello_exit);
