#include "types.h"
#include "user.h"
#include "syscall.h"

void mythread(void *arg)
{
	printf(1, "%s\n", (char *)arg);
	exit();
}

int
main()
{
	void *stacks[2];
	void *args[2];
	void *return_stack = 0;

	stacks[0] = (void *)malloc(4096);
	stacks[1] = (void *)malloc(4096);
	args[0] = "A";
	args[1] = "B";

	printf(1, "main: begin\n");
	thread_create(mythread, stacks[0], args[0]);
	thread_create(mythread, stacks[1], args[1]);

	thread_join((void **)&return_stack);
	thread_join((void **)&return_stack);
	printf(1, "main: end\n");
	exit();

	return 0;
}