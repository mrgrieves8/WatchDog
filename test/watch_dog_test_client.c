#define _POSIX_C_SOURCE 200112L

#include <stdio.h>		/* printf() */
#include <stdlib.h>		/* atoi() */
#include <unistd.h>		/* sleep() */

#include "watch_dog.h"	/* MMI(), DNR() */

#define PURPLE "\x1B[1;35m"
#define WHITE "\033[0;37m"

int main(int argc, char *argv[])
{
	size_t interval_in_seconds = 0;
	size_t repetitions = 0;
	size_t test_count = 1;

	if (argc < 3)
	{
		printf("Usage: %s <interval_in_seconds> <repetitions>\n", argv[0]);
		return 1;
	}

	interval_in_seconds = (size_t)atoi(argv[1]);
	repetitions = (size_t)atoi(argv[2]);

	MMI(interval_in_seconds, repetitions, argv);
	printf("\n%sMMI returned.%s\n", PURPLE, WHITE);
	
	while(test_count <= 30)
	{
		printf("\t%s%ld!!!%s\n", PURPLE, test_count++, WHITE);
		sleep(1);
	}
	
	printf("%s\ncalling DNR...\n%s", PURPLE, WHITE);
	DNR();

	printf("%s\ntest finished.\n%s", PURPLE, WHITE);
	return 0;
}

