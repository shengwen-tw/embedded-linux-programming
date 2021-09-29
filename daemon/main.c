#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void daemon_loop(void)
{
	char *log_file = "/tmp/daemon.log";
	char *str = "hello world\n";

	/* open the log file */
	int fd = open(log_file, O_CREAT | O_WRONLY);
	chmod(log_file, 777);

	while(1) {
		/* write something to the file every ten seconds */
		write(fd, str, strlen(str));
		sleep(10);
	}
}

int main(void)
{
	/* fork new procss for daemon */
	pid_t pid = fork();

	/* failed to create child process */
	if(pid < 0) {
		/* failed to fork a procss */
		printf("failed to fork a process for the daemon.\n");
		exit(EXIT_FAILURE);
	}

	/* parent process */
	if(pid > 0) {
		/* stop parent procss after fork() */
		exit(EXIT_SUCCESS);
	}

	/* modify the file mode mask to let the daemon read or write to
           the file (log) it needs */
	umask(0);

	/* need to set the session id for the child process otherwise
           it becomes a orphan */
	pid_t sid = setsid();
	if(sid < 0) {
		/* failed to set sid */
		printf("failed to set session id for the daemon.\n");
		exit(EXIT_FAILURE);
	} 

	/* change to working directory to a place that always exists */
	if(chdir("/") < 0) {
		/* failed to change directory */
		printf("failed to set the working directory for the daemon\n\r");
		exit(EXIT_FAILURE);
	}

	/* close the standard file descriptors since daemon programs
           do not interact with the user via terminal */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	/* start running the daemon code from here */
	daemon_loop();

	exit(EXIT_SUCCESS);
}
