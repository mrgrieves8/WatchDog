#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int main(void)
{
	pid_t pid;
	char *terminal_argv[] = {"./bin/watch_dog_test", "3", "3", NULL};

	pid = fork();
	if (0 == pid)
	{
		/* Child process: Run the test in a new terminal */
		execvp(terminal_argv[0], terminal_argv);
		perror("execvp failed"); /* This will only be reached if execvp fails */
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		/* Parent process: Sleep forever */
		while (1)
		{
			sleep(1);
		}
	}
	else
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}

	return 0;
}

