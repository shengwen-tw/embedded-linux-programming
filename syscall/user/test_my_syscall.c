#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
	long retval = syscall(398);

	return 0;
}
