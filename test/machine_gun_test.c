#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define WHITE "\x1B[1;37m"
#define CYAN "\x1B[1;36m"

int main(void)
{
	pid_t pid = 0;
	int keep_running = 1;
	char command_line[100] = {'\0'};
	char *terminal_argv[] = {"gnome-terminal", "--", "./bin/fork_and_test", NULL};

	pid = fork();
	if (0 == pid)
	{
		/* Child process to start another terminal process */
		execvp(terminal_argv[0], terminal_argv);
		perror("execvp failed"); /* This will only be reached if execvp fails */
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		sleep(5);
		printf("%swatch dog test started, you can type a pid to kill or exit to quit.%s\n", CYAN, WHITE);
		while (keep_running)
		{
			fgets(command_line, 100, stdin);

			/* Remove trailing newline */
			command_line[strcspn(command_line, "\n")] = '\0';

			if (strcmp(command_line, "exit") == 0)
			{
				keep_running = 0;
				system("pkill -f './bin/fork_and_test'");
			}
			else
			{
				printf("%sreloading...%s\n", CYAN, WHITE);
				kill(atoi(command_line), SIGTERM);
				sleep(1);
				printf("%s...%s\n", CYAN, WHITE);
				sleep(1);
				printf("%s...%s\n", CYAN, WHITE);
				sleep(1);
				printf("%s...%s\n", CYAN, WHITE);
				sleep(1);
				printf("%s...%s\n", CYAN, WHITE);
				printf("%sReady! you can type a pid to kill or exit to quit.%s\n", CYAN, WHITE);
			}
		}
	}
	else
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}

	return 0;
}

