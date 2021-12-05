#include "put.h"
#include "sched.h"

int start_kernel()
{
	const char *msg = "ZJU OS LAB 4     Student1: 3190101094 朱理真 Student2:3190101931 李兆渊\n";
	puts(msg);
	task_init();
	dead_loop();
	return 0;
}
