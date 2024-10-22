#define _POSIX_C_SOURCE 200112L

#include <assert.h>		/* assert() */
#include <stddef.h>		/* size_t, NULL */
#include <signal.h>		/* sigaction, SA_SIGINFO */
#include <fcntl.h>		/* O_CREAT */
#include <semaphore.h>	/* sem_open(), sem_wait(), sem_close(), sem_unlink() */
#include <unistd.h>		/* kill(), fork() */
#include <stdio.h>		/* printf() */
#include <stdlib.h>		/* getenv() */
#include <pthread.h>	/* pthread_create(), pthread_join()*/

#include "watch_dog_private.h" /* SerializeArgs(), Start(), End(), */

static void *MMIHelper(void *args);
static void HandleChild(void *args);
static void HandleClientProcess(void *args);

static pthread_t mmi_helper_thread = {0};

static char wd_pathname[] = WATCH_DOG_PATH;
static pid_t watch_dog_pid = 0;
static size_t g_interval = 0;
static size_t g_reps = 0;

int MMI(size_t interval_in_seconds, size_t repetitions, char **argv)
{
	struct thread_args args = {0};
	char *env_var_wd = NULL;
	sem_t *mmi_sem = NULL;
	
	assert(argv);
	assert(argv[0]);
	
	/* zero semaphores */
	sem_unlink(MMI_SEM);
	sem_unlink(DNR_SEM);
	
	/* update static global vars for DNR */
	g_interval = interval_in_seconds;
	g_reps = repetitions;
	
	/* create semaphore */
	mmi_sem = sem_open(MMI_SEM, O_CREAT, 0666, 0);
	if (mmi_sem == SEM_FAILED) {
		perror("sem_open failed");
		return -1;
	}

	/* define args for watch dog process */
	args.interval_in_seconds = interval_in_seconds;
	args.repetitions = repetitions;
	args.partner_pid = getpid();
	args.argv_to_revive = argv;
	
	InitSigactions();
	
	env_var_wd = getenv("WD_PID");
	if(env_var_wd) /* watch_dog process is already running */
	{
		/* get watchdog pid from the env var */
		watch_dog_pid = atoi(env_var_wd);
		printf("\nClient PID: %d, WatchDog PID: %d\n", getpid(), watch_dog_pid);
		
		HandleClientProcess(&args);
	}
	else /* watch_dog process needs to be created */
	{
		watch_dog_pid = fork();

		if (0 == watch_dog_pid) /* child */
		{
			sem_close(mmi_sem);
			HandleChild(&args);
		}
		else if (0 > watch_dog_pid)
		{
			printf("fork failed!\n");
			sem_close(mmi_sem);
			return -1;
		}
		else /* parent */
		{
			printf("\nClient PID: %d, WatchDog PID: %d\n", getpid(), watch_dog_pid);
			
			HandleClientProcess(&args);
			sem_close(mmi_sem);
		}
	}
	
	
	return 0;
}

static void *MMIHelper(void *args)
{
	struct thread_args my_args = {0};
	char *wd_argv[3] = {NULL};
	char serizalized_args[MAX_ARGV_LENGTH] = {0};
	
	assert(args);
	my_args = *(struct thread_args *)args;
	
	/* unblock sigactions to the helper thread */
	SetSignalMask(SIG_UNBLOCK);
	
	/* serialize arguments to pass to revive procedure */
	SerializeArgs(serizalized_args, &my_args);
	
	wd_argv[0] = wd_pathname;
	wd_argv[1] = serizalized_args;
	wd_argv[2] = NULL;
	
	/* edit args to satisfy helper thread requirements */
	my_args.partner_pid = watch_dog_pid;
	my_args.argv_to_revive = wd_argv;;
	
	printf("\nthread calling Start...\n");

	Start(&my_args);

	return NULL;
}


void DNR(void)
{
	/* create semaphore */
	sem_t *dnr_sem = sem_open(DNR_SEM, O_CREAT, 0666, 0);
	
	/* send SIGUSR2 to helper thread */
	kill(getpid(), SIGUSR2);
	
	/* wait for both the helper thread and the watch dog process to stop */
	sem_wait(dnr_sem);
	sem_wait(dnr_sem);
	
	pthread_join(mmi_helper_thread, NULL);
	
	/* clean up */
	sem_close(dnr_sem);
	SetSignalMask(SIG_UNBLOCK);
	RestoreSigactions();
	sem_unlink(MMI_SEM);
	sem_unlink(DNR_SEM);
}

static void HandleChild(void *args)
{
	char serizalized_args[MAX_ARGV_LENGTH] = {0};
	char *argv[3] = {NULL};
	struct thread_args *my_args = NULL;
	
	assert(args);
	my_args = (struct thread_args *)args;
	/* serialize arguments for executing watch dog process */
	SerializeArgs(serizalized_args, my_args);
	
	argv[0] = wd_pathname;
	argv[1] = serizalized_args;
	argv[2] = NULL;

	execvp(argv[0], argv);
	perror("execvp");
}

static void HandleClientProcess(void *args)
{
	sem_t *mmi_sem = NULL;
	assert(args);
	
	mmi_sem = sem_open(MMI_SEM, 0);
	
	printf("\nhandling client process...\n");
	/* create helper thread */
	pthread_create(&mmi_helper_thread, NULL, MMIHelper, args);
	
	/* block sigactions to the root thread */
	SetSignalMask(SIG_BLOCK);

	/* wait for connection sequence to finish */
	sem_wait(mmi_sem);
	sem_close(mmi_sem);
}

