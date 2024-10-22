#include <stdio.h> /* printf() */
#include <string.h>  /* strcat() */

#include <sys/types.h> /*size_t, time_t*/

#include "task.h"
#include "uid.h"

#define TESTNUM 6

static char failed_tests_print[200] = {'\0'};

static int TestTaskRun(void);
static int TestTaskGetUid(void);
static int TestTaskGetExecTime(void);
static int TestTaskGetIntervalInSeconds(void);
static int TestTaskSetExecTime(void);
static int TestTaskIsSame(void);

int PrintInTest(void *str)
{
	printf("\n%s\n\n", (char *)str);
	return 0;
}

int IncrementInTest(void *count)
{
	++*(size_t *)count;
	return 0;
}

void AddFailedTest(const char *str);

int main(void)
{
	int failed_tests_num = 0;
	
	failed_tests_num += TestTaskRun();
	printf("Tested TaskRun\n");
	failed_tests_num += TestTaskGetUid();
	printf("Tested TaskGetUid\n");
	failed_tests_num += TestTaskGetExecTime();
	printf("Tested TaskGetExecTime\n");
	failed_tests_num += TestTaskGetIntervalInSeconds();
	printf("Tested TaskGetIntervalInSeconds\n");
	failed_tests_num += TestTaskSetExecTime();
	printf("Tested TaskSetExecTime\n");
	failed_tests_num += TestTaskIsSame();
	printf("Tested TaskIsSame\n");
	
	if (failed_tests_num)
	{
		printf("%d out %d tests failed\nFailed tests:\n%s"
		, failed_tests_num, TESTNUM, failed_tests_print);
		
		return failed_tests_num;
	}
	
	printf("All Tests Passed!\n");
	
    return 0;
}


void AddFailedTest(const char *str)
{
	strcat(failed_tests_print, str);
}

static int TestTaskRun(void)
{
	task_t *print = NULL;
	task_t *increment = NULL;
	size_t test_count = 0;
	size_t loop_count = 0;
	
	print = TaskCreate(51, 43, PrintInTest, "---Started testing TaskRun---");
	increment = TaskCreate(123, 22343, IncrementInTest, &test_count);
	
	TaskRun(print);
	for(; loop_count < 5; ++loop_count)
	{
		TaskRun(increment);
	}
	
	if (5 != test_count)
	{
		AddFailedTest("TestTaskRun\n");
		TaskDestroy(print);
		TaskDestroy(increment);
		
		return 1;
	}
	
	TaskDestroy(print);
	TaskDestroy(increment);
	
	return 0;
}

static int TestTaskGetUid(void)
{
	task_t *print = NULL;
	task_t *increment = NULL;
	time_t exec1 = 100;
	time_t exec2 = 200;
	time_t interval1 = 4;
	time_t interval2 = 5;
	ilrd_uid_t get_uid = BadUID;
	size_t test_count = 0;
	
	print = TaskCreate(exec1, interval1, PrintInTest,
						 "---Started testing TaskGetUid---");
				 
	increment = TaskCreate(exec2, interval2, IncrementInTest, &test_count);
	
	TaskRun(print);
	
	get_uid = TaskGetUid(print);
	if (IsSameUID(BadUID, get_uid))
	{
		AddFailedTest("TestTaskGetUid\n");
		TaskDestroy(print);
		TaskDestroy(increment);
		
		return 1;
	}
	
	get_uid = TaskGetUid(increment);
	if (IsSameUID(BadUID, get_uid))
	{
		AddFailedTest("TestTaskGetUid\n");
		TaskDestroy(print);
		TaskDestroy(increment);
		
		return 1;
	}
	
	TaskDestroy(print);
	TaskDestroy(increment);
	
	return 0;
}
	

