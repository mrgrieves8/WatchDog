#include <stdio.h> /* printf() */
#include <string.h>  /* strcat() */
#include <unistd.h> /* sleep() */
#include <time.h> /* time() */
#include <sys/types.h> /*size_t, time_t*/

#include "scheduler_v2.h"
#include "uid.h"

#define TESTNUM 7

typedef struct test_case
{
	size_t counter;
	ilrd_uid_t uid;
	scheduler_t *scheduler;
} test_case_t;

static char failed_tests_print[200] = {'\0'};

static int TestSchedulerAdd(void);
static int TestSchedulerRemove(void);
static int TestSchedulerRun(void);
static int TestSchedulerStop(void);

static int TestSchedulerSize(void);
static int TestSchedulerIsEmpty(void);
static int TestSchedulerClear(void);

int CountToThreeAndRemove(void *test_case)
{
	if (((test_case_t *)test_case)->counter < 3)
	{
		++((test_case_t *)test_case)->counter;
		return 0;
	}
	else
	{
		return 1;
	}
}

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

int StopInTest(void *scheduler)
{
	SchedulerStop(scheduler);
	return 0;
}

void AddFailedTest(const char *str);

int main(void)
{
	int failed_tests_num = 0;
	
	
	failed_tests_num += TestSchedulerAdd();
	printf("Tested SchedulerAdd\n");
	failed_tests_num += TestSchedulerRemove();
	printf("Tested SchedulerRemove\n");
	
	failed_tests_num += TestSchedulerRun();
	printf("Tested SchedulerRun\n");
	failed_tests_num += TestSchedulerStop();
	printf("Tested SchedulerStop\n");
	
	failed_tests_num += TestSchedulerSize();
	printf("Tested SchedulerSize\n");
	failed_tests_num += TestSchedulerIsEmpty();
	printf("Tested SchedulerIsEmpty\n");
	failed_tests_num += TestSchedulerClear();
	printf("Tested SchedulerClear\n");
	
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

static int TestSchedulerAdd(void)
{
	scheduler_t *scheduler = SchedulerCreate();
	ilrd_uid_t get_uid1 = BadUID;
	ilrd_uid_t get_uid2 = BadUID;
	ilrd_uid_t get_uid3 = BadUID;
	size_t test_count = 0;
	size_t get_size = 0;
	int status = 0;
	
	get_uid1 = SchedulerAdd(scheduler, time(NULL) + 6, 0, IncrementInTest,
															 &test_count);	
	if (IsSameUID(BadUID, get_uid1))
	{
		AddFailedTest("TestSchedulerAdd1\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	

	get_uid2 = SchedulerAdd(scheduler, time(NULL) + 3, 0, PrintInTest,
										"---Started testing SchedulerAdd---");
	if (IsSameUID(BadUID, get_uid2))
	{
		AddFailedTest("TestSchedulerAdd2\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	get_uid3 = SchedulerAdd(scheduler, time(NULL) + 4, 0, StopInTest,scheduler);
	
	if (IsSameUID(BadUID, get_uid3))
	{
		AddFailedTest("TestSchedulerAdd3\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	get_size = SchedulerSize(scheduler);
	if (3 != get_size)
	{
		AddFailedTest("TestSchedulerAdd4\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	status = SchedulerRun(scheduler);
	if (SCHEDULER_STOP != status)
	{
		AddFailedTest("TestSchedulerAdd5\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	get_size = SchedulerSize(scheduler);
	if (1 != get_size)
	{
		AddFailedTest("TestSchedulerAdd6\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	if (0 != test_count)
	{
		AddFailedTest("TestSchedulerAdd7\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	SchedulerAdd(scheduler, time(NULL) + 1, 0, PrintInTest,
										"---SchedulerAdd second run---");

	status = SchedulerRun(scheduler);
	if (SUCCESS != status)
	{
		AddFailedTest("TestSchedulerAdd8\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	if (1 != test_count)
	{
		AddFailedTest("TestSchedulerAdd9\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	if (!SchedulerIsEmpty(scheduler))
	{
		AddFailedTest("TestSchedulerAdd10\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}

	SchedulerDestroy(scheduler);
	
	return 0;
}

static int TestSchedulerRemove(void)
{
	scheduler_t *scheduler = SchedulerCreate();
	ilrd_uid_t get_uid1 = BadUID;
	ilrd_uid_t get_uid2 = BadUID;
	ilrd_uid_t get_uid3 = BadUID;
	size_t test_count = 0;
	size_t get_size = 0;
	int status = 0;
	test_case_t my_case = {0};
	
	get_uid1 = SchedulerAdd(scheduler, time(NULL) + 3, 0, PrintInTest,
									"---Started testing SchedulerRemove---");
	get_uid2 = SchedulerAdd(scheduler, time(NULL) + 4, 1, IncrementInTest,
															 &test_count);
															 
	get_uid3 = SchedulerAdd(scheduler, time(NULL) + 4, 0, StopInTest, 
															scheduler);
															 
	status = SchedulerRemove(scheduler, get_uid2);
	if (SUCCESS != status)
	{
		AddFailedTest("TestSchedulerRemove1\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	status = SchedulerRun(scheduler);
	if (SUCCESS != status)
	{
		AddFailedTest("TestSchedulerRemove2\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	if (0 != test_count)
	{
		AddFailedTest("TestSchedulerRemove3\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
						 
	get_size = SchedulerSize(scheduler);
	if (0 != get_size)
	{
		AddFailedTest("TestSchedulerRemove5\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	get_uid1 = SchedulerAdd(scheduler, time(NULL) + 2, 0, PrintInTest,
									"---SchedulerRemove second run---");
	get_uid2 = SchedulerAdd(scheduler, time(NULL) + 2, 1, IncrementInTest,
															 &test_count);
															 
	get_uid3 = SchedulerAdd(scheduler, time(NULL) + 4, 1, StopInTest, 
															scheduler);
															
															
	status = SchedulerRun(scheduler);
	if (SCHEDULER_STOP != status)
	{
		AddFailedTest("TestSchedulerRemove6\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
						 
	get_size = SchedulerSize(scheduler);
	if (2 != get_size)
	{
		AddFailedTest("TestSchedulerRemove7\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	if (0 == test_count)
	{
		AddFailedTest("TestSchedulerRemove8\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	status = SchedulerRemove(scheduler, get_uid1);
	if (SCHEDULER_UID_NOT_FOUND != status)
	{
		AddFailedTest("TestSchedulerRemove9\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	status = SchedulerRemove(scheduler, get_uid2);
	if (SUCCESS != status)
	{
		AddFailedTest("TestSchedulerRemove10\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	status = SchedulerRemove(scheduler, get_uid3);
	if (SUCCESS != status)
	{
		AddFailedTest("TestSchedulerRemove11\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
						 
	get_size = SchedulerSize(scheduler);
	if (0 != get_size)
	{
		AddFailedTest("TestSchedulerRemove12\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	if (!SchedulerIsEmpty(scheduler))
	{
		AddFailedTest("TestSchedulerRemove13\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	get_uid1 = SchedulerAdd(scheduler, time(NULL) + 2, 1, CountToThreeAndRemove,
															 &my_case);
	
	my_case.counter = 0;
	my_case.uid = get_uid1;
	my_case.scheduler = scheduler;
	
	status = SchedulerRun(scheduler);
	if (SUCCESS != status)
	{
		AddFailedTest("TestSchedulerRemove14\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	if (3 != my_case.counter)
	{
		AddFailedTest("TestSchedulerRemove15\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}

	SchedulerDestroy(scheduler);
	
	return 0;
}
	
	
static int TestSchedulerRun(void)
{
	scheduler_t *scheduler = SchedulerCreate();
	size_t test_count = 0;
	int status = 0;
	
	SchedulerAdd(scheduler, time(NULL) - 1, 0, IncrementInTest,
															 &test_count);
															 
	SchedulerAdd(scheduler, time(NULL) - 3, 0, PrintInTest,
							"---SchedulerRun runs irrelevant tasks---");
	
	SchedulerAdd(scheduler, time(NULL) + 0, 0, StopInTest,
															 scheduler);
															 
	SchedulerRun(scheduler);
	
	if (0 != test_count)
	{
		AddFailedTest("TestSchedulerRun0\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	SchedulerAdd(scheduler, time(NULL) + 3, 0, PrintInTest,
										"---Started testing SchedulerRun---");
	SchedulerAdd(scheduler, time(NULL) + 4, 1, IncrementInTest,
															 &test_count);
	SchedulerAdd(scheduler, time(NULL) + 9, 0, StopInTest,
															 scheduler);
	
	status = SchedulerRun(scheduler);
	
	if (SCHEDULER_STOP != status)
	{
		AddFailedTest("TestSchedulerRun1\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	if (test_count < 5)
	{
		AddFailedTest("TestSchedulerRun2\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	SchedulerClear(scheduler);

	SchedulerAdd(scheduler, time(NULL) + 3, 0, PrintInTest,
										"---SchedulerRun second run---");
	SchedulerAdd(scheduler, time(NULL) + 4, 0, IncrementInTest,
															 &test_count);
	SchedulerAdd(scheduler, time(NULL) + 5, 0, IncrementInTest,
															 &test_count);					
	status = SchedulerRun(scheduler);
	if (SUCCESS != status)
	{
		AddFailedTest("TestSchedulerRun3\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
										
	SchedulerDestroy(scheduler);
	
	return 0;
}



static int TestSchedulerStop(void)
{
	scheduler_t *scheduler = SchedulerCreate();
	size_t test_count = 0;
	
	SchedulerAdd(scheduler, time(NULL) + 2, 0, PrintInTest,
										"---Started testing SchedulerStop---");
	SchedulerAdd(scheduler, time(NULL) + 3, 1, IncrementInTest,
															 &test_count);
	SchedulerAdd(scheduler, time(NULL) + 8, 1, StopInTest,
															 scheduler);
	
	SchedulerRun(scheduler);


	if (test_count < 5)
	{
		AddFailedTest("TestSchedulerStop1\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	SchedulerAdd(scheduler, time(NULL), 0, PrintInTest,
										"---SchedulerStop second run---");
	
	SchedulerRun(scheduler);

	if (test_count > 7)
	{
		AddFailedTest("TestSchedulerStop2\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}

	SchedulerDestroy(scheduler);
	
	return 0;
}

static int TestSchedulerSize(void)
{
	scheduler_t *scheduler = SchedulerCreate();
	ilrd_uid_t get_uid1 = BadUID;
	ilrd_uid_t get_uid2 = BadUID;
	ilrd_uid_t get_uid3 = BadUID;
	size_t test_count = 0;
	size_t get_size = 0;
	
	get_uid1 = SchedulerAdd(scheduler, time(NULL) + 2, 0, PrintInTest,
										"---Started testing SchedulerSize---");
	get_uid2 = SchedulerAdd(scheduler, time(NULL) + 3, 0, IncrementInTest,
															 &test_count);
	get_uid3 = SchedulerAdd(scheduler, time(NULL) + 4, 0, StopInTest,
															 scheduler);
	SchedulerRun(scheduler);
	
	get_size = SchedulerSize(scheduler);
	if (0 != get_size)
	{
		AddFailedTest("TestSchedulerSize1\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	get_uid1 = SchedulerAdd(scheduler, time(NULL) + 2, 0, PrintInTest,
										"---Started testing SchedulerSize---");
	
	get_size = SchedulerSize(scheduler);
	if (1 != get_size)
	{
		AddFailedTest("TestSchedulerSize2\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	get_uid2 = SchedulerAdd(scheduler, time(NULL) + 3, 0, IncrementInTest, 
															&test_count);
	
	get_size = SchedulerSize(scheduler);
	if (2 != get_size)
	{
		AddFailedTest("TestSchedulerSize3\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}															

	get_uid3 = SchedulerAdd(scheduler, time(NULL) + 4, 0, StopInTest,
																scheduler);
	get_size = SchedulerSize(scheduler);
	if (3 != get_size)
	{
		AddFailedTest("TestSchedulerSize4\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}								
						 
	SchedulerRemove(scheduler, get_uid1);
	get_size = SchedulerSize(scheduler);
	if (2 != get_size)
	{
		AddFailedTest("TestSchedulerSize5\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	SchedulerRemove(scheduler, get_uid3);
	get_size = SchedulerSize(scheduler);
	if (1 != get_size)
	{
		AddFailedTest("TestSchedulerSize6\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	SchedulerRemove(scheduler, get_uid2);
	get_size = SchedulerSize(scheduler);
	if (0 != get_size)
	{
		AddFailedTest("TestSchedulerSize7\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}

	SchedulerDestroy(scheduler);
	
	return 0;
}


static int TestSchedulerIsEmpty(void)
{
	scheduler_t *scheduler = SchedulerCreate();
	ilrd_uid_t get_uid1 = BadUID;
	
	if (!SchedulerIsEmpty(scheduler))
	{
		AddFailedTest("TestSchedulerIsEmpty1\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	get_uid1 = SchedulerAdd(scheduler, time(NULL) + 3, 0, PrintInTest,
								"---Started testing SchedulerIsEmpty---");
	if (SchedulerIsEmpty(scheduler))
	{
		AddFailedTest("TestSchedulerIsEmpty2\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	SchedulerRemove(scheduler, get_uid1);
	if (!SchedulerIsEmpty(scheduler))
	{
		AddFailedTest("TestSchedulerIsEmpty3\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	SchedulerDestroy(scheduler);
	
	return 0;
}


static int TestSchedulerClear(void)
{
	scheduler_t *scheduler = SchedulerCreate();
	size_t test_count = 0;
	size_t get_size = 0;
	
	
	SchedulerAdd(scheduler, time(NULL) + 2, 0, PrintInTest,
										"---Started testing SchedulerClear---");
	SchedulerAdd(scheduler, time(NULL) + 3, 0, IncrementInTest, 
															&test_count);
															
	SchedulerClear(scheduler);
	
	if (!SchedulerIsEmpty(scheduler))
	{
		AddFailedTest("TestSchedulerClear\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	get_size = SchedulerSize(scheduler);
	if (0 != get_size)
	{
		AddFailedTest("TestSchedulerClear\n");
		SchedulerDestroy(scheduler);
		
		return 1;
	}
	
	SchedulerDestroy(scheduler);
	
	return 0;
}















