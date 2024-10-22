#define _POSIX_C_SOURCE 200112L

#include <assert.h>		/* assert() */
#include <stddef.h>		/* size_t, NULL */
#include <signal.h>		/* sigaction, SA_SIGINFO */
#include <stdatomic.h>	/* atomic_fetch_add(), atomic_store() */
#include <stdio.h>		/* printf(), perror() */
#include <semaphore.h>	/* sem_open(), sem_post(), sem_close() */
#include <unistd.h>		/* fork(), execvp(), getpid() */
#include <string.h>		/* strcpy(), strlen() */
#include <time.h>		/* time() */

#include "watch_dog_private.h"
#include "scheduler_v2.h"

static atomic_int counter = ATOMIC_VAR_INIT(0);
static scheduler_t *scheduler;
static atomic_int g_stop_flag = ATOMIC_VAR_INIT(0);
static struct sigaction oldact_sigusr1 = {0};
static struct sigaction oldact_sigusr2 = {0};

static void Connect(void *args);
static void KeepMeAlive(void *args);
static void KeepMeAliveAddTasks(void *args);

static int SendSignalAndIncrementCounter(void *params);
static int CheckConnection(void *params);
static int CheckPulse(void *params);

#define GREEN "\033[0;32m"
#define WHITE "\033[0;37m"

/****************************API FUNC**********************************/
void Start(void *args)
{
	assert(args);
	
	scheduler = SchedulerCreate();
	printf("%s%d:\t\tScheduler created.\n%s", GREEN, getpid(), WHITE);

	/* connection sequence */
	Connect(args);

	/* keep me alive sequence */
	KeepMeAlive(args);
}

/****************************SIGNAL HANDLERS*******************************/
void End(int signum)
{
	(void)signum;
	
	atomic_store(&g_stop_flag, 1);
}

void ZeroCounter(int signum)
{
	(void)signum;
	
	atomic_store(&counter, 0);
}

/****************************MMI SEQUENCES**********************************/
/**************************************************************************
	connect has two tasks:
	a.	send SIGUSR1 and increment counter. 
		every interval_in_seconds/repetitions seconds.
	b.	check if counter is lower then repetitions, if it is, 
		then connection is successfull.
		every interval_in_seconds seconds.
		
	once connection is detetected, the scheduler stops and the process moves
	on to KeepMeAlive
**************************************************************************/
static void Connect(void *args)
{
	struct thread_args *my_args = NULL;
	
	assert(args);
	my_args = (struct thread_args *)args;
	
	atomic_store(&counter, my_args->repetitions);
	
	SchedulerAdd(scheduler,
				 time(NULL) + 1, 
				 my_args->interval_in_seconds / my_args->repetitions, 
				 SendSignalAndIncrementCounter, 
				 args);

	SchedulerAdd(scheduler,
				 time(NULL) + 1 + my_args->repetitions, 
				 my_args->interval_in_seconds, 
				 CheckConnection, 
				 args);

	printf("%s%d:\t\tConnect: Scheduler run.\n%s", GREEN, getpid(), WHITE);
	SchedulerRun(scheduler);
	
	printf("%s%d:\t\tconnection detected.\n%s", GREEN, getpid(), WHITE);
	
	SchedulerClear(scheduler);
	printf("%s%d:\t\tScheduler cleared.\n%s", GREEN, getpid(), WHITE);
	
}


