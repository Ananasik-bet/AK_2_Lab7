#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/bug.h>

#include <linux/ktime.h>

struct personal_list_head {
struct personal_list_head *next;
ktime_t time;
};

MODULE_AUTHOR("Vlad Diachenko <diachenko.vlad@ukr.net>");
MODULE_DESCRIPTION("Hello world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static struct personal_list_head *header;


static uint times = 1;
module_param(times, uint, 0444);
MODULE_PARM_DESC(times, "Times of print Hello World!");


static int __init hello_init(void)
{
	uint i = 0;
	struct personal_list_head *first, *second;

	if (times == 0) {
		pr_warn("Warning: times = 0");
	} else if (times >= 5 && times <= 10) {
		pr_warn("Warning: times between 5 and 10");
	} else if (times > 10) {
		pr_err("Error: invalid value!");
		BUG_ON(1);
	}

	header = kmalloc(sizeof(struct personal_list_head *), GFP_KERNEL);
	first = header;
	for (i = 0; i < times; i++) {
		if (i == 2)
			first->next = kmalloc(0, GFP_KERNEL);
		else 
			first->next = kmalloc(sizeof(struct personal_list_head), GFP_KERNEL);
		first->time = ktime_get();
		pr_info("Hello World!");
		second = first;
		first = first->next;
	}
	if (times != 0) {
		kfree(second->next);
		second->next = NULL;
	}
	pr_info("times: %d\n", times);
	return 0;
}


static void __exit hello_exit(void)
{
	struct personal_list_head *variable;
	while (header != NULL && times != 0) {
		variable = header;
		pr_info("Time : %lld", variable->time);
		header = variable->next;
		kfree(variable);
	}
	if (times == 0)
		pr_info("Print never used because time is 0");
		pr_info("");
}

module_init(hello_init);
module_exit(hello_exit);
