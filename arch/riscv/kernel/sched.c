#include "put.h"
#include "sched.h"
#define offset (0xffffffe000000000 - 0x80000000)

struct task_struct *current;
struct task_struct *task[NR_TASKS];
long PRIORITY_INIT_COUNTER[NR_TASKS] = {0, 1, 2, 3, 4};
long COUNTER_INIT_COUNTER[NR_TASKS] = {0, 2, 2, 1, 4};

extern void init_epc(void);
extern void __switch_to(struct task_struct *current, struct task_struct *next);
extern unsigned int rand();
extern uint64_t cur;			//获取当前页表已被分配的物理页
extern unsigned long long _end; //由于开启了MMU, 此时&_end的地址为虚拟地址

void task_init(void)
{

	puts("task init...\n");
	//initialize task[0]
	cur++; //为进程开辟新的物理空间
	current = (struct task_struct *)((uint64_t)&_end + cur * TASK_SIZE);
	current->state = TASK_RUNNING;
	current->counter = COUNTER_INIT_COUNTER[0];
	current->priority = PRIORITY_INIT_COUNTER[0];
	current->blocked = 0;
	current->pid = 0;
	task[0] = current;
	task[0]->thread.sp = (unsigned long long)task[0] + TASK_SIZE;

	//set other 4 tasks
	for (int i = 1; i <= LAB_TEST_NUM; ++i)
	{
		cur++;
		task[i] = (struct task_struct *)((uint64_t)&_end + cur * TASK_SIZE);
		task[i]->counter = COUNTER_INIT_COUNTER[i];
		task[i]->priority = PRIORITY_INIT_COUNTER[i];
		task[i]->pid = i;
		task[i]->thread.sp = (unsigned long long)task[i] + TASK_SIZE;
		task[i]->thread.ra = (uint64_t)(init_epc);

		puts("[PID = ");
		puti(task[i]->pid);
		puts("] Process Create Successfully! counter = ");
		puti(task[i]->counter);
		puts("\n");
	}
}

void do_timer(void)
{
	puts("[PID = ");
	puti(current->pid);
	puts("] Context Calculation: ");
	puts("counter = ");
	puti(current->counter);
	puts("\n");

	current->counter--;
	if (current->counter <= 0)
		schedule();
}

void schedule(void)
{
	unsigned char next = 0;
	unsigned char i;
	int min_counter = 100;
	for (i = LAB_TEST_NUM; i > 0; i--)
	{
		if (task[i]->counter > 0 && task[i]->counter < min_counter)
		{
			next = i;
			min_counter = task[i]->counter;
		}
	}
	if (!next) 	//当前task1-4的时间片已经用完
	{
		for (int j = 1; j <= LAB_TEST_NUM; j++)
		{
			task[j]->counter = rand();
			puts("[PID = ");
			puti(task[j]->pid);
			puts("] Reset counter = ");
			puti(task[j]->counter);
			puts("\n");
		}
		schedule();
		return; //important!!!!!没有可能会导致进程跳转错误
	}

	/*另一种写法*/
	// while (1)
	// {
	// 	for (i = 4; i > 0; i--)
	// 		if (task[i]->counter != 0)
	// 			break;

	// 	if (i)
	// 		break;
	// 	for (int j = 1; j <= LAB_TEST_NUM; j++)
	// 	{
	// 		task[j]->counter = rand();
	// 		puts("[PID = ");
	// 		puti(task[j]->pid);
	// 		puts("] Reset counter = ");
	// 		puti(task[j]->counter);
	// 		puts("\n");
	// 	}
	// }
	// next = i;
	// for (i = i - 1; i > 0; i--)
	// 	if (task[i]->counter > 0 && task[i]->counter < task[next]->counter)
	// 		next = i;

	if (current->pid != task[next]->pid)
	{
		puts("[!] Switch from task ");
		puti(current->pid);
		puts(" [task struct: ");
		putullHex((uint64_t)current);
		puts(", sp: ");
		putullHex(current->thread.sp);
		puts("] to task ");
		puti(task[next]->pid);
		puts(" [task struct: ");
		putullHex((uint64_t)task[next]);
		puts(", sp: ");
		putullHex(task[next]->thread.sp);
		puts("], prio: ");
		puti(task[next]->priority);
		puts(", counter: ");
		puti(task[next]->counter);
		puts("\n");
	}

	switch_to(task[next]);
}

void switch_to(struct task_struct *next)
{

	if (current != next)
	{
		struct task_struct *prev = current;
		current = next;
		__switch_to(prev, next);
	}
}

void dead_loop(void)
{
	while (1)
	{
		continue;
	}
}