static int TestTaskGetExecTime(void)
{
	task_t *print = NULL;
	task_t *increment = NULL;
	time_t exec1 = 100;
	time_t exec2 = 200;
	time_t interval1 = 4;
	time_t interval2 = 5;
	time_t get_exec_time = 0;
	size_t test_count = 0;
	
	print = TaskCreate(exec1, interval1, PrintInTest,
						 "---Started testing TaskGetExecTime---");
				 
	increment = TaskCreate(exec2, interval2, IncrementInTest, &test_count);
	
	TaskRun(print);
	
	get_exec_time = TaskGetExecTime(print);
	if (exec1 != get_exec_time)
	{
		AddFailedTest("TestTaskGetExecTime\n");
		TaskDestroy(print);
		TaskDestroy(increment);
		
		return 1;
	}
	
	get_exec_time = TaskGetExecTime(increment);
	if (exec2 != get_exec_time)
	{
		AddFailedTest("TestTaskGetExecTime\n");
		TaskDestroy(print);
		TaskDestroy(increment);
		
		return 1;
	}
	
	TaskDestroy(print);
	TaskDestroy(increment);
	
	return 0;
}


static int TestTaskGetIntervalInSeconds(void)
{
	task_t *print = NULL;
	task_t *increment = NULL;
	time_t exec1 = 100;
	time_t exec2 = 200;
	time_t interval1 = 4;
	time_t interval2 = 5;
	time_t get_interval = 0;
	size_t test_count = 0;
	
	print = TaskCreate(exec1, interval1, PrintInTest,
						 "---Started testing TaskGetIntervalInSeconds---");
				 
	increment = TaskCreate(exec2, interval2, IncrementInTest, &test_count);
	
	TaskRun(print);
	
	get_interval = TaskGetIntervalInSeconds(print);
	if (interval1 != get_interval)
	{
		AddFailedTest("TestTaskGetIntervalInSeconds1\n");
		TaskDestroy(print);
		TaskDestroy(increment);
		
		return 1;
	}
	
	get_interval = TaskGetIntervalInSeconds(increment);
	if (interval2 != get_interval)
	{
		AddFailedTest("TestTaskGetIntervalInSeconds2\n");
		TaskDestroy(print);
		TaskDestroy(increment);
		
		return 1;
	}
	
	TaskDestroy(print);
	TaskDestroy(increment);
	
	return 0;
}


static int TestTaskSetExecTime(void)
{
	task_t *print = NULL;
	time_t exec1 = 100;
	time_t exec2 = 200;
	time_t interval1 = 4;
	
	print = TaskCreate(exec1, interval1, PrintInTest,
						 "---Started testing TaskSetExecTime---");
				 
	TaskRun(print);
	 
	TaskSetExecTime(print, exec2);
	if (exec2 != TaskGetExecTime(print))
	{
		AddFailedTest("TestTaskSetExecTime\n");
		TaskDestroy(print);
		
		return 1;
	}
	
	if (exec2 == exec1)
	{
		AddFailedTest("TestTaskSetExecTime\n");
		TaskDestroy(print);
		
		return 1;
	}
	
	TaskSetExecTime(print, exec2 + TaskGetIntervalInSeconds(print));
	if (exec2 == TaskGetExecTime(print))
	{
		AddFailedTest("TestTaskSetExecTime\n");
		TaskDestroy(print);
		
		return 1;
	}
	
	if (exec2 + interval1 != TaskGetExecTime(print))
	{
		AddFailedTest("TestTaskSetExecTime\n");
		TaskDestroy(print);
		
		return 1;
	}
	
	TaskDestroy(print);
	
	return 0;
}

static int TestTaskIsSame(void)
{
	task_t *print = NULL;
	task_t *increment1 = NULL;
	task_t *increment2 = NULL;
	time_t exec1 = 100;
	time_t exec2 = 200;
	time_t interval1 = 4;
	time_t interval2 = 5;
	int is_same = 0;
	size_t test_count = 0;
	
	print = TaskCreate(exec1, interval1, PrintInTest,
						 "---Started testing TaskIsSame---");
	increment1 = TaskCreate(exec2, interval2, IncrementInTest, &test_count);
	increment2 = increment1;
	
	TaskRun(print);
	
	is_same = TaskIsSame(print, increment1);
	if (is_same)
	{
		AddFailedTest("TestTaskIsSame\n");
		TaskDestroy(print);
		TaskDestroy(increment1);
		
		return 1;
	}
	
	is_same = TaskIsSame(increment2, increment1);
	if (!is_same)
	{
		AddFailedTest("TestTaskIsSame\n");
		TaskDestroy(print);
		TaskDestroy(increment1);
		
		return 1;
	}
	
	TaskDestroy(print);
	TaskDestroy(increment1);
	
	return 0;
}












