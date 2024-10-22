#define _POSIX_C_SOURCE 200112L

#include <assert.h>		/* assert() */
#include <stdio.h>		/* printf(), perror() */
#include <stdlib.h>		/* size_t, atoi(), setenv(), unsetenv()*/
#include <unistd.h>		/* getpid() */
#include <signal.h>		/* sigaction, SA_SIGINFO, SIGUSR1, SIGUSR2 */

#include "watch_dog_private.h" /* DeserializeToArgs(), Start(), End(), ZerofyCounter() */

int main(int argc, char *argv[])
{
	struct thread_args args = {0};
	char *argv_buffers[BUFSIZE] = {0};
	char argv_buffer_space[BUFSIZE][BUFSIZE] = {'\0'};
	char pid_buffer[BUFSIZE] = {'\0'};
	size_t i = 0;
	int setenv_status = 0;
	
	assert(argv);
	assert(argv[1]);
	
	if (argc < 2)
	{
		printf("Usage: %s <serialized_args>\n", argv[0]);
		return 1;
	}
	
	printf("\nStarting watch dog process...\n");
	
	/* create empty buffer for argv to revive */
	for (i = 0; i < BUFSIZE; i++) 
	{
		argv_buffers[i] = argv_buffer_space[i];
	}

	/* deserialize argv[1] to args */
	DeserializeToArgs(argv[1], &args, argv_buffers);
	
	/* set env var to watchdog process PID */
	sprintf(pid_buffer, "%d", getpid());
	setenv_status = setenv("WD_PID", pid_buffer, 1);
	if (setenv_status != 0)
	{
		perror("setenv");
	}
	else
	{
		printf("\nEnvironment variable WD_PID set to: %s\n", pid_buffer);
	}

	InitSigactions();
	
	Start(&args);

	printf("\nWatch dog process finished.\n");
	
	/* unset env var */
	unsetenv("WD_PID");
	
	return 0;
}

