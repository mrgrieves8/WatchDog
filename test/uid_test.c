#include <stdio.h> /* printf() */
#include <string.h>  /* strcat() */
#include <sys/types.h> /* pid_t, time_t, size_t */
#include <unistd.h> /* getpid() */

#include "uid.h"

#define TESTNUM 1

static char failed_tests_print[300] = {'\0'};

static void AddFailedTest(const char *str);

static int TestUid(void);


int main(void)
{
	int failed_tests_num = 0;
	
	failed_tests_num += TestUid();
	printf("Tested Uid\n");
	
	if (failed_tests_num)
	{
		printf("%d out %d tests failed\nFailed tests:\n%s"
		, failed_tests_num, TESTNUM, failed_tests_print);
		return failed_tests_num;
	}
	
	printf("All Tests Passed!\n");
	
    return 0;
}

static void AddFailedTest(const char *str)
{
	strcat(failed_tests_print, str);
}

static int TestUid(void)
{
	ilrd_uid_t uid1 = {0};
	ilrd_uid_t uid2 = {0};
	ilrd_uid_t uid3 = {0};
	ilrd_uid_t uid4 = {0};
	
	uid1 = UIDCreate();
	sleep(1);
	uid2 = UIDCreate();
	uid3 = UIDCreate();
	uid4 = uid2;
	
	if (getpid() != uid1.pid)
	{
		AddFailedTest("TestUid\n");
		return 1;
	}
	
	if (uid1.timestamp == uid2.timestamp)
	{
		AddFailedTest("TestUid\n");
		return 1;
	}
	
	if (uid1.timestamp == uid3.timestamp)
	{
		AddFailedTest("TestUid\n");
		return 1;
	}
	
	if (IsSameUID(uid2, uid3))
	{
		AddFailedTest("TestUid\n");
		return 1;
	}
	
	if (IsSameUID(uid1, BadUID))
	{
		AddFailedTest("TestUid\n");
		return 1;
	}
	
	if (!IsSameUID(uid2, uid4))
	{
		AddFailedTest("TestUid\n");
		return 1;
	}
	
	
	
	return 0;
}


