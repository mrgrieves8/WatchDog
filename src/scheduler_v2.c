#include <sys/types.h> /* size_t, time_t*/
#include <assert.h> /* assert() */
#include <stdlib.h> /* malloc(), free() */
#include <unistd.h> /* sleep() */
#include <time.h> /* time() */
#include <stdio.h>

#include "heap_pq.h"
#include "task.h"
#include "uid.h"
#include "scheduler_v2.h"

struct scheduler
{
	heap_pq_t *task_queue;
	int is_running;
};

static int SchedulerCompareTimes(const void *data, const void *param);
static time_t CalculteTimeToSleep(time_t exec_time);

scheduler_t *SchedulerCreate(void)
{
	scheduler_t *scheduler = (scheduler_t *)malloc(sizeof(scheduler_t));
		
	if (NULL == scheduler)
	{
		return NULL;
	}
	
	scheduler->task_queue = HeapPQCreate(SchedulerCompareTimes);
	
	if (NULL == scheduler->task_queue)
	{
		free(scheduler);
		return NULL;
	}
	scheduler->is_running = 0;
	
	return scheduler;
}


void SchedulerDestroy(scheduler_t *scheduler)
{
	assert(scheduler);
	assert(scheduler->task_queue);
	
	SchedulerClear(scheduler);
	HeapPQDestroy(scheduler->task_queue);
	free(scheduler);
}


ilrd_uid_t SchedulerAdd(scheduler_t *scheduler,
                    time_t exec_time, 
                    time_t interval_in_seconds, 
                    int (*action)(void *params), 
                    void *params)
{
	task_t *task = NULL;
	
	assert(scheduler);
	assert(action);
	assert(scheduler->task_queue);
	
	task = TaskCreate(exec_time, interval_in_seconds, action, params);
	
	if (!task)
	{
		return BadUID;
	}
	
	if(!HeapPQEnqueue(scheduler->task_queue, task))
	{
		return TaskGetUid(task);
	}
	
	return BadUID;
}


int SchedulerRemove(scheduler_t *scheduler, ilrd_uid_t uid)
{
	task_t *task = NULL;
	
	assert(scheduler);
	assert(scheduler->task_queue);
	
	task = (task_t *)HeapPQErase(scheduler->task_queue, TaskIsUidMatch, &uid);	
	
	if (task)
	{
		TaskDestroy(task);
		return SUCCESS;
	}
	else
	{
		return SCHEDULER_UID_NOT_FOUND;
	}
}


int SchedulerRun(scheduler_t *scheduler)
{
	task_t *task = NULL;
	time_t exec_time = 0;
	time_t interval = 0;
	time_t time_to_sleep = 0;
	int return_value = 0;
	
	assert(scheduler);
	assert(scheduler->task_queue);
	
	scheduler->is_running = 1;
	
	while(scheduler->is_running && !SchedulerIsEmpty(scheduler))
	{
		task = HeapPQPeek(scheduler->task_queue);
		exec_time = TaskGetExecTime(task);
		interval = TaskGetIntervalInSeconds(task);
		HeapPQDequeue(scheduler->task_queue);
		
		if (exec_time < time(NULL))
		{
			TaskDestroy(task);
		}
		else
		{
			time_to_sleep = CalculteTimeToSleep(exec_time);
			while (time_to_sleep)
			{
				time_to_sleep = sleep(time_to_sleep);			
			}

			return_value = TaskRun(task);
			
			if (interval && return_value == 0)
			{
				TaskSetExecTime(task, exec_time + interval);
				HeapPQEnqueue(scheduler->task_queue, task);
			}
			else
			{
				TaskDestroy(task);
			}
		}
	}
	
	if (!SchedulerIsEmpty(scheduler))
	{
		return SCHEDULER_STOP;
	}
	
	return SUCCESS;
}


void SchedulerStop(scheduler_t *scheduler)
{
	assert(scheduler);
	
	scheduler->is_running = 0;
}


size_t SchedulerSize(scheduler_t *scheduler)
{
	assert(scheduler);
	assert(scheduler->task_queue);
	
	return HeapPQSize(scheduler->task_queue);
}


int SchedulerIsEmpty(scheduler_t *scheduler)
{
	assert(scheduler);
	assert(scheduler->task_queue);
	
	return HeapPQIsEmpty(scheduler->task_queue);
}


void SchedulerClear(scheduler_t *scheduler)
{
	task_t *task = NULL;
	
	assert(scheduler);
	assert(scheduler->task_queue);
	
	while (!SchedulerIsEmpty(scheduler))
	{
		task = (task_t *)HeapPQDequeue(scheduler->task_queue);
		TaskDestroy(task);
	}
}

static int SchedulerCompareTimes(const void *data, const void *param)
{	
	return TaskGetExecTime((task_t *)data) - TaskGetExecTime((task_t *)param);
}

static time_t CalculteTimeToSleep(time_t exec_time)
{
	return exec_time - time(NULL);
}










