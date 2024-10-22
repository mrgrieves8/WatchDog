#include <stdio.h> /* printf() */
#include <string.h>  /* strcat() */

#include "dvector.h"

#define TESTNUM 7

static char failed_tests_print[200] = {'\0'};

int TestPush();
int TestPop();
int TestGetElement();
int TestShrink();
int TestReserve();
int TestGetSize();
int TestGetCapacity();
void AddFailedTest(const char *str);

int main(void)
{
	int failed_tests_num = 0;
	
	failed_tests_num += TestPush();
	failed_tests_num += TestPop();
	failed_tests_num += TestGetElement();
	failed_tests_num += TestShrink();
	failed_tests_num += TestReserve();
	failed_tests_num += TestGetSize();
	failed_tests_num += TestGetCapacity();
	
	if (failed_tests_num)
	{
		printf("%d out %d tests failed\nFailed tests:\n%s"
		, failed_tests_num, TESTNUM, failed_tests_print);
		return failed_tests_num;
	}
	
	printf("All Tests Passed!\n");
	
    return 0;
}

int TestPush()
{
	dvector_t *int_vector = DvectorCreate(2, sizeof(int));
	int i_1 = 8;
	int i_2 = 80000;
	int i_3 = 9;
	int get_element = 0;
	int status = 0;
	
	status = DvectorPushBack(int_vector,&i_1);
	
	if (0 != status)
	{
		AddFailedTest("TestPush\n");
		return 1;
	}
	
	status = DvectorPushBack(int_vector,&i_2);
	
	if (0 != status)
	{
		AddFailedTest("TestPush\n");
		return 1;
	}
	
	if (DvectorCapacity(int_vector) != 2)
	{
		AddFailedTest("TestPush\n");
		return 1;
	}
	
	status = DvectorPushBack(int_vector,&i_3);
	
	if (0 != status)
	{
		AddFailedTest("TestPush\n");
		return 1;
	}
	
	if (DvectorSize(int_vector) != 3)
	{
		AddFailedTest("TestPush\n");
		return 1;
	}
	if (DvectorCapacity(int_vector) != 4)
	{
		AddFailedTest("TestPush\n");
		return 1;
	}
	get_element = *(int *)DvectorGetElement(int_vector, 0);
	if (get_element != 8)
	{
		AddFailedTest("TestPush\n");
		return 1;
	}
	get_element = *(int *)DvectorGetElement(int_vector, 1);
	if (get_element != 80000)
	{
		AddFailedTest("TestPush\n");
		return 1;
	}
	
	
	
	DvectorDestroy(int_vector);
	return 0;
}

int TestPop()
{
	dvector_t *int_vector = DvectorCreate(10, sizeof(int));
	int i_1 = 8;
	int i_2 = 80000;
	int get_element = 0;
	
	DvectorPushBack(int_vector,&i_1);
	DvectorPushBack(int_vector,&i_2);
	
	DvectorPopBack(int_vector);
	
	if (DvectorSize(int_vector) != 1)
	{
		AddFailedTest("TestPop\n");
		return 1;
	}
	
	get_element = *(int *)DvectorGetElement(int_vector, 0);
	if (get_element != 8)
	{
		AddFailedTest("TestPop\n");
		return 1;
	}
	
	DvectorPopBack(int_vector);
	
	if (DvectorSize(int_vector) != 0)
	{
		AddFailedTest("TestPop\n");
		return 1;
	}
	
	DvectorDestroy(int_vector);
	return 0;	
}

int TestGetElement()
{
	dvector_t *int_vector = DvectorCreate(2, sizeof(int));
	int i_1 = 8;
	int i_2 = 80000;
	int i_3 = 9;
	int get_element = 0;
	
	DvectorPushBack(int_vector,&i_1);
	DvectorPushBack(int_vector,&i_2);
	DvectorPushBack(int_vector,&i_3);

	get_element = *(int *)DvectorGetElement(int_vector, 0);
	if (get_element != 8)
	{
		AddFailedTest("TestGetElement\n");
		return 1;
	}
	get_element = *(int *)DvectorGetElement(int_vector, 1);
	if (get_element != 80000)
	{
		AddFailedTest("TestGetElement\n");
		return 1;
	}
	get_element = *(int *)DvectorGetElement(int_vector, 2);
	if (get_element != 9)
	{
		AddFailedTest("TestGetElement\n");
		return 1;
	}
	
	DvectorDestroy(int_vector);
	return 0;
}

