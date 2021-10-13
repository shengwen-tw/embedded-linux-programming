#include <sys/mman.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#define MAX_SAFE_STACK (8 * 1024)

/* pre-fault the stack, so that a future stack fault will not undermine
 * deterministic behavior
 */
void prefault_stack(void)
{
	unsigned char dummy[MAX_SAFE_STACK];
	memset(dummy, 0, MAX_SAFE_STACK);
}

/* accurate delay function for real-time application */
void delay_ms(int delay_ms)
{
	struct timespec ts;	
	clock_gettime(CLOCK_MONOTONIC, &ts);

	ts.tv_nsec += delay_ms * 10e6;
	if(ts.tv_nsec >= 10e9) {
		ts.tv_nsec -= 10e9;
		ts.tv_sec++;
	}

	/* use CLOCK_MONOTONIC and TIMER_ABSTIME for accurate timing */
	clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
}

int main(void)
{
	/* change process priority */
	struct sched_param sp;
	sp.sched_priority = 30;
	if(pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp)) {
		printf("Failed to set priority of the proceess.\n");
	}

	/* lock the memory page of the real-time application to prevent memory swapping */
	if(mlockall(MCL_FUTURE | MCL_CURRENT)) {
		printf("Failed to lock the memory, try to execure the program with root privilege.\n");
	}

	/* stack pre-fault */
	prefault_stack();

	/* your real-time program's main loop */
	while(1) {
		printf("Real-time application is running...\n");
		delay_ms(1000);
	}

	return 0;
}
