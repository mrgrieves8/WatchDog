#include <sys/types.h> /* size_t, time_t*/
#include <assert.h> /* assert() */
#include <stdlib.h> /* malloc(), free() */

#include "task.h"
#include "uid.h"

struct task
{
    ilrd_uid_t uid;
    time_t exe_time;
    time_t interval_in_seconds;
    int (*action)(void *params);
    void *params;
};


task_t *TaskCreate(time_t exe_time,
					time_t interval_in_seconds,
					int (*action)(void *params), 
					void *params)
{
	task_t *task = NULL;
	
	assert(action);
	
	task = (task_t *)malloc(sizeof(task_t));
		
	if (NULL == task)
	{
		return NULL;
	}
	
	task->uid = UIDCreate();
	
	if (IsSameUID(BadUID, task->uid))
	{
		free(task);
		return NULL;
	}
	
	task->exe_time = exe_time;
	task->interval_in_seconds = interval_in_seconds;
	task->action = action;
	task->params = params;
	
	return task;
}


void TaskDestroy(task_t *task)
{
	assert(task);
	
	free(task);
}


int TaskRun(task_t *task)
{
	assert(task);
	assert(task->action);
	
	return task->action(task->params);
}


ilrd_uid_t TaskGetUid(task_t *task)
{
	assert(task);
	
	return task->uid;
}


time_t TaskGetExecTime(task_t *task)
{
	assert(task);
	
	return task->exe_time;
}

time_t TaskGetIntervalInSeconds(task_t *task)
{
	assert(task);
	
	return task->interval_in_seconds;
}


void TaskSetExecTime(task_t *task, time_t new_exec_time)
{
	assert(task);
	
	task->exe_time = new_exec_time;
}

int TaskIsUidMatch(const void *task, const void *uid)
{
	return (IsSameUID(((task_t *)task)->uid, *(ilrd_uid_t *)uid));
}

int TaskIsSame(task_t *task1, task_t *task2)
{
	return (IsSameUID(task1->uid, task2->uid));
}
