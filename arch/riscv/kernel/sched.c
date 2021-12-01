#include "put.h"
#include "sched.h"
#define offset (0xffffffe000000000 - 0x80000000)
#define TASK_BASE 0xffffffe000ff0000

struct task_struct *current;
struct task_struct *task[NR_TASKS];
long PRIORITY_INIT_COUNTER[NR_TASKS] = {0, 1, 2, 3, 4};
long COUNTER_INIT_COUNTER[NR_TASKS] = {0, 1, 2, 3, 4};

extern void init_epc(void);
extern void __switch_to(struct task_struct *current, struct task_struct *next);
extern unsigned int rand();
extern uint64_t cur;

void task_init(void)
{
	for (int i = 1; i <= LAB_TEST_NUM; i++)
	{
		puts("task init...\n");

		//initialize task[0]
		current = (struct task_struct *)TASK_BASE;
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
			task[i] = (struct task_struct *)(TASK_BASE + i * TASK_SIZE);
			task[i]->counter = COUNTER_INIT_COUNTER[i];
			task[i]->priority = PRIORITY_INIT_COUNTER[i];
			task[i]->pid = i;
			task[i]->thread.sp = (unsigned long long)task[i] + TASK_SIZE;
			task[i]->thread.ra = (void *)(init_epc);

			puts("[PID = ");
			puti(task[i]->pid);
			puts("] Process Create Successfully! counter = ");
			puti(task[i]->counter);
			puts("\n");
		}
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
	unsigned char next;
	unsigned char i;
	for (i = 4; i > 0; i--)
		if (task[i]->counter != 0)
			break;
	if (i == 0)
	{
		for (int i = 1; i < LAB_TEST_NUM; i++)
		{
			task[i]->counter = rand();
			puts("[PID = ");
			puti(task[i]->pid);
			puts("] Reset counter = ");
			puti(task[i]->counter);
			puts("\n");
		}
		schedule();
	}
	else
	{
		next = i;
		for (i = i - 1; i > 0; i--)
			if (task[i]->counter > 0 && task[i]->counter < task[next]->counter)
				next = i;
	}

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
		struct task_struct *cur = current;
		current = next;
		__switch_to(cur, next);
	}
}

void dead_loop(void)
{
	while (1)
	{
		continue;
	}
}