/**************************************************************************
	keep me alive has two tasks:
	a.	send SIGUSR1 and increment counter. 
		every interval_in_seconds/repetitions seconds.
		
	b.	check if counter is lower then repetitions, if it isn't, 
		then connection is lost and revive procedure is needed.
		every interval_in_seconds seconds.
		
	the scheduler runs until it recieves a SIGUSR2.
	after the scheduler is stopped, 
		*the func will block SIGUSR1 and SIGUSR2, 
		*destroy the scheduler 
		*and post the dnr sem to let the root thread know it's done 
**************************************************************************/
static void KeepMeAlive(void *args)
{
	sem_t *mmi_sem = NULL;	
	sem_t *dnr_sem = NULL;
	
	assert(args);
	
	printf("%s%d:\t\tKeepMeAlive started.\n%s", GREEN, getpid(), WHITE);
	KeepMeAliveAddTasks(args);
	
	mmi_sem = sem_open(MMI_SEM, 0);
	sem_post(mmi_sem);
	printf("%s%d:\t\tSemaphore posted.\n%s", GREEN, getpid(), WHITE);
	sem_close(mmi_sem);
	
	printf("%s%d:\t\tKeepMeAlive: Scheduler run.\n%s", GREEN, getpid(), WHITE);
	SchedulerRun(scheduler);

	/* block sigactions */
	SetSignalMask(SIG_BLOCK);
	
	printf("%s%d:\t\tScheduler stopped.\n%s", GREEN, getpid(), WHITE);
	
	SchedulerDestroy(scheduler);
	printf("%s%d:\t\tScheduler destroyed.\n%s", GREEN, getpid(), WHITE);
	
	dnr_sem = sem_open(DNR_SEM, 0);
	sem_post(dnr_sem);
	
	sem_close(dnr_sem);
}

static void KeepMeAliveAddTasks(void *args)
{
	struct thread_args *my_args = NULL;
	
	assert(args);
	my_args = (struct thread_args *)args;
	
	SchedulerAdd(scheduler,
				 time(NULL) + 1, 
				 my_args->interval_in_seconds / my_args->repetitions, 
				 SendSignalAndIncrementCounter, 
				 args);

	SchedulerAdd(scheduler,
				 time(NULL) + 1 + my_args->interval_in_seconds, 
				 my_args->interval_in_seconds, 
				 CheckPulse, 
				 args);
}

/*************************SCHELDULER ACTIONS*******************************/
static int SendSignalAndIncrementCounter(void *params)
{
	struct thread_args *my_args = NULL;
	
	assert(params);
	my_args = (struct thread_args *)params;
	
	if(atomic_load(&g_stop_flag))
	{
		kill(my_args->partner_pid, SIGUSR2);
		SchedulerStop(scheduler);
		return 0;
	}

	atomic_fetch_add(&counter, 1);
	kill(my_args->partner_pid, SIGUSR1);

	printf("%d:\t\tSignal sent to PID %d. Counter incremented.\n", getpid(), 
	my_args->partner_pid);

	return 0;
}

static int CheckConnection(void *params)
{
	struct thread_args *my_args = NULL;
	
	assert(params);
	my_args = (struct thread_args *)params;

	if ((size_t)atomic_load(&counter) < my_args->repetitions)
	{
		SchedulerStop(scheduler);
	}

	return 0;
}

static int CheckPulse(void *params)
{
	struct thread_args *my_args = NULL;
	pid_t partner = 0;
	
	assert(params);
	my_args = (struct thread_args *)params;
	
	if(atomic_load(&g_stop_flag))
	{
		kill(my_args->partner_pid, SIGUSR2);
		SchedulerStop(scheduler);
		return 0;
	}
	
	if ((size_t)atomic_load(&counter) >= my_args->repetitions)
	{	
		/* revive procedure */
		partner = fork();
		if (0 == partner) /* child */
		{
			printf("Revived Partner PID: %d\n", getpid());
			execvp(my_args->argv_to_revive[0], my_args->argv_to_revive);
		}
		else if (0 > partner)
		{
			printf("revive failed!\n");
			return 1;
		}
		else /* parent */
		{
			printf("Healer PID: %d\n", getpid());
			my_args->partner_pid = partner;
			
			atomic_store(&counter, 0);
		}
	}
	else
	{
		printf("%d:\t\tCheckPulse: value=%d, repetitions=%ld\n", getpid(), 
			atomic_load(&counter), my_args->repetitions);
	}
	
	return 0;
}
/*************************SIGACTION HELPER FUNCS*******************************/
void InitSigactions(void)
{
	struct sigaction sa_watchdog_send = {0};
	struct sigaction sa_watchdog_stop = {0};	
	
	/* define sigactions */
	sa_watchdog_send.sa_handler = ZeroCounter;
	sigaction(SIGUSR1, &sa_watchdog_send, &oldact_sigusr1);
	
	sa_watchdog_stop.sa_handler = End;
	sigaction(SIGUSR2, &sa_watchdog_stop, &oldact_sigusr2);
}

