#ifndef WATCH_DOG_PROCESS_H
#define WATCH_DOG_PROCESS_H

#define _POSIX_C_SOURCE 200112L

#include <sys/types.h> /* pid_t */
#include <signal.h>    /* siginfo_t, sigaction, signal constants */
#include <stddef.h>    /* size_t */

#define WATCH_DOG_PATH "./bin/watch_dog_main_exec"
#define MMI_SEM "sem1_watchdog"
#define DNR_SEM "sem2_watchdog"
#define BUFSIZE 64
#define MAX_ARGV_LENGTH 512

/* Struct for passing arguments to threads */
struct thread_args
{
	pid_t partner_pid;
	size_t interval_in_seconds;
	size_t repetitions;
	char **argv_to_revive;
};

void Start(void *args);
void End(int signum);
void ZeroCounter(int signum);
void InitSigactions(void);
void RestoreSigactions(void);
void SetSignalMask(int blocking_status);

void SerializeArgs(char *dest, struct thread_args *args);
void DeserializeToArgs(char *src, 
						struct thread_args *args, 
						char **argv_buffers);

#endif /* WATCH_DOG_PROCESS_H */