int TestShrink()
{
	dvector_t *int_vector = DvectorCreate(10, sizeof(int));
	int i_1 = 8;
	int i_2 = 80000;
	int status = 0;
	
	DvectorPushBack(int_vector,&i_1);
	DvectorPushBack(int_vector,&i_2);
	
	status = DvectorShrink(int_vector);
	
	if (0 != status)
	{
		AddFailedTest("TestShrink\n");
		return 1;
	}

	if (2 != DvectorCapacity(int_vector))
	{
		AddFailedTest("TestShrink\n");
		return 1;
	}
	
	DvectorDestroy(int_vector);
	return 0;
}

int TestReserve()
{
	dvector_t *int_vector = DvectorCreate(10, sizeof(int));
	int i_1 = 8;
	int i_2 = 80000;
	int status = 0;
	
	DvectorPushBack(int_vector,&i_1);
	DvectorPushBack(int_vector,&i_2);
	
	status = DvectorReserve(int_vector, 6);
	
	if (0 != status)
	{
		AddFailedTest("TestReserve\n");
		return 1;
	}

	if (6 != DvectorCapacity(int_vector))
	{
		AddFailedTest("TestReserve\n");
		return 1;
	}
	
	DvectorDestroy(int_vector);
	return 0;
}

int TestGetSize()
{
	dvector_t *int_vector = DvectorCreate(2, sizeof(int));
	int i_1 = 8;
	int i_2 = 80000;
	int i_3 = 9;
	
	DvectorPushBack(int_vector,&i_1);
	
	if (DvectorSize(int_vector) != 1)
	{
		AddFailedTest("TestGetSize\n");
		return 1;
	}
	
	DvectorPushBack(int_vector,&i_2);
	
	if (DvectorSize(int_vector) != 2)
	{
		AddFailedTest("TestGetSize\n");
		return 1;
	}
	
	DvectorPushBack(int_vector,&i_3);
	
	if (DvectorSize(int_vector) != 3)
	{
		AddFailedTest("TestGetSize\n");
		return 1;
	}
	
	DvectorPopBack(int_vector);
	
	if (DvectorSize(int_vector) != 2)
	{
		AddFailedTest("TestGetSize\n");
		return 1;
	}
	
	DvectorPopBack(int_vector);
	
	if (DvectorSize(int_vector) != 1)
	{
		AddFailedTest("TestGetSize\n");
		return 1;
	}
	DvectorPopBack(int_vector);
	
	if (DvectorSize(int_vector) != 0)
	{
		AddFailedTest("TestGetSize\n");
		return 1;
	}
	
	DvectorDestroy(int_vector);
	return 0;
}

int TestGetCapacity()
{
	dvector_t *int_vector = DvectorCreate(2, sizeof(int));
	int i_1 = 8;
	int i_2 = 80000;
	int i_3 = 9;
	
	if (2 != DvectorCapacity(int_vector))
	{
		AddFailedTest("TestGetCapacity\n");
		return 1;
	}
	
	DvectorPushBack(int_vector,&i_1);
	DvectorPushBack(int_vector,&i_2);
	DvectorPushBack(int_vector,&i_3);
	
	if (4 != DvectorCapacity(int_vector))
	{
		AddFailedTest("TestGetCapacity\n");
		return 1;
	}
	
	DvectorShrink(int_vector);
	
	if (3 != DvectorCapacity(int_vector))
	{
		AddFailedTest("TestGetCapacity\n");
		return 1;
	}
	
	DvectorReserve(int_vector, 5);
	
	if (5 != DvectorCapacity(int_vector))
	{
		AddFailedTest("TestGetCapacity\n");
		return 1;
	}
	
	DvectorDestroy(int_vector);
	return 0;
}

void AddFailedTest(const char *str)
{
	strcat(failed_tests_print, str);
}