void RestoreSigactions(void)
{
	sigaction(SIGUSR1, &oldact_sigusr1, NULL);
	sigaction(SIGUSR2, &oldact_sigusr2, NULL);
}

void SetSignalMask(int blocking_status)
{
	sigset_t set = {0};

	/* blocks/unblocks SIGUSR1 & SIGUSR2 */
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigaddset(&set, SIGUSR2);
	pthread_sigmask(blocking_status, &set, NULL);
}

/*************************SERIALIZATION FUNCS*******************************/
void SerializeArgs(char *dest, struct thread_args *args)
{
	size_t offset = 0;
	size_t argc = 0;
	size_t i = 0;
	size_t len = 0;
	char buffer[BUFSIZE] = {'\0'}; /* Buffer for integer to string conversion */
	
	assert(dest);
	assert(args);

	/* Copy partner_pid */
	sprintf(buffer, "%d", args->partner_pid);
	strcpy(dest + offset, buffer);
	offset += strlen(buffer);
	dest[offset++] = '$';

	/* Copy interval_in_seconds */
	sprintf(buffer, "%ld", (long)args->interval_in_seconds);
	strcpy(dest + offset, buffer);
	offset += strlen(buffer);
	dest[offset++] = '$';

	/* Copy repetitions */
	sprintf(buffer, "%ld", (long)args->repetitions);
	strcpy(dest + offset, buffer);
	offset += strlen(buffer);
	dest[offset++] = '$';

	/* Copy argv_to_revive */
	while (args->argv_to_revive[argc] != NULL)
	{
		argc++;
	}
	sprintf(buffer, "%ld", (long)argc);
	strcpy(dest + offset, buffer);
	offset += strlen(buffer);
	dest[offset++] = '$';
	for (i = 0; i < argc; ++i)
	{
		len = strlen(args->argv_to_revive[i]) + 1;
		sprintf(buffer, "%ld", (long)len);
		strcpy(dest + offset, buffer);
		offset += strlen(buffer);
		dest[offset++] = '$';
		strcpy(dest + offset, args->argv_to_revive[i]);
		offset += len - 1;
		dest[offset++] = '$';
	}

}

void DeserializeToArgs(char *src, 
						struct thread_args *args, 
						char **argv_buffers)
{
	size_t argc = 0;
	size_t i = 0;
	size_t len = 0;
	char *token = NULL;
	char delimiters[] = "$";
	
	assert(src);
	assert(args);
	assert(argv_buffers);
	assert(argv_buffers[0]);

	/* Copy partner_pid */
	token = strtok(src, delimiters);
	sscanf(token, "%d", &(args->partner_pid));

	/* Copy interval_in_seconds */
	token = strtok(NULL, delimiters);
	sscanf(token, "%ld", (long *)&(args->interval_in_seconds));

	/* Copy repetitions */
	token = strtok(NULL, delimiters);
	sscanf(token, "%ld", (long *)&(args->repetitions));

	/* Copy argv_to_revive */
	token = strtok(NULL, delimiters);
	sscanf(token, "%ld", (long *)&argc);
	args->argv_to_revive = argv_buffers;
	for (i = 0; i < argc; ++i)
	{
		token = strtok(NULL, delimiters);
		sscanf(token, "%ld", (long *)&len);
		token = strtok(NULL, delimiters);
		strcpy(args->argv_to_revive[i], token);
	}
	args->argv_to_revive[argc] = NULL;

}